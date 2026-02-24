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
#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QScroller>

#include "BackButtonsPage.h"
#include "../include/ASCIIHIDMap.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"

namespace OWC {
    BackButtonsPage::BackButtonsPage(const QString &helpLbl, const CharMapMode charMapMode) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonsLyt = new QHBoxLayout();
        QLabel *helpTx = new QLabel(helpLbl);
        QScrollArea *scrollArea = new QScrollArea();
        QFont helpFont = helpTx->font();

        backBtnLyt = new QHBoxLayout();
        backBtn = new QPushButton("Home");
        resetBtn = new QPushButton("Reset");
        charMapBtn = new QPushButton("Char Map");
        charMap = new CharMapWidget(charMapMode);

        charMap->setVisible(false);
        helpFont.setItalic(true);
        helpTx->setFont(helpFont);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

        scrollArea->widget()->setLayout(backBtnLyt);

        buttonsLyt->addWidget(helpTx);
        buttonsLyt->addStretch();
        buttonsLyt->addWidget(charMapBtn);
        buttonsLyt->addWidget(backBtn);
        buttonsLyt->addWidget(resetBtn);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(charMap);
        lyt->addSpacing(12);
        lyt->addWidget(scrollArea);
        lyt->addLayout(buttonsLyt);

        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &BackButtonsPage::onBackBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &BackButtonsPage::onResetBtnClicked);
        QObject::connect(charMapBtn, &QPushButton::clicked, this, &BackButtonsPage::onCharMapBtnClicked);
        QObject::connect(charMap, &CharMapWidget::keyPressed, this, &BackButtonsPage::onCharMapKeyPressed);
    }

    void BackButtonsPage::keyPressEvent(QKeyEvent *event) {
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

    void BackButtonsPage::setGamepadKey(const QString &key) const {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }

    void BackButtonsPage::onBackBtnClicked() {
        emit backToHome();
    }

    void BackButtonsPage::onResetBtnClicked() {
        emit resetBackButtons();
    }

    void BackButtonsPage::onCharMapBtnClicked() const {
        charMap->setVisible(!charMap->isVisible());
    }

    void BackButtonsPage::onkeyButtonPressed() const {
        QPushButton *btn = qobject_cast<QPushButton *>(QObject::sender());

        if (pendingBtn != nullptr) {
            pendingBtn->setText(oldPendingBtnText);

            if (btn != nullptr && pendingBtn == btn) { // cancel edit
                pendingBtn = nullptr;
                return;
            }
        }

        pendingBtn = btn;
        oldPendingBtnText = pendingBtn->text();

        pendingBtn->setText("...");
    }

    void BackButtonsPage::onCharMapKeyPressed(const QString &key) const {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }
}
