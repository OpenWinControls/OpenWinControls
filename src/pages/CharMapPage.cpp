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
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScroller>
#include <QLabel>
#include <QPushButton>

#include "CharMapPage.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"
#include "../extern/libOpenWinControls/src/include/XinputUsageIDMap.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    CharMapPage::CharMapPage(const bool hasGamepadKeys) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *unsetLyt = new QHBoxLayout();
        QVBoxLayout *kbMouseSectionLyt = new QVBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QVBoxLayout *scrollWidgLyt = new QVBoxLayout();
        QScrollArea *scrollArea = new QScrollArea();
        QPushButton *backBtn = new QPushButton(u"Back"_s);
        QLabel *alphLbl = new QLabel(u"Alphabet"_s);
        QLabel *numLbl = new QLabel(u"Number"_s);
        QLabel *fLbl = new QLabel(u"Function"_s);
        QLabel *mouseLbl = new QLabel(u"Mouse"_s);
        QLabel *kbLbl = new QLabel(u"Keyboard"_s);
        QLabel *specialLbl = new QLabel(u"Special"_s);
        QFont font;

        kbMouseSection = new QWidget();
        font = alphLbl->font();

        font.setBold(true);
        font.setPointSize(14);
        alphLbl->setFont(font);
        alphLbl->setAlignment(Qt::AlignCenter);
        numLbl->setFont(font);
        numLbl->setAlignment(Qt::AlignCenter);
        fLbl->setFont(font);
        fLbl->setAlignment(Qt::AlignCenter);
        mouseLbl->setFont(font);
        mouseLbl->setAlignment(Qt::AlignCenter);
        kbLbl->setFont(font);
        kbLbl->setAlignment(Qt::AlignCenter);
        specialLbl->setFont(font);
        specialLbl->setAlignment(Qt::AlignCenter);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

        unsetLyt->addWidget(makeKeyBtn(0));
        unsetLyt->addStretch();
        kbMouseSectionLyt->setContentsMargins(0, 0, 0, 0);
        kbMouseSectionLyt->addWidget(alphLbl);
        kbMouseSectionLyt->addSpacing(4);
        kbMouseSectionLyt->addLayout(makeAlphabetSection());
        kbMouseSectionLyt->addWidget(numLbl);
        kbMouseSectionLyt->addSpacing(4);
        kbMouseSectionLyt->addLayout(makeNumbersSection());
        kbMouseSectionLyt->addWidget(fLbl);
        kbMouseSectionLyt->addSpacing(4);
        kbMouseSectionLyt->addLayout(makeFKeysSection());
        kbMouseSectionLyt->addWidget(kbLbl);
        kbMouseSectionLyt->addSpacing(4);
        kbMouseSectionLyt->addLayout(makeKBKeysSection());
        kbMouseSectionLyt->addWidget(mouseLbl);
        kbMouseSectionLyt->addSpacing(4);
        kbMouseSectionLyt->addLayout(makeMouseSection());
        kbMouseSectionLyt->addWidget(specialLbl);
        kbMouseSectionLyt->addSpacing(4);
        kbMouseSectionLyt->addLayout(makeSpecialSection());
        bottomLyt->addStretch();
        bottomLyt->addWidget(backBtn);
        scrollWidgLyt->addLayout(unsetLyt);
        scrollWidgLyt->addWidget(kbMouseSection);
        scrollWidgLyt->addStretch();
        lyt->addWidget(scrollArea);
        lyt->addLayout(bottomLyt);

        if (hasGamepadKeys) {
            QVBoxLayout *gamepadSectionLyt = new QVBoxLayout();
            QLabel *gamepadLbl = new QLabel(u"Gamepad"_s);

            gamepadSection = new QWidget();

            gamepadLbl->setFont(font);
            gamepadLbl->setAlignment(Qt::AlignCenter);
            gamepadSectionLyt->setContentsMargins(0, 0, 0, 0);
            gamepadSectionLyt->addWidget(gamepadLbl);
            gamepadSectionLyt->addSpacing(4);
            gamepadSectionLyt->addLayout(makeGamepadSection());

            scrollWidgLyt->insertWidget(2, gamepadSection);
            gamepadSection->setLayout(gamepadSectionLyt);
        }

        kbMouseSection->setLayout(kbMouseSectionLyt);
        scrollArea->widget()->setLayout(scrollWidgLyt);
        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &CharMapPage::onBackBtnClicked);
    }

    QPushButton *CharMapPage::makeKeyBtn(const int code) const {
        const std::string key = code >= 0x8000 ? XinputUsageIDMap.at(code) : HIDUsageIDMap.at(code);
        QPushButton *kbtn = new QPushButton(QString::fromStdString(key));

        kbtn->setFlat(true);
        kbtn->setMinimumHeight(60);
        kbtn->setToolTip(QString("Usage ID: 0x%1").arg(QString::number(code, 16)));

        QObject::connect(kbtn, &QPushButton::clicked, this, &CharMapPage::onKeyClicked);
        return kbtn;
    }

    FlowLayout *CharMapPage::makeAlphabetSection() const {
        FlowLayout *lyt = new FlowLayout();

        for (int i=0x4; i<=0x1d; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    FlowLayout *CharMapPage::makeNumbersSection() const {
        FlowLayout *lyt = new FlowLayout();

        for (int i=0x1e; i<=0x27; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    FlowLayout *CharMapPage::makeFKeysSection() const {
        FlowLayout *lyt = new FlowLayout();

        for (int i=0x3a; i<=0x45; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0x68; i<=0x73; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    FlowLayout *CharMapPage::makeKBKeysSection() const {
        FlowLayout *lyt = new FlowLayout();

        for (int i=0x28; i<=0x39; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0x46; i<=0x52; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0x74; i<=0x81; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0xe0; i<=0xe7; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    FlowLayout *CharMapPage::makeMouseSection() const {
        FlowLayout *lyt = new FlowLayout();

        for (int i=0xe8; i<=0xed; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    FlowLayout *CharMapPage::makeSpecialSection() const {
        FlowLayout *lyt = new FlowLayout();

        lyt->addWidget(makeKeyBtn(0x85));

        for (int i=0x53; i<=0x67; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0x87; i<=0x8c; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0x90; i<=0x94; ++i)
            lyt->addWidget(makeKeyBtn(i));

        for (int i=0xb6; i<=0xb7; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    FlowLayout *CharMapPage::makeGamepadSection() const {
        FlowLayout *lyt = new FlowLayout();

        for (int i=0x8000; i<=0x8018; ++i)
            lyt->addWidget(makeKeyBtn(i));

        return lyt;
    }

    void CharMapPage::setMode(const CharMapMode mode) const {
        kbMouseSection->setVisible(mode == CharMapMode::Keyboard || mode == CharMapMode::Mixed);

        if (gamepadSection != nullptr)
            gamepadSection->setVisible(mode == CharMapMode::Xinput || mode == CharMapMode::Mixed);
    }

    void CharMapPage::onKeyClicked() {
        const QPushButton *btn = qobject_cast<QPushButton *>(QObject::sender());

        if (btn != nullptr)
            emit keyPressed(btn->text());
    }

    void CharMapPage::onBackBtnClicked() {
        emit hideCharMap();
    }
}
