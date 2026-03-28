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
#include "../../include/ASCIIHIDMap.h"
#include "../../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"
#include "Widgets/DirectionalButtonBlockWidget.h"
#include "Widgets/ShoulderButtonBlockWidget.h"
#include "Widgets/SingleButtonBlockWidget.h"

namespace OWC {
    KeyboardMouseButtonsPage::KeyboardMouseButtonsPage(): FaceButtonsPage(CharMapMode::Keyboard) {
        QHBoxLayout *row1Lyt = new QHBoxLayout();
        QHBoxLayout *row2Lyt = new QHBoxLayout();
        DirectionalButtonBlockWidget *dpad;
        DirectionalButtonBlockWidget *actions;
        DirectionalButtonBlockWidget *ls;
        ShoulderButtonBlockWidget *shoulderL;
        ShoulderButtonBlockWidget *shoulderR;
        SingleButtonBlockWidget *l3;
        SingleButtonBlockWidget *r3;
        SingleButtonBlockWidget *start;
        SingleButtonBlockWidget *select;
        SingleButtonBlockWidget *menu;

        buttonList = {
            dpad = new DirectionalButtonBlockWidget(Button::KBD_DPAD_UP, Button::KBD_DPAD_LEFT, Button::KBD_DPAD_RIGHT, Button::KBD_DPAD_DOWN, "DPAD_UP", "DPAD_LEFT", "DPAD_RIGHT", "DPAD_DOWN", "dpad", 75),
            shoulderL = new ShoulderButtonBlockWidget(Button::KBD_L2, Button::KBD_L1, "L2", "L1", "l2", "l1", 55, 55, 70, 30),
            shoulderR = new ShoulderButtonBlockWidget(Button::KBD_R2, Button::KBD_R1, "R2", "R1", "r2", "r1", 55, 55, 70, 30),
            actions = new DirectionalButtonBlockWidget(Button::KBD_Y, Button::KBD_X, Button::KBD_B, Button::KBD_A, "Y", "X", "B", "A", "face", 80),
            ls = new DirectionalButtonBlockWidget(Button::KBD_LANALOG_UP, Button::KBD_LANALOG_LEFT, Button::KBD_LANALOG_RIGHT, Button::KBD_LANALOG_DOWN, "L_ANALOG_UP", "L_ANALOG_LEFT", "L_ANALOG_RIGHT", "L_ANALOG_DOWN", "ls", 62),
            l3 = new SingleButtonBlockWidget(Button::KBD_L3, "L3", "l3", 55, 55),
            r3 = new SingleButtonBlockWidget(Button::KBD_R3, "R3", "r3", 55, 55),
            start = new SingleButtonBlockWidget(Button::KBD_START, "START", "start", 70, 30),
            select = new SingleButtonBlockWidget(Button::KBD_SELECT, "SELECT", "select", 70, 30),
            menu = new SingleButtonBlockWidget(Button::KBD_MENU, "MENU", "menu", 70, 30)
        };

        row1Lyt->addWidget(dpad);
        row1Lyt->addStretch();
        row1Lyt->addWidget(shoulderL);
        row1Lyt->addWidget(shoulderR);
        row1Lyt->addStretch();
        row1Lyt->addWidget(actions);

        row2Lyt->addWidget(ls);
        row2Lyt->addStretch();
        row2Lyt->addWidget(l3);
        row2Lyt->addWidget(r3);
        row2Lyt->addStretch();
        row2Lyt->addWidget(start);
        row2Lyt->addWidget(select);
        row2Lyt->addWidget(menu);

        controlsLyt->addLayout(row1Lyt);
        controlsLyt->addSpacing(12);
        controlsLyt->addLayout(row2Lyt);
        controlsLyt->addStretch();

        QObject::connect(dpad, &DirectionalButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(dpad, &DirectionalButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(shoulderL, &ShoulderButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(shoulderL, &ShoulderButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(shoulderR, &ShoulderButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(shoulderR, &ShoulderButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(actions, &DirectionalButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(actions, &DirectionalButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(ls, &DirectionalButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(ls, &DirectionalButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(l3, &SingleButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(l3, &SingleButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(r3, &SingleButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(r3, &SingleButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(start, &SingleButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(start, &SingleButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(select, &SingleButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(select, &SingleButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
        QObject::connect(menu, &SingleButtonBlockWidget::logSent, this, &KeyboardMouseButtonsPage::onLogSent);
        QObject::connect(menu, &SingleButtonBlockWidget::pendingEditBtn, this, &KeyboardMouseButtonsPage::onkeyButtonPressed);
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
