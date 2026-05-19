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

#include "Widgets/FlowLayout.h"
#include "../include/CharMapMode.h"

namespace OWC {
    class CharMapPage final: public QWidget {
        Q_OBJECT

    private:
        QWidget *kbMouseSection = nullptr;
        QWidget *gamepadSection = nullptr;

        [[nodiscard]] QPushButton *makeKeyBtn(int code) const;
        [[nodiscard]] FlowLayout *makeAlphabetSection() const;
        [[nodiscard]] FlowLayout *makeNumbersSection() const;
        [[nodiscard]] FlowLayout *makeFKeysSection() const;
        [[nodiscard]] FlowLayout *makeKBKeysSection() const;
        [[nodiscard]] FlowLayout *makeMouseSection() const;
        [[nodiscard]] FlowLayout *makeSpecialSection() const;
        [[nodiscard]] FlowLayout *makeGamepadSection() const;

    public:
        explicit CharMapPage(bool hasGamepadKeys);

        void setMode(CharMapMode mode) const;

    private slots:
        void onKeyClicked();
        void onBackBtnClicked();

    signals:
        void keyPressed(const QString &key);
        void hideCharMap();
    };
}
