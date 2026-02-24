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
#include "XinputButtonsPage.h"

namespace OWC {
    XinputButtonsPage::XinputButtonsPage(): FaceButtonsPage(CharMapMode::Xinput) {
        QHBoxLayout *row3Lyt = new QHBoxLayout();

        rAnalogUpBtn = new QPushButton();
        rAnalogDownBtn = new QPushButton();
        rAnalogLeftBtn = new QPushButton();
        rAnalogRightBtn = new QPushButton();

        btnMap = {
            {"X_A", {aBtn, Button::X_A}},
            {"X_B", {bBtn, Button::X_B}},
            {"X_X", {xBtn, Button::X_X}},
            {"X_Y", {yBtn, Button::X_Y}},
            {"X_DPAD_UP", {dpadUpBtn, Button::X_DPAD_UP}},
            {"X_DPAD_DOWN", {dpadDownBtn, Button::X_DPAD_DOWN}},
            {"X_DPAD_LEFT", {dpadLeftBtn, Button::X_DPAD_LEFT}},
            {"X_DPAD_RIGHT", {dpadRight, Button::X_DPAD_RIGHT}},
            {"X_L_ANALOG_UP", {lAnalogUpBtn, Button::X_LANALOG_UP}},
            {"X_L_ANALOG_DOWN", {lAnalogDownBtn, Button::X_LANALOG_DOWN}},
            {"X_L_ANALOG_LEFT", {lAnalogLeftBtn, Button::X_LANALOG_LEFT}},
            {"X_L_ANALOG_RIGHT", {lAnalogRightBtn, Button::X_LANALOG_RIGHT}},
            {"X_R_ANALOG_UP", {rAnalogUpBtn, Button::X_RANALOG_UP}},
            {"X_R_ANALOG_DOWN", {rAnalogDownBtn, Button::X_RANALOG_DOWN}},
            {"X_R_ANALOG_LEFT", {rAnalogLeftBtn, Button::X_RANALOG_LEFT}},
            {"X_R_ANALOG_RIGHT", {rAnalogRightBtn, Button::X_RANALOG_RIGHT}},
            {"X_L1", {l1Btn, Button::X_L1}},
            {"X_L2", {l2Btn, Button::X_L2}},
            {"X_L3", {l3Btn, Button::X_L3}},
            {"X_R1", {r1Btn, Button::X_R1}},
            {"X_R2", {r2Btn, Button::X_R2}},
            {"X_R3", {r3Btn, Button::X_R3}},
            {"X_START", {startBtn, Button::X_START}},
            {"X_SELECT", {selectBtn, Button::X_SELECT}},
            {"X_MENU", {menuBtn, Button::X_MENU}}
        };

        row3Lyt->addLayout(makeDirectionalBlock(rAnalogUpBtn, rAnalogLeftBtn, rAnalogRightBtn, rAnalogDownBtn, "rs", 62));
        row3Lyt->addStretch();

        controlsLyt->addSpacing(12);
        controlsLyt->addLayout(row3Lyt);
    }

    void XinputButtonsPage::setGamepadKey(const QString &key) const {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }

    void XinputButtonsPage::onResetBtnClicked() {
        emit resetXinputButtons();
    }
}
