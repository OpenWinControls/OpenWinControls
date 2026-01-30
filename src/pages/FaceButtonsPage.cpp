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
        dpadUpBtn->setText(QString::fromStdString(gpd->getDpadUp()));
        dpadDownBtn->setText(QString::fromStdString(gpd->getDpadDown()));
        dpadLeftBtn->setText(QString::fromStdString(gpd->getDpadLeft()));
        dpadRight->setText(QString::fromStdString(gpd->getDpadRight()));
        LAnalogUpBtn->setText(QString::fromStdString(gpd->getLAnalogUp()));
        LAnalogDownBtn->setText(QString::fromStdString(gpd->getLAnalogDown()));
        LAnalogLeftBtn->setText(QString::fromStdString(gpd->getLAnalogLeft()));
        LAnalogRightBtn->setText(QString::fromStdString(gpd->getLAnalogRight()));
        aBtn->setText(QString::fromStdString(gpd->getA()));
        bBtn->setText(QString::fromStdString(gpd->getB()));
        xBtn->setText(QString::fromStdString(gpd->getX()));
        yBtn->setText(QString::fromStdString(gpd->getY()));
        startBtn->setText(QString::fromStdString(gpd->getStart()));
        selectBtn->setText(QString::fromStdString(gpd->getSelect()));
        menuBtn->setText(QString::fromStdString(gpd->getMenu()));
        l1Btn->setText(QString::fromStdString(gpd->getL1()));
        l2Btn->setText(QString::fromStdString(gpd->getL2()));
        l3Btn->setText(QString::fromStdString(gpd->getL3()));
        r1Btn->setText(QString::fromStdString(gpd->getR1()));
        r2Btn->setText(QString::fromStdString(gpd->getR2()));
        r3Btn->setText(QString::fromStdString(gpd->getR3()));
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

        ts << "A: " << aBtn->text() << "\n"
            "B: " << bBtn->text() << "\n"
            "X: " << xBtn->text() << "\n"
            "Y: " << yBtn->text() << "\n"
            "DPAD_UP: " << dpadUpBtn->text() << "\n"
            "DPAD_DOWN: " << dpadDownBtn->text() << "\n"
            "DPAD_LEFT: " << dpadLeftBtn->text() << "\n"
            "DPAD_RIGHT: " << dpadRight->text() << "\n"
            "L_ANALOG_UP: " << LAnalogUpBtn->text() << "\n"
            "L_ANALOG_DOWN: " << LAnalogDownBtn->text() << "\n"
            "L_ANALOG_LEFT: " << LAnalogLeftBtn->text() << "\n"
            "L_ANALOG_RIGHT: " << LAnalogRightBtn->text() << "\n"
            "L1: " << l1Btn->text() << "\n"
            "L2: " << l2Btn->text() << "\n"
            "L3: " << l3Btn->text() << "\n"
            "R1: " << r1Btn->text() << "\n"
            "R2: " << r2Btn->text() << "\n"
            "R3: " << r3Btn->text() << "\n"
            "START: " << startBtn->text() << "\n"
            "SELECT: " << selectBtn->text() << "\n"
            "MENU: " << menuBtn->text();

        return yaml;
    }

    void FaceButtonsPage::importMappingFromYaml(const YAML::Node &yaml) const {
        if (yaml["DPAD_UP"])
            dpadUpBtn->setText(QString::fromStdString(yaml["DPAD_UP"].as<std::string>()));

        if (yaml["DPAD_DOWN"])
            dpadDownBtn->setText(QString::fromStdString(yaml["DPAD_DOWN"].as<std::string>()));

        if (yaml["DPAD_LEFT"])
            dpadLeftBtn->setText(QString::fromStdString(yaml["DPAD_LEFT"].as<std::string>()));

        if (yaml["DPAD_RIGHT"])
            dpadRight->setText(QString::fromStdString(yaml["DPAD_RIGHT"].as<std::string>()));

        if (yaml["A"])
            aBtn->setText(QString::fromStdString(yaml["A"].as<std::string>()));

        if (yaml["B"])
            bBtn->setText(QString::fromStdString(yaml["B"].as<std::string>()));

        if (yaml["X"])
            xBtn->setText(QString::fromStdString(yaml["X"].as<std::string>()));

        if (yaml["Y"])
            yBtn->setText(QString::fromStdString(yaml["Y"].as<std::string>()));

        if (yaml["L_ANALOG_UP"])
            LAnalogUpBtn->setText(QString::fromStdString(yaml["L_ANALOG_UP"].as<std::string>()));

        if (yaml["L_ANALOG_DOWN"])
            LAnalogDownBtn->setText(QString::fromStdString(yaml["L_ANALOG_DOWN"].as<std::string>()));

        if (yaml["L_ANALOG_LEFT"])
            LAnalogLeftBtn->setText(QString::fromStdString(yaml["L_ANALOG_LEFT"].as<std::string>()));

        if (yaml["L_ANALOG_RIGHT"])
            LAnalogRightBtn->setText(QString::fromStdString(yaml["L_ANALOG_RIGHT"].as<std::string>()));

        if (yaml["START"])
            startBtn->setText(QString::fromStdString(yaml["START"].as<std::string>()));

        if (yaml["SELECT"])
            selectBtn->setText(QString::fromStdString(yaml["SELECT"].as<std::string>()));

        if (yaml["MENU"])
            menuBtn->setText(QString::fromStdString(yaml["MENU"].as<std::string>()));

        if (yaml["L1"])
            l1Btn->setText(QString::fromStdString(yaml["L1"].as<std::string>()));

        if (yaml["L2"])
            l2Btn->setText(QString::fromStdString(yaml["L2"].as<std::string>()));

        if (yaml["L3"])
            l3Btn->setText(QString::fromStdString(yaml["L3"].as<std::string>()));

        if (yaml["R1"])
            r1Btn->setText(QString::fromStdString(yaml["R1"].as<std::string>()));

        if (yaml["R2"])
            r2Btn->setText(QString::fromStdString(yaml["R2"].as<std::string>()));

        if (yaml["R3"])
            r3Btn->setText(QString::fromStdString(yaml["R3"].as<std::string>()));
    }

    void FaceButtonsPage::writeMapping(const QSharedPointer<Controller> &gpd) {
        if (!gpd->setDpadUp(dpadUpBtn->text().toStdString()))
            emit logSent("failed to set dpad up key");

        if (!gpd->setDpadDown(dpadDownBtn->text().toStdString()))
            emit logSent("failed to set dpad down key");

        if (!gpd->setDpadLeft(dpadLeftBtn->text().toStdString()))
            emit logSent("failed to set dpad left key");

        if (!gpd->setDpadRight(dpadRight->text().toStdString()))
            emit logSent("failed to set dpad right key");

        if (!gpd->setA(aBtn->text().toStdString()))
            emit logSent("failed to set A key");

        if (!gpd->setB(bBtn->text().toStdString()))
            emit logSent("failed to set B key");

        if (!gpd->setX(xBtn->text().toStdString()))
            emit logSent("failed to set X key");

        if (!gpd->setY(yBtn->text().toStdString()))
            emit logSent("failed to set Y key");

        if (!gpd->setLAnalogUp(LAnalogUpBtn->text().toStdString()))
            emit logSent("failed to set left analog up key");

        if (!gpd->setLAnalogDown(LAnalogDownBtn->text().toStdString()))
            emit logSent("failed to set left analog down key");

        if (!gpd->setLAnalogLeft(LAnalogLeftBtn->text().toStdString()))
            emit logSent("failed to set left analog left key");

        if (!gpd->setLAnalogRight(LAnalogRightBtn->text().toStdString()))
            emit logSent("failed to set left analog right key");

        if (!gpd->setStart(startBtn->text().toStdString()))
            emit logSent("failed to set start key");

        if (!gpd->setSelect(selectBtn->text().toStdString()))
            emit logSent("failed to set select key");

        if (!gpd->setMenu(menuBtn->text().toStdString()))
            emit logSent("failed to set menu key");

        if (!gpd->setL1(l1Btn->text().toStdString()))
            emit logSent("failed to set L1 key");

        if (!gpd->setL2(l2Btn->text().toStdString()))
            emit logSent("failed to set L2 key");

        if (!gpd->setL3(l3Btn->text().toStdString()))
            emit logSent("failed to set L3 key");

        if (!gpd->setR1(r1Btn->text().toStdString()))
            emit logSent("failed to set R1 key");

        if (!gpd->setR2(r2Btn->text().toStdString()))
            emit logSent("failed to set R2 key");

        if (!gpd->setR3(r3Btn->text().toStdString()))
            emit logSent("failed to set R3 key");
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
