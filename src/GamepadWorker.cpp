/*
 * This file is part of OpenWinControls.
 * Copyright (C) 2026 kylon
 *
 * OpenWinControls is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenWinControls is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QCoreApplication>
#include <QThread>

#include "GamepadWorker.h"
#include "extern/SDL/include/SDL3/SDL_init.h"
#include "extern/SDL/include/SDL3/SDL_hints.h"
#include "extern/SDL/include/SDL3/SDL_events.h"

namespace OWC {
    GamepadWorker::~GamepadWorker() {
        for (auto [jid, gpad]: sdlGamepadMap.asKeyValueRange())
            SDL_CloseGamepad(gpad);

        SDL_Quit();
    }

    bool GamepadWorker::isDeadzone(const int axis) const {
        return axis >= -deadzone && axis <= deadzone;
    }

    void GamepadWorker::startSDLEventsThread() {
        SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "0");

        if (!SDL_Init(SDL_INIT_GAMEPAD)) {
            emit logSent("Failed to init SDL gamepad subsystem");
            emit initFail();
            return;
        }

        while (true) {
            SDL_Event evt;
            const bool hasEvent = SDL_PollEvent(&evt);

            if (hasEvent && evt.type == SDL_EVENT_QUIT) [[unlikely]]
                break;

            QCoreApplication::processEvents();

            if (!hasEvent) [[likely]] {
                QThread::msleep(15);
                continue;
            }

            switch (evt.type) {
                case SDL_EVENT_GAMEPAD_ADDED: {
                    // sdl may send multiple events, skip if existing
                    if (sdlGamepadMap.contains(evt.gdevice.which)) [[unlikely]]
                        break;

                    const SDL_JoystickID id = evt.gdevice.which;
                    SDL_Gamepad *pad = SDL_OpenGamepad(id);

                    if (pad == nullptr) {
                        emit logSent(QString("Gamepad connection error: %1").arg(SDL_GetError()));
                        SDL_ClearError();
                        break;
                    }

                    sdlGamepadMap.insert(id, pad);
                }
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    if (sdlGamepadMap.contains(evt.gdevice.which)) [[likely]]
                        SDL_CloseGamepad(sdlGamepadMap.take(evt.gdevice.which));
                }
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    if (!enabled)
                        break;

                    switch (evt.gbutton.button) {
                        case SDL_GAMEPAD_BUTTON_SOUTH:
                            emit gamepadButton("BTN_A");
                            break;
                        case SDL_GAMEPAD_BUTTON_EAST:
                            emit gamepadButton("BTN_B");
                            break;
                        case SDL_GAMEPAD_BUTTON_WEST:
                            emit gamepadButton("BTN_X");
                            break;
                        case SDL_GAMEPAD_BUTTON_NORTH:
                            emit gamepadButton("BTN_Y");
                            break;
                        case SDL_GAMEPAD_BUTTON_START:
                            emit gamepadButton("START");
                            break;
                        case SDL_GAMEPAD_BUTTON_BACK:
                            emit gamepadButton("SELECT");
                            break;
                        case SDL_GAMEPAD_BUTTON_GUIDE:
                            emit gamepadButton("MENU");
                            break;
                        case SDL_GAMEPAD_BUTTON_LEFT_STICK:
                            emit gamepadButton("L3");
                            break;
                        case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
                            emit gamepadButton("R3");
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_UP:
                            emit gamepadButton("DPAD_UP");
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
                            emit gamepadButton("DPAD_DOWN");
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                            emit gamepadButton("DPAD_LEFT");
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                            emit gamepadButton("DPAD_RIGHT");
                            break;
                        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
                            emit gamepadButton("L1");
                            break;
                        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
                            emit gamepadButton("R1");
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    if (!enabled)
                        break;

                    switch (evt.gaxis.axis) {
                        case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: {
                            if (evt.gaxis.value == SDL_JOYSTICK_AXIS_MAX)
                                emit gamepadButton("L2");
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: {
                            if (evt.gaxis.value == SDL_JOYSTICK_AXIS_MAX)
                                emit gamepadButton("R2");
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_LEFTX: {
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.leftX);

                            axisState.leftX = std::abs(evt.gaxis.value);

                            if (axisState.leftY > axisState.leftX || hasDrift)
                                break;

                            if (evt.gaxis.value > deadzone)
                                emit gamepadButton("LSTICK_RIGHT");
                            else if (evt.gaxis.value < -deadzone)
                                emit gamepadButton("LSTICK_LEFT");
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_LEFTY: {
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.leftY);

                            axisState.leftY = std::abs(evt.gaxis.value);

                            if (axisState.leftX > axisState.leftY || hasDrift)
                                break;

                            if (evt.gaxis.value > deadzone)
                                emit gamepadButton("LSTICK_DOWN");
                            else if (evt.gaxis.value < -deadzone)
                                emit gamepadButton("LSTICK_UP");
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_RIGHTX: {
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.rightX);

                            axisState.rightX = std::abs(evt.gaxis.value);

                            if (axisState.rightY > axisState.rightX || hasDrift)
                                break;

                            if (evt.gaxis.value > deadzone)
                                emit gamepadButton("RSTICK_RIGHT");
                            else if (evt.gaxis.value < -deadzone)
                                emit gamepadButton("RSTICK_LEFT");
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_RIGHTY: {
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.rightY);

                            axisState.rightY = std::abs(evt.gaxis.value);

                            if (axisState.rightX > axisState.rightY || hasDrift)
                                break;

                            if (evt.gaxis.value > deadzone)
                                emit gamepadButton("RSTICK_DOWN");
                            else if (evt.gaxis.value < -deadzone)
                                emit gamepadButton("RSTICK_UP");
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }

    void GamepadWorker::enableEvents(const bool enable) {
        enabled = enable;
    }
}
