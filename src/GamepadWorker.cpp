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

namespace OWC {
    using namespace Qt::StringLiterals;

    GamepadWorker::~GamepadWorker() {
        for (auto [jid, gpad]: gamepadsMap.asKeyValueRange())
            SDL_CloseGamepad(gpad);

        SDL_Quit();
    }

    short GamepadWorker::getAxisState(const Sint16 axisValue) const {
        if (axisValue >= -axisMax && axisValue <= axisMax) // deadzone
            return 0;

        return axisValue > 0 ? 1 : -1;
    }

    void GamepadWorker::handleEvent(const SDL_Event &evt) {
        switch (evt.type) {
            case SDL_EVENT_GAMEPAD_ADDED: {
                if (gamepadsMap.contains(evt.gdevice.which)) [[unlikely]]
                    break; // skip if duplicate

                const SDL_JoystickID id = evt.gdevice.which;
                SDL_Gamepad *pad = SDL_OpenGamepad(id);

                if (pad == nullptr) {
                    emit logSent(u"Gamepad connection error: %1"_s.arg(SDL_GetError()));
                    SDL_ClearError();
                    break;
                }

                gamepadsMap.insert(id, pad);
            }
                break;
            case SDL_EVENT_GAMEPAD_REMOVED: {
                if (gamepadsMap.contains(evt.gdevice.which)) [[likely]]
                    SDL_CloseGamepad(gamepadsMap.take(evt.gdevice.which));
            }
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                if (!eventsEnabled)
                    break;

                switch (evt.gbutton.button) {
                    case SDL_GAMEPAD_BUTTON_SOUTH:
                        emit gamepadButton(u"BTN_A"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_EAST:
                        emit gamepadButton(u"BTN_B"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_WEST:
                        emit gamepadButton(u"BTN_X"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_NORTH:
                        emit gamepadButton(u"BTN_Y"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_START:
                        emit gamepadButton(u"START"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_BACK:
                        emit gamepadButton(u"SELECT"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_GUIDE:
                        emit gamepadButton(u"MENU"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_LEFT_STICK:
                        emit gamepadButton(u"L3"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
                        emit gamepadButton(u"R3"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_DPAD_UP:
                        emit gamepadButton(u"DPAD_UP"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
                        emit gamepadButton(u"DPAD_DOWN"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                        emit gamepadButton(u"DPAD_LEFT"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                        emit gamepadButton(u"DPAD_RIGHT"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
                        emit gamepadButton(u"L1"_s);
                        break;
                    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
                        emit gamepadButton(u"R1"_s);
                        break;
                    default:
                        break;
                }
            }
                break;
            case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                if (!eventsEnabled)
                    break;

                switch (evt.gaxis.axis) {
                    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: {
                        if (evt.gaxis.value >= axisMax)
                            emit gamepadButton(u"L2"_s);
                    }
                        break;
                    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: {
                        if (evt.gaxis.value >= axisMax)
                            emit gamepadButton(u"R2"_s);
                    }
                        break;
                    case SDL_GAMEPAD_AXIS_LEFTX: {
                        const short old = axisState.leftX;

                        axisState.leftX = getAxisState(evt.gaxis.value);

                        if (old == axisState.leftX || axisState.leftX == 0)
                            break;
                        else if (axisState.leftX > 0)
                            emit gamepadButton(u"LSTICK_RIGHT"_s);
                        else
                            emit gamepadButton(u"LSTICK_LEFT"_s);
                    }
                        break;
                    case SDL_GAMEPAD_AXIS_LEFTY: {
                        const short old = axisState.leftY;

                        axisState.leftY = getAxisState(evt.gaxis.value);

                        if (old == axisState.leftY || axisState.leftY == 0)
                            break;
                        else if (axisState.leftY > 0)
                            emit gamepadButton(u"LSTICK_DOWN"_s);
                        else
                            emit gamepadButton(u"LSTICK_UP"_s);
                    }
                        break;
                    case SDL_GAMEPAD_AXIS_RIGHTX: {
                        const short old = axisState.rightX;

                        axisState.rightX = getAxisState(evt.gaxis.value);

                        if (old == axisState.rightX || axisState.rightX == 0)
                            break;
                        else if (axisState.rightX > 0)
                            emit gamepadButton(u"RSTICK_RIGHT"_s);
                        else
                            emit gamepadButton(u"RSTICK_LEFT"_s);
                    }
                        break;
                    case SDL_GAMEPAD_AXIS_RIGHTY: {
                        const short old = axisState.rightY;

                        axisState.rightY = getAxisState(evt.gaxis.value);

                        if (old == axisState.rightY || axisState.rightY == 0)
                            break;
                        else if (axisState.rightY > 0)
                            emit gamepadButton(u"RSTICK_DOWN"_s);
                        else
                            emit gamepadButton(u"RSTICK_UP"_s);
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

    void GamepadWorker::startSDLEventsThread() {
        SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "0");

        if (!SDL_Init(SDL_INIT_GAMEPAD)) {
            emit logSent(u"Failed to init SDL gamepad subsystem"_s);
            emit initFail();
            return;
        }

        while (!QThread::currentThread()->isInterruptionRequested()) {
            SDL_Event evt;

            while (SDL_PollEvent(&evt))
                handleEvent(evt);

            QCoreApplication::processEvents();
            QThread::msleep(12);
        }
    }

    void GamepadWorker::enableEvents(const bool enable) {
        eventsEnabled = enable;
    }
}
