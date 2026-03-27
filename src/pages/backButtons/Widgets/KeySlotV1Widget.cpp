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
#include <QHBoxLayout>
#include <QLabel>

#include "KeySlotV1Widget.h"

namespace OWC {
    KeySlotV1Widget::KeySlotV1Widget(const int slotNum) {
        QHBoxLayout *lyt = new QHBoxLayout();

        keySlotBtn = new QPushButton();
        startTime = new QSpinBox();

        keySlotBtn->setFixedWidth(150);
        startTime->setRange(0, INT16_MAX - 1);

        lyt->setAlignment(Qt::AlignLeft);
        lyt->addWidget(new QLabel(QString::number(slotNum).rightJustified(2, '0')));
        lyt->addWidget(keySlotBtn);
        lyt->addSpacing(6);
        lyt->addWidget(startTime);
        lyt->addWidget(new QLabel("ms"));

        setLayout(lyt);

        QObject::connect(keySlotBtn, &QPushButton::clicked, this, &KeySlotV1Widget::onKeySlotBtnClicked);
    }

    void KeySlotV1Widget::onKeySlotBtnClicked() {

    }
}
