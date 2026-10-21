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
#include "HomePage.h"
#include "../include/GPDProducts.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    HomePage::HomePage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *selectionLyt = new QHBoxLayout();
        QVBoxLayout *mapFaceBtnLyt = new QVBoxLayout();
        QVBoxLayout *mapBackBtnLyt = new QVBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QPushButton *logsBtn = new QPushButton(u"Logs"_s);

        headLyt = new QHBoxLayout();
        mappingMode = new QComboBox();
        faceButtonsMapBtn = new QPushButton(u"Face buttons"_s);
        backButtonsMapBtn = new QPushButton(u"Back buttons"_s);
        ymlBrowserBtn = new QPushButton(u"Profiles"_s);
        settingsBtn = new QPushButton(u"Settings"_s);
        applyBtn = new QPushButton(u"Apply"_s);
        exportYamlBtn = new QPushButton(u"Export mapping"_s);
        importYamlBtn = new QPushButton(u"Import mapping"_s);
        frontPic = new QLabel();
        backPic = new QLabel();

        mappingMode->addItem(u"Keyboard&Mouse"_s);
        mappingMode->setToolTip(u"Face buttons mapping mode"_s);
        faceButtonsMapBtn->setMinimumHeight(40);
        backButtonsMapBtn->setMinimumHeight(40);
        frontPic->setAlignment(Qt::AlignCenter);
        frontPic->setPixmap(QPixmap(u":/unkd"_s));
        backPic->setAlignment(Qt::AlignCenter);
        backPic->setPixmap(QPixmap(u":/unkd"_s));
        faceButtonsMapBtn->setFixedWidth(280);
        backButtonsMapBtn->setFixedWidth(280);
        enableButtons(false);

        mapFaceBtnLyt->setAlignment(Qt::AlignCenter);
        mapFaceBtnLyt->addWidget(frontPic);
        mapFaceBtnLyt->addWidget(faceButtonsMapBtn);
        mapBackBtnLyt->setAlignment(Qt::AlignCenter);
        mapBackBtnLyt->addWidget(backPic);
        mapBackBtnLyt->addWidget(backButtonsMapBtn);

        headLyt->addStretch();
        headLyt->addWidget(mappingMode);

        bottomLyt->addWidget(logsBtn);
        bottomLyt->addWidget(ymlBrowserBtn);
        bottomLyt->addWidget(settingsBtn);
        bottomLyt->addStretch();
        bottomLyt->addWidget(applyBtn);
        bottomLyt->addWidget(exportYamlBtn);
        bottomLyt->addWidget(importYamlBtn);

        selectionLyt->addLayout(mapBackBtnLyt);
        selectionLyt->addLayout(mapFaceBtnLyt);

        lyt->addLayout(headLyt);
        lyt->addStretch();
        lyt->addLayout(selectionLyt);
        lyt->addStretch();
        lyt->addLayout(bottomLyt);

        setLayout(lyt);

        QObject::connect(faceButtonsMapBtn, &QPushButton::clicked, this, &HomePage::onFaceButtonsMapClicked);
        QObject::connect(backButtonsMapBtn, &QPushButton::clicked, this, &HomePage::backButtonsMap);
        QObject::connect(logsBtn, &QPushButton::clicked, this, &HomePage::showLogs);
        QObject::connect(ymlBrowserBtn, &QPushButton::clicked, this, &HomePage::yamlBrowser);
        QObject::connect(settingsBtn, &QPushButton::clicked, this, &HomePage::settingsPage);
        QObject::connect(applyBtn, &QPushButton::clicked, this, &HomePage::applyChanges);
        QObject::connect(exportYamlBtn, &QPushButton::clicked, this, &HomePage::exportYaml);
        QObject::connect(importYamlBtn, &QPushButton::clicked, this, &HomePage::importYaml);
    }

    void HomePage::init(const QString &product) const {
        if (product == win5 || product == mini25 || product == max3 || product == miniUltra)
            mappingMode->addItem(u"Xinput"_s);

        if (product == win4) {
            frontPic->setPixmap(QPixmap(u":/win4f"_s).scaled(254, 107, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(u":/win4b"_s).scaled(254, 107, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        } else if (product == mini24 || product == mini25 || product == mini25L) {
            frontPic->setPixmap(QPixmap(u":/minif"_s).scaled(160, 155, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(u":/minib"_s).scaled(200, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        } else if (product == max2_22 || product == max2_25) {
            frontPic->setPixmap(QPixmap(u":/max2f"_s).scaled(260, 155, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(u":/max2b"_s).scaled(230, 155, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        } else if (product == win5) {
            frontPic->setPixmap(QPixmap(u":/win5f"_s));
            backPic->setPixmap(QPixmap(u":/win5b"_s));

        } else if (product == win3) {
            frontPic->setPixmap(QPixmap(u":/win3f"_s).scaled(235, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(u":/win3b"_s).scaled(235, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        } else if (product == max3) {
            frontPic->setPixmap(QPixmap(u":/max3f"_s).scaled(220, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(u":/max3b"_s).scaled(300, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        } else if (product == miniUltra) {
            frontPic->setPixmap(QPixmap(u":/miniuf"_s).scaled(280, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(u":/miniub"_s).scaled(280, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }

    void HomePage::setEmulationMode(const EmulationMode mode) const {
        QLabel *emuMode = new QLabel();
        QFont font = emuMode->font();

        switch (mode) {
            case EmulationMode::KeyboardMouse:
                emuMode->setText(u"Keyboard&Mouse"_s);
                break;
            case EmulationMode::Xinput:
                emuMode->setText(u"Xinput"_s);
                break;
            case EmulationMode::KeyboardSpecial:
                emuMode->setText(u"Keyboard Special Mode"_s);
                break;
            default:
                emuMode->setText(u"Unknown"_s);
                break;
        }

        font.setItalic(true);
        emuMode->setFont(font);

        headLyt->insertWidget(0, new QLabel(u"Controller mode:"_s));
        headLyt->insertWidget(1, emuMode);
    }

    void HomePage::enableButtons(const bool enable) const {
        faceButtonsMapBtn->setEnabled(enable);
        backButtonsMapBtn->setEnabled(enable);
        settingsBtn->setEnabled(enable);
        applyBtn->setEnabled(enable);
        exportYamlBtn->setEnabled(enable);
        importYamlBtn->setEnabled(enable);
    }

    void HomePage::onFaceButtonsMapClicked() {
        if (mappingMode->currentIndex() == 0)
            emit keyboardMouseMap();
        else
            emit xinputMap();
    }
}
