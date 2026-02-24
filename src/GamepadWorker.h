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

namespace OWC {
    class GamepadWorker final: public QObject {
        Q_OBJECT

    private:
        struct AxisState final {
            int leftX = 0;
            int leftY = 0;
            int rightX = 0;
            int rightY = 0;
        };

        static constexpr int deadzone = 8000;
        QHash<SDL_JoystickID, SDL_Gamepad *> sdlGamepadMap;
        AxisState axisState;
        bool enabled = false;

        [[nodiscard]] bool isDeadzone(int axis) const;

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
