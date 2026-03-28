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
#include "Widgets/DirectionalButtonBlockWidget.h"
#include "Widgets/ShoulderButtonBlockWidget.h"
#include "Widgets/SingleButtonBlockWidget.h"

namespace OWC {
    XinputButtonsPage::XinputButtonsPage(): FaceButtonsPage(CharMapMode::Xinput) {
        QHBoxLayout *row1Lyt = new QHBoxLayout();
        QHBoxLayout *row2Lyt = new QHBoxLayout();
        QHBoxLayout *row3Lyt = new QHBoxLayout();
        DirectionalButtonBlockWidget *dpad;
        DirectionalButtonBlockWidget *actions;
        DirectionalButtonBlockWidget *ls;
        DirectionalButtonBlockWidget *rs;
        ShoulderButtonBlockWidget *shoulderL;
        ShoulderButtonBlockWidget *shoulderR;
        SingleButtonBlockWidget *l3;
        SingleButtonBlockWidget *r3;
        SingleButtonBlockWidget *start;
        SingleButtonBlockWidget *select;
        SingleButtonBlockWidget *menu;

        buttonList = {
            dpad = new DirectionalButtonBlockWidget(Button::X_DPAD_UP, Button::X_DPAD_LEFT, Button::X_DPAD_RIGHT, Button::X_DPAD_DOWN, "X_DPAD_UP", "X_DPAD_LEFT", "X_DPAD_RIGHT", "X_DPAD_DOWN", "dpad", 75),
            shoulderL = new ShoulderButtonBlockWidget(Button::X_L2, Button::X_L1, "X_L2", "X_L1", "l2", "l1", 55, 55, 70, 30),
            shoulderR = new ShoulderButtonBlockWidget(Button::X_R2, Button::X_R1, "X_R2", "X_R1", "r2", "r1", 55, 55, 70, 30),
            actions = new DirectionalButtonBlockWidget(Button::X_Y, Button::X_X, Button::X_B, Button::X_A, "X_Y", "X_X", "X_B", "X_A", "face", 80),
            ls = new DirectionalButtonBlockWidget(Button::X_LANALOG_UP, Button::X_LANALOG_LEFT, Button::X_LANALOG_RIGHT, Button::X_LANALOG_DOWN, "X_L_ANALOG_UP", "X_L_ANALOG_LEFT", "X_L_ANALOG_RIGHT", "X_L_ANALOG_DOWN", "ls", 62),
            l3 = new SingleButtonBlockWidget(Button::X_L3, "X_L3", "l3", 55, 55),
            r3 = new SingleButtonBlockWidget(Button::X_R3, "X_R3", "r3", 55, 55),
            start = new SingleButtonBlockWidget(Button::X_START, "X_START", "start", 70, 30),
            select = new SingleButtonBlockWidget(Button::X_SELECT, "X_SELECT", "select", 70, 30),
            menu = new SingleButtonBlockWidget(Button::X_MENU, "X_MENU", "menu", 70, 30),
            rs = new DirectionalButtonBlockWidget(Button::X_RANALOG_UP, Button::X_RANALOG_LEFT, Button::X_RANALOG_RIGHT, Button::X_RANALOG_DOWN, "X_R_ANALOG_UP", "X_R_ANALOG_LEFT", "X_R_ANALOG_RIGHT", "X_R_ANALOG_DOWN", "rs", 62),
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

        row3Lyt->addWidget(rs);
        row3Lyt->addStretch();

        controlsLyt->addLayout(row1Lyt);
        controlsLyt->addSpacing(12);
        controlsLyt->addLayout(row2Lyt);
        controlsLyt->addSpacing(12);
        controlsLyt->addLayout(row3Lyt);
        controlsLyt->addStretch();

        QObject::connect(dpad, &DirectionalButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(dpad, &DirectionalButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(shoulderL, &ShoulderButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(shoulderL, &ShoulderButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(shoulderR, &ShoulderButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(shoulderR, &ShoulderButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(actions, &DirectionalButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(actions, &DirectionalButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(ls, &DirectionalButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(ls, &DirectionalButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(l3, &SingleButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(l3, &SingleButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(r3, &SingleButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(r3, &SingleButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(start, &SingleButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(start, &SingleButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(select, &SingleButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(select, &SingleButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(menu, &SingleButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(menu, &SingleButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
        QObject::connect(rs, &DirectionalButtonBlockWidget::logSent, this, &XinputButtonsPage::onLogSent);
        QObject::connect(rs, &DirectionalButtonBlockWidget::pendingEditBtn, this, &XinputButtonsPage::onkeyButtonPressed);
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
