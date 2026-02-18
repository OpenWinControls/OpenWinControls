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

#include "FaceButtonsPage.h"
#include "../include/ASCIIHIDMap.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"

namespace OWC {
    FaceButtonsPage::FaceButtonsPage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonsLyt = new QHBoxLayout();
        QHBoxLayout *row1Lyt = new QHBoxLayout();
        QHBoxLayout *row2Lyt = new QHBoxLayout();

        backBtn = new QPushButton("Home");
        resetBtn = new QPushButton("Reset");
        charMapBtn = new QPushButton("Char Map");
        charMap = new CharMapWidget();

        charMap->setVisible(false);

        row1Lyt->addLayout(makeDPadBlock());
        row1Lyt->addStretch();
        row1Lyt->addLayout(makeL1L2Block());
        row1Lyt->addStretch();
        row1Lyt->addLayout(makeR1R2Block());
        row1Lyt->addStretch();
        row1Lyt->addLayout(makeActionsBlock());

        row2Lyt->addLayout(makeLeftAnalogBlock());
        row2Lyt->addStretch();
        row2Lyt->addLayout(makeL3Block());
        row2Lyt->addStretch();
        row2Lyt->addLayout(makeR3Block());
        row2Lyt->addStretch();
        row2Lyt->addLayout(makeSelectBlock());
        row2Lyt->addLayout(makeStartBlock());
        row2Lyt->addLayout(makeMenuBlock());

        buttonsLyt->addStretch();
        buttonsLyt->addWidget(charMapBtn);
        buttonsLyt->addWidget(backBtn);
        buttonsLyt->addWidget(resetBtn);

        lyt->addWidget(charMap);
        lyt->addSpacing(12);
        lyt->addLayout(row1Lyt);
        lyt->addSpacing(12);
        lyt->addLayout(row2Lyt);
        lyt->addStretch();
        lyt->addLayout(buttonsLyt);

        setLayout(lyt);

        btnMap = {
            {"A", {aBtn, Button::KBD_A}},
            {"B", {bBtn, Button::KBD_B}},
            {"X", {xBtn, Button::KBD_X}},
            {"Y", {yBtn, Button::KBD_Y}},
            {"DPAD_UP", {dpadUpBtn, Button::KBD_DPAD_UP}},
            {"DPAD_DOWN", {dpadDownBtn, Button::KBD_DPAD_DOWN}},
            {"DPAD_LEFT", {dpadLeftBtn, Button::KBD_DPAD_LEFT}},
            {"DPAD_RIGHT", {dpadRight, Button::KBD_DPAD_RIGHT}},
            {"L_ANALOG_UP", {LAnalogUpBtn, Button::KBD_LANALOG_UP}},
            {"L_ANALOG_DOWN", {LAnalogDownBtn, Button::KBD_LANALOG_DOWN}},
            {"L_ANALOG_LEFT", {LAnalogLeftBtn, Button::KBD_LANALOG_LEFT}},
            {"L_ANALOG_RIGHT", {LAnalogRightBtn, Button::KBD_LANALOG_RIGHT}},
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

        QObject::connect(backBtn, &QPushButton::clicked, this, &FaceButtonsPage::onBackBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &FaceButtonsPage::onResetBtnClicked);
        QObject::connect(charMapBtn, &QPushButton::clicked, this, &FaceButtonsPage::onCharMapBtnClicked);
        QObject::connect(charMap, &CharMapWidget::keyPressed, this, &FaceButtonsPage::onCharMapKeyPressed);
    }

    QVBoxLayout *FaceButtonsPage::makeDPadBlock() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *topLyt = new QHBoxLayout();
        QHBoxLayout *centerLyt = new QHBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QLabel *dpadIcon = new QLabel();

        dpadUpBtn = new QPushButton();
        dpadDownBtn = new QPushButton();
        dpadLeftBtn = new QPushButton();
        dpadRight = new QPushButton();

