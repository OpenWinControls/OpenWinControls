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
#include "KeyboardMouseButtonsPage.h"
#include "../include/ASCIIHIDMap.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"

namespace OWC {
    KeyboardMouseButtonsPage::KeyboardMouseButtonsPage(): FaceButtonsPage(CharMapMode::Keyboard) {
        btnMap = {
            {"A", {aBtn, Button::KBD_A}},
            {"B", {bBtn, Button::KBD_B}},
            {"X", {xBtn, Button::KBD_X}},
            {"Y", {yBtn, Button::KBD_Y}},
            {"DPAD_UP", {dpadUpBtn, Button::KBD_DPAD_UP}},
            {"DPAD_DOWN", {dpadDownBtn, Button::KBD_DPAD_DOWN}},
            {"DPAD_LEFT", {dpadLeftBtn, Button::KBD_DPAD_LEFT}},
            {"DPAD_RIGHT", {dpadRight, Button::KBD_DPAD_RIGHT}},
            {"L_ANALOG_UP", {lAnalogUpBtn, Button::KBD_LANALOG_UP}},
            {"L_ANALOG_DOWN", {lAnalogDownBtn, Button::KBD_LANALOG_DOWN}},
            {"L_ANALOG_LEFT", {lAnalogLeftBtn, Button::KBD_LANALOG_LEFT}},
            {"L_ANALOG_RIGHT", {lAnalogRightBtn, Button::KBD_LANALOG_RIGHT}},
            {"L1", {l1Btn, Button::KBD_L1}},
            {"L2", {l2Btn, Button::KBD_L2}},
            {"L3", {l3Btn, Button::KBD_L3}},
            {"R1", {r1Btn, Button::KBD_R1}},
            {"R2", {r2Btn, Button::KBD_R2}},
            {"R3", {r3Btn, Button::KBD_R3}},
            {"START", {startBtn, Button::KBD_START}},
            {"SELECT", {selectBtn, Button::KBD_SELECT}},
            {"MENU", {menuBtn, Button::KBD_MENU}}
        };

        controlsLyt->addStretch();
    }

    void KeyboardMouseButtonsPage::keyPressEvent(QKeyEvent *event) {
        if (pendingBtn == nullptr)
            return;

        const Qt::Key kc = static_cast<Qt::Key>(event->key());

        if (!ASCIIHIDMap.contains(kc)) {
            emit logSent(QString("unknown scan code: %1").arg(kc));
            return;
        }

        if (!HIDUsageIDMap.contains(ASCIIHIDMap[kc])) {
            emit logSent(QString("unknown hid code: %1").arg(kc));
            return;
        }

        pendingBtn->setText(QString::fromStdString(HIDUsageIDMap.at(ASCIIHIDMap[kc])));
        pendingBtn = nullptr;
    }

    void KeyboardMouseButtonsPage::onResetBtnClicked() {
        emit resetKeyboardMouseButtons();
    }
}
