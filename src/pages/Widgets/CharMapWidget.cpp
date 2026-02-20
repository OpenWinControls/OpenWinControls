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
#include <QScrollArea>
#include <QScroller>

#include "CharMapWidget.h"
#include "../../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"
#include "../../extern/libOpenWinControls/src/include/XinputUsageIDMap.h"

namespace OWC {
    CharMapWidget::CharMapWidget(const CharMapMode mode) {
        QHBoxLayout *lyt = new QHBoxLayout();
        QHBoxLayout *scrollWidgLyt = new QHBoxLayout();
        QScrollArea *scrollArea = new QScrollArea();

        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

        if (mode == CharMapMode::Keyboard) {
            addKeys(HIDUsageIDMap, scrollWidgLyt);

        } else if (mode == CharMapMode::Xinput) {
            addKeys(XinputUsageIDMap, scrollWidgLyt);

        } else {
            std::map<int, std::string> keys = HIDUsageIDMap;

            keys.insert(XinputUsageIDMap.begin(), XinputUsageIDMap.end());
            addKeys(keys, scrollWidgLyt);
        }

        scrollArea->widget()->setLayout(scrollWidgLyt);
        scrollWidgLyt->addStretch();
        lyt->addWidget(scrollArea);

        setLayout(lyt);
        setFixedHeight(130);
    }

    void CharMapWidget::addKeys(const std::map<int, std::string> &keyMap, QHBoxLayout *lyt) {
        for (const auto &[code, key]: keyMap) {
            QPushButton *charBtn = new QPushButton(QString::fromStdString(key));

            charBtn->setFlat(true);
            charBtn->setMinimumHeight(60);
            charBtn->setToolTip(QString("Usage ID: 0x%1").arg(QString::number(code, 16)));

            lyt->addWidget(charBtn);
            charBtnList.append(charBtn);

            QObject::connect(charBtn, &QPushButton::clicked, this, &CharMapWidget::onKeyClicked);
        }
    }

    void CharMapWidget::onKeyClicked() {
        const QPushButton *btn = qobject_cast<QPushButton *>(QObject::sender());

        if (btn != nullptr)
            emit keyPressed(btn->text());
    }
}