        dpadUpBtn->setFixedWidth(buttonWidth);
        dpadDownBtn->setFixedWidth(buttonWidth);
        dpadLeftBtn->setFixedWidth(buttonWidth);
        dpadRight->setFixedWidth(buttonWidth);
        dpadIcon->setPixmap(QPixmap(":/icons/dpad").scaled(75, 75, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        topLyt->setAlignment(Qt::AlignCenter);
        topLyt->addWidget(dpadUpBtn);

        centerLyt->setAlignment(Qt::AlignCenter);
        centerLyt->addWidget(dpadLeftBtn);
        centerLyt->addWidget(dpadIcon);
        centerLyt->addWidget(dpadRight);

        bottomLyt->setAlignment(Qt::AlignCenter);
        bottomLyt->addWidget(dpadDownBtn);

        lyt->addLayout(topLyt);
        lyt->addLayout(centerLyt);
        lyt->addLayout(bottomLyt);

        QObject::connect(dpadUpBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(dpadDownBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(dpadLeftBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(dpadRight, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeLeftAnalogBlock() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *topLyt = new QHBoxLayout();
        QHBoxLayout *centerLyt = new QHBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QLabel *analogIcon = new QLabel();

        LAnalogUpBtn = new QPushButton();
        LAnalogDownBtn = new QPushButton();
        LAnalogLeftBtn = new QPushButton();
        LAnalogRightBtn = new QPushButton();

        LAnalogUpBtn->setFixedWidth(buttonWidth);
        LAnalogDownBtn->setFixedWidth(buttonWidth);
        LAnalogLeftBtn->setFixedWidth(buttonWidth);
        LAnalogRightBtn->setFixedWidth(buttonWidth);
        analogIcon->setPixmap(QPixmap(":/icons/ls").scaled(62, 62, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        topLyt->setAlignment(Qt::AlignCenter);
        topLyt->addWidget(LAnalogUpBtn);

        centerLyt->setAlignment(Qt::AlignCenter);
        centerLyt->addWidget(LAnalogLeftBtn);
        centerLyt->addWidget(analogIcon);
        centerLyt->addWidget(LAnalogRightBtn);

        bottomLyt->setAlignment(Qt::AlignCenter);
        bottomLyt->addWidget(LAnalogDownBtn);

        lyt->addLayout(topLyt);
        lyt->addLayout(centerLyt);
        lyt->addLayout(bottomLyt);

        QObject::connect(LAnalogUpBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(LAnalogDownBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(LAnalogLeftBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(LAnalogRightBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeActionsBlock() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *topLyt = new QHBoxLayout();
        QHBoxLayout *centerLyt = new QHBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QLabel *faceBtnIcon = new QLabel();

        aBtn = new QPushButton();
        bBtn = new QPushButton();
        xBtn = new QPushButton();
        yBtn = new QPushButton();

        aBtn->setFixedWidth(buttonWidth);
        bBtn->setFixedWidth(buttonWidth);
        xBtn->setFixedWidth(buttonWidth);
        yBtn->setFixedWidth(buttonWidth);
        faceBtnIcon->setPixmap(QPixmap(":/icons/face").scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        topLyt->setAlignment(Qt::AlignCenter);
        topLyt->addWidget(yBtn);

        centerLyt->setAlignment(Qt::AlignCenter);
        centerLyt->addWidget(xBtn);
        centerLyt->addWidget(faceBtnIcon);
        centerLyt->addWidget(bBtn);

        bottomLyt->setAlignment(Qt::AlignCenter);
        bottomLyt->addWidget(aBtn);

        lyt->addLayout(topLyt);
        lyt->addLayout(centerLyt);
        lyt->addLayout(bottomLyt);

        QObject::connect(aBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(bBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(xBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(yBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeL1L2Block() {
        QVBoxLayout *l1Lyt = new QVBoxLayout();
        QVBoxLayout *l2Lyt = new QVBoxLayout();
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *l1Icon = new QLabel();
        QLabel *l2Icon = new QLabel();

        l1Btn = new QPushButton();
        l2Btn = new QPushButton();

        l1Btn->setFixedWidth(buttonWidth);
        l2Btn->setFixedWidth(buttonWidth);
        l1Icon->setPixmap(QPixmap(":/icons/l1").scaled(70, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        l1Icon->setAlignment(Qt::AlignCenter);
        l2Icon->setPixmap(QPixmap(":/icons/l2").scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        l2Icon->setAlignment(Qt::AlignCenter);

        l1Lyt->addWidget(l1Btn);
        l1Lyt->addWidget(l1Icon);

        l2Lyt->addWidget(l2Btn);
        l2Lyt->addWidget(l2Icon);

        lyt->addLayout(l2Lyt);
        lyt->addSpacing(8);
        lyt->addLayout(l1Lyt);

        QObject::connect(l1Btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(l2Btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeR1R2Block() {
        QVBoxLayout *r1Lyt = new QVBoxLayout();
        QVBoxLayout *r2Lyt = new QVBoxLayout();
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *r1Icon = new QLabel();
        QLabel *r2Icon = new QLabel();

        r1Btn = new QPushButton();
        r2Btn = new QPushButton();

        r1Btn->setFixedWidth(buttonWidth);
        r2Btn->setFixedWidth(buttonWidth);
        r1Icon->setPixmap(QPixmap(":/icons/r1").scaled(70, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        r1Icon->setAlignment(Qt::AlignCenter);
        r2Icon->setPixmap(QPixmap(":/icons/r2").scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        r2Icon->setAlignment(Qt::AlignCenter);

        r1Lyt->addWidget(r1Btn);
        r1Lyt->addWidget(r1Icon);

        r2Lyt->addWidget(r2Btn);
        r2Lyt->addWidget(r2Icon);

        lyt->addLayout(r2Lyt);
        lyt->addSpacing(8);
        lyt->addLayout(r1Lyt);

        QObject::connect(r1Btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(r2Btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeStartBlock() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *startIcon = new QLabel();

        startBtn = new QPushButton();

        startBtn->setFixedWidth(buttonWidth);
        startIcon->setPixmap(QPixmap(":/icons/start").scaled(70, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        startIcon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(startBtn);
        lyt->addWidget(startIcon);

        QObject::connect(startBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeSelectBlock() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *selectIcon = new QLabel();

        selectBtn = new QPushButton();

        selectBtn->setFixedWidth(buttonWidth);
        selectIcon->setPixmap(QPixmap(":/icons/select").scaled(70, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        selectIcon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(selectBtn);
        lyt->addWidget(selectIcon);

        QObject::connect(selectBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeMenuBlock() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *menuIcon = new QLabel();

        menuBtn = new QPushButton();

        menuBtn->setFixedWidth(buttonWidth);
        menuIcon->setPixmap(QPixmap(":/icons/menu").scaled(70, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        menuIcon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(menuBtn);
        lyt->addWidget(menuIcon);

        QObject::connect(menuBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeL3Block() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *l3Icon = new QLabel();

        l3Btn = new QPushButton();

        l3Btn->setFixedWidth(buttonWidth);
        l3Icon->setPixmap(QPixmap(":/icons/l3").scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        l3Icon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(l3Btn);
        lyt->addWidget(l3Icon);

        QObject::connect(l3Btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeR3Block() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *r3Icon = new QLabel();

        r3Btn = new QPushButton();

        r3Btn->setFixedWidth(buttonWidth);
        r3Icon->setPixmap(QPixmap(":/icons/r3").scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        r3Icon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(r3Btn);
        lyt->addWidget(r3Icon);

        QObject::connect(r3Btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    void FaceButtonsPage::setMapping(const QSharedPointer<Controller> &gpd) const {
        for (const auto &[key, btnPair]: btnMap.asKeyValueRange())
            btnPair.first->setText(QString::fromStdString(gpd->getButton(btnPair.second)));
    }

    void FaceButtonsPage::keyPressEvent(QKeyEvent *event) {
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

    QString FaceButtonsPage::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts (&yaml);

        for (const auto &[key, btnPair]: btnMap.asKeyValueRange())
            ts << key.data() << ": " << btnPair.first->text() << "\n";

        return yaml;
    }

    void FaceButtonsPage::importMappingFromYaml(const YAML::Node &yaml) const {
        for (const auto &[key, btnPair]: btnMap.asKeyValueRange()) {
            if (yaml[key])
                btnPair.first->setText(QString::fromStdString(yaml[key].as<std::string>()));
        }
    }

    void FaceButtonsPage::writeMapping(const QSharedPointer<Controller> &gpd) {
        for (const auto &[key, btnPair]: btnMap.asKeyValueRange()) {
            if (!gpd->setButton(btnPair.second, btnPair.first->text().toStdString()))
                emit logSent(QString("failed to set %1").arg(key));
        }
    }

    void FaceButtonsPage::onBackBtnClicked() {
        emit backToHome();
    }

    void FaceButtonsPage::onResetBtnClicked() {
        emit resetFaceButtons();
    }

    void FaceButtonsPage::onCharMapBtnClicked() const {
        charMap->setVisible(!charMap->isVisible());
    }

    void FaceButtonsPage::onkeyButtonPressed() {
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

    void FaceButtonsPage::onCharMapKeyPressed(const QString &key) {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }
}
