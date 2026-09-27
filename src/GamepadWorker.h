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
#pragma once

#include <QObject>
#include <QHash>

#include "extern/SDL/include/SDL3/SDL_gamepad.h"
#include "extern/SDL/include/SDL3/SDL_events.h"

namespace OWC {
    class GamepadWorker final: public QObject {
        Q_OBJECT

    private:
        struct AxisState final {
            short leftX = 0;
            short leftY = 0;
            short rightX = 0;
            short rightY = 0;
        };

        static constexpr int axisMax = (SDL_JOYSTICK_AXIS_MAX * 50) / 100;
        QHash<SDL_JoystickID, SDL_Gamepad *> gamepadsMap;
        bool eventsEnabled = false;
        AxisState axisState;

        [[nodiscard]] short getAxisState(Sint16 axisValue);
        void handleEvent(const SDL_Event &evt);

    public:
        ~GamepadWorker() override;

    public slots:
        void startSDLEventsThread();
        void enableEvents(bool enable);

    signals:
        void logSent(const QString &msg);
        void initFail();
        void gamepadButton(const QString &key);
    };
}
