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

#include "HomePage.h"
#include "../include/GPDProducts.h"

namespace OWC {
    HomePage::HomePage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *selectionLyt = new QHBoxLayout();
        QVBoxLayout *mapFaceBtnLyt = new QVBoxLayout();
        QVBoxLayout *mapBackBtnLyt = new QVBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QLabel *frontLbl = new QLabel("Face buttons");
        QLabel *backLbl = new QLabel("Back buttons");
        QFont lblFont = frontLbl->font();

        faceButtonsMapBtn = new QPushButton("Customize");
        backButtonsMapBtn = new QPushButton("Customize");
        showLogsBtn = new QPushButton("Logs");
        settingsBtn = new QPushButton("Settings");
        applyBtn = new QPushButton("Apply");
        exportYamlBtn = new QPushButton("Export mapping");
        importYamlBtn = new QPushButton("Import mapping");
        frontPic = new QLabel();
        backPic = new QLabel();

        faceButtonsMapBtn->setMinimumHeight(40);
        backButtonsMapBtn->setMinimumHeight(40);
        lblFont.setBold(true);
        lblFont.setPointSize(14);
        frontLbl->setFont(lblFont);
        frontLbl->setAlignment(Qt::AlignCenter);
        backLbl->setFont(lblFont);
        backLbl->setAlignment(Qt::AlignCenter);
        frontPic->setAlignment(Qt::AlignCenter);
        frontPic->setPixmap(QPixmap(":/unkd"));
        backPic->setAlignment(Qt::AlignCenter);
        backPic->setPixmap(QPixmap(":/unkd"));
        faceButtonsMapBtn->setFixedWidth(280);
        backButtonsMapBtn->setFixedWidth(280);
        enableButtons(false);

        mapFaceBtnLyt->setAlignment(Qt::AlignCenter);
        mapFaceBtnLyt->addStretch();
        mapFaceBtnLyt->addWidget(frontLbl);
        mapFaceBtnLyt->addWidget(frontPic);
        mapFaceBtnLyt->addWidget(faceButtonsMapBtn);
        mapFaceBtnLyt->addStretch();
        mapBackBtnLyt->setAlignment(Qt::AlignCenter);
        mapBackBtnLyt->addStretch();
        mapBackBtnLyt->addWidget(backLbl);
        mapBackBtnLyt->addWidget(backPic);
        mapBackBtnLyt->addWidget(backButtonsMapBtn);
        mapBackBtnLyt->addStretch();

        bottomLyt->addWidget(showLogsBtn);
        bottomLyt->addStretch();
        bottomLyt->addWidget(settingsBtn);
        bottomLyt->addWidget(applyBtn);
        bottomLyt->addWidget(exportYamlBtn);
        bottomLyt->addWidget(importYamlBtn);

        selectionLyt->addLayout(mapBackBtnLyt);
        selectionLyt->addLayout(mapFaceBtnLyt);

        lyt->addStretch();
        lyt->addLayout(selectionLyt);
        lyt->addStretch();
        lyt->addLayout(bottomLyt);

        setLayout(lyt);

        QObject::connect(faceButtonsMapBtn, &QPushButton::clicked, this, &HomePage::onFaceButtonsMapClicked);
        QObject::connect(backButtonsMapBtn, &QPushButton::clicked, this, &HomePage::onBackButtonsMapClicked);
        QObject::connect(showLogsBtn, &QPushButton::clicked, this, &HomePage::onShowLogsBtnClicked);
        QObject::connect(settingsBtn, &QPushButton::clicked, this, &HomePage::onSettingsClicked);
        QObject::connect(applyBtn, &QPushButton::clicked, this, &HomePage::onApplyBtnClicked);
        QObject::connect(exportYamlBtn, &QPushButton::clicked, this, &HomePage::onExportYamlBtnClicked);
        QObject::connect(importYamlBtn, &QPushButton::clicked, this, &HomePage::onImportYamlBtnClicked);
    }

    void HomePage::setDevice(const QString &product) const {
        bool found = false;

        if (product == win4) {
            frontPic->setPixmap(QPixmap(":/win4f").scaled(254, 107, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(":/win4b").scaled(254, 107, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            found = true;

        } else if (product == mini23 || product == mini24 || product == mini25) {
            frontPic->setPixmap(QPixmap(":/minif").scaled(160, 155, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(":/minib").scaled(200, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            found = true;

        } else if (product == max2_22 || product == max2_25) {
            frontPic->setPixmap(QPixmap(":/max2f").scaled(260, 155, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(":/max2b").scaled(230, 155, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            found = true;

        } else if (product == win5) {
            frontPic->setPixmap(QPixmap(":/win5f"));
            backPic->setPixmap(QPixmap(":/win5b"));
            found = true;

        } else if (product == win3) {
            frontPic->setPixmap(QPixmap(":/win3f").scaled(235, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backPic->setPixmap(QPixmap(":/win3b").scaled(235, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            found = true;
        }

        enableButtons(found);
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
        emit faceButtonsMap();
    }

    void HomePage::onBackButtonsMapClicked() {
        emit backButtonsMap();
    }

    void HomePage::onShowLogsBtnClicked() {
        emit showLogs();
    }

    void HomePage::onSettingsClicked() {
        emit settingsPage();
    }

    void HomePage::onApplyBtnClicked() {
        emit applyChanges();
    }

    void HomePage::onExportYamlBtnClicked() {
        emit exportYaml();
    }

    void HomePage::onImportYamlBtnClicked() {
        emit importYaml();
    }
}
