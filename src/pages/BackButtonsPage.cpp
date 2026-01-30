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

#include "BackButtonsPage.h"
#include "../include/ASCIIHIDMap.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"

namespace OWC {
    BackButtonsPage::BackButtonsPage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonsLyt = new QHBoxLayout();

        backBtn = new QPushButton("Home");
        resetBtn = new QPushButton("Reset");
        charMapBtn = new QPushButton("Char Map");
        charMap = new CharMapWidget();

        charMap->setVisible(false);

        buttonsLyt->addStretch();
        buttonsLyt->addWidget(charMapBtn);
        buttonsLyt->addWidget(backBtn);
        buttonsLyt->addWidget(resetBtn);

        lyt->addWidget(charMap);
        lyt->addStretch();
        lyt->addLayout(makeLBlock());
        lyt->addSpacing(12);
        lyt->addLayout(makeRBlock());
        lyt->addStretch();
        lyt->addLayout(buttonsLyt);

        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &BackButtonsPage::onBackBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &BackButtonsPage::onResetBtnClicked);
        QObject::connect(charMapBtn, &QPushButton::clicked, this, &BackButtonsPage::onCharMapBtnClicked);
        QObject::connect(charMap, &CharMapWidget::keyPressed, this, &BackButtonsPage::onCharMapKeyPressed);
    }

    QHBoxLayout *BackButtonsPage::makeLBlock() {
        QHBoxLayout *lyt = new QHBoxLayout();
        QLabel *plus1 = new QLabel("+");
        QLabel *plus2 = new QLabel("+");
        QLabel *plus3 = new QLabel("+");
        QLabel *lIcon = new QLabel();
        QFont plusFont = plus1->font();

        l1Btn = new QPushButton();
        l2Btn = new QPushButton();
        l3Btn = new QPushButton();
        l4Btn = new QPushButton();
        l1Ms = new QSpinBox();
        l2Ms = new QSpinBox();
        l3Ms = new QSpinBox();
        l4Ms = new QSpinBox();

        l1Btn->setFixedWidth(buttonWidth);
        l2Btn->setFixedWidth(buttonWidth);
        l3Btn->setFixedWidth(buttonWidth);
        l4Btn->setFixedWidth(buttonWidth);
        l1Ms->setRange(0, msMax);
        l1Ms->setMaximumWidth(80);
        l2Ms->setRange(0, msMax);
        l2Ms->setMaximumWidth(80);
        l3Ms->setRange(0, msMax);
        l3Ms->setMaximumWidth(80);
        l4Ms->setRange(0, msMax);
        l4Ms->setMaximumWidth(80);
        plusFont.setBold(true);
        plusFont.setPointSize(14);
        plus1->setFont(plusFont);
        plus2->setFont(plusFont);
        plus3->setFont(plusFont);
        lIcon->setPixmap(QPixmap(":/icons/l4").scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(lIcon);
        lyt->addSpacing(8);
        lyt->addWidget(l1Btn);
        lyt->addWidget(l1Ms);
        lyt->addWidget(plus1);
        lyt->addWidget(l2Btn);
        lyt->addWidget(l2Ms);
        lyt->addWidget(plus2);
        lyt->addWidget(l3Btn);
        lyt->addWidget(l3Ms);
        lyt->addWidget(plus3);
        lyt->addWidget(l4Btn);
        lyt->addWidget(l4Ms);

        QObject::connect(l1Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);
        QObject::connect(l2Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);
        QObject::connect(l3Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);
        QObject::connect(l4Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QHBoxLayout *BackButtonsPage::makeRBlock() {
        QHBoxLayout *lyt = new QHBoxLayout();
        QLabel *plus1 = new QLabel("+");
        QLabel *plus2 = new QLabel("+");
        QLabel *plus3 = new QLabel("+");
        QLabel *rIcon = new QLabel();
        QFont plusFont = plus1->font();

        r1Btn = new QPushButton();
        r2Btn = new QPushButton();
        r3Btn = new QPushButton();
        r4Btn = new QPushButton();
        r1Ms = new QSpinBox();
        r2Ms = new QSpinBox();
        r3Ms = new QSpinBox();
        r4Ms = new QSpinBox();

        r1Btn->setFixedWidth(buttonWidth);
        r2Btn->setFixedWidth(buttonWidth);
        r3Btn->setFixedWidth(buttonWidth);
        r4Btn->setFixedWidth(buttonWidth);
        r1Ms->setRange(0, msMax);
        r1Ms->setMaximumWidth(80);
        r2Ms->setRange(0, msMax);
        r2Ms->setMaximumWidth(80);
        r3Ms->setRange(0, msMax);
        r3Ms->setMaximumWidth(80);
        r4Ms->setRange(0, msMax);
        r4Ms->setMaximumWidth(80);
        plusFont.setBold(true);
        plusFont.setPointSize(14);
        plus1->setFont(plusFont);
        plus2->setFont(plusFont);
        plus3->setFont(plusFont);
        rIcon->setPixmap(QPixmap(":/icons/r4").scaled(55, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(rIcon);
        lyt->addSpacing(8);
        lyt->addWidget(r1Btn);
        lyt->addWidget(r1Ms);
        lyt->addWidget(plus1);
        lyt->addWidget(r2Btn);
        lyt->addWidget(r2Ms);
        lyt->addWidget(plus2);
        lyt->addWidget(r3Btn);
        lyt->addWidget(r3Ms);
        lyt->addWidget(plus3);
        lyt->addWidget(r4Btn);
        lyt->addWidget(r4Ms);

        QObject::connect(r1Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);
        QObject::connect(r2Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);
        QObject::connect(r3Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);
        QObject::connect(r4Btn, &QPushButton::clicked, this, &BackButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    void BackButtonsPage::setMapping(const QSharedPointer<Controller> &gpd) const {
        l1Btn->setText(QString::fromStdString(gpd->getLBack1()));
        l2Btn->setText(QString::fromStdString(gpd->getLBack2()));
        l3Btn->setText(QString::fromStdString(gpd->getLBack3()));
        l4Btn->setText(QString::fromStdString(gpd->getLBack4()));
        r1Btn->setText(QString::fromStdString(gpd->getRBack1()));
        r2Btn->setText(QString::fromStdString(gpd->getRBack2()));
        r3Btn->setText(QString::fromStdString(gpd->getRBack3()));
        r4Btn->setText(QString::fromStdString(gpd->getRBack4()));
        l1Ms->setValue(gpd->getLBack1Delay());
        l2Ms->setValue(gpd->getLBack2Delay());
        l3Ms->setValue(gpd->getLBack3Delay());
        l4Ms->setValue(gpd->getLBack4Delay());
        r1Ms->setValue(gpd->getRBack1Delay());
        r2Ms->setValue(gpd->getRBack2Delay());
        r3Ms->setValue(gpd->getRBack3Delay());
        r4Ms->setValue(gpd->getRBack4Delay());
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

    QString BackButtonsPage::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts(&yaml);

        ts << "L_BACK1: " << l1Btn->text() << "\n"
            "L_BACK1_DELAY: " << l1Ms->value() << "\n"
            "L_BACK2: " << l2Btn->text() << "\n"
            "L_BACK2_DELAY: " << l2Ms->value() << "\n"
            "L_BACK3: " << l3Btn->text() << "\n"
            "L_BACK3_DELAY: " << l3Ms->value() << "\n"
            "L_BACK4: " << l4Btn->text() << "\n"
            "L_BACK4_DELAY: " << l4Ms->value() << "\n"
            "R_BACK1: " << r1Btn->text() << "\n"
            "R_BACK1_DELAY: " << r1Ms->value() << "\n"
            "R_BACK2: " << r2Btn->text() << "\n"
            "R_BACK2_DELAY: " << r2Ms->value() << "\n"
            "R_BACK3: " << r3Btn->text() << "\n"
            "R_BACK3_DELAY: " << r3Ms->value() << "\n"
            "R_BACK4: " << r4Btn->text() << "\n"
            "R_BACK4_DELAY: " << r4Ms->value();

        return yaml;
    }

    void BackButtonsPage::importMappingFromYaml(const YAML::Node &yaml) const {
        if (yaml["L_BACK1"])
            l1Btn->setText(QString::fromStdString(yaml["L_BACK1"].as<std::string>()));

        if (yaml["L_BACK2"])
            l2Btn->setText(QString::fromStdString(yaml["L_BACK2"].as<std::string>()));

        if (yaml["L_BACK3"])
            l3Btn->setText(QString::fromStdString(yaml["L_BACK3"].as<std::string>()));

        if (yaml["L_BACK4"])
            l4Btn->setText(QString::fromStdString(yaml["L_BACK4"].as<std::string>()));

        if (yaml["R_BACK1"])
            r1Btn->setText(QString::fromStdString(yaml["R_BACK1"].as<std::string>()));

        if (yaml["R_BACK2"])
            r2Btn->setText(QString::fromStdString(yaml["R_BACK2"].as<std::string>()));

        if (yaml["R_BACK3"])
            r3Btn->setText(QString::fromStdString(yaml["R_BACK3"].as<std::string>()));

        if (yaml["R_BACK4"])
            r4Btn->setText(QString::fromStdString(yaml["R_BACK4"].as<std::string>()));

        if (yaml["L_BACK1_DELAY"])
            l1Ms->setValue(yaml["L_BACK1_DELAY"].as<int>());

        if (yaml["L_BACK2_DELAY"])
            l2Ms->setValue(yaml["L_BACK2_DELAY"].as<int>());

        if (yaml["L_BACK3_DELAY"])
            l3Ms->setValue(yaml["L_BACK3_DELAY"].as<int>());

        if (yaml["L_BACK4_DELAY"])
            l4Ms->setValue(yaml["L_BACK4_DELAY"].as<int>());

        if (yaml["R_BACK1_DELAY"])
            r1Ms->setValue(yaml["R_BACK1_DELAY"].as<int>());

        if (yaml["R_BACK2_DELAY"])
            r2Ms->setValue(yaml["R_BACK2_DELAY"].as<int>());

        if (yaml["R_BACK3_DELAY"])
            r3Ms->setValue(yaml["R_BACK3_DELAY"].as<int>());

        if (yaml["R_BACK4_DELAY"])
            r4Ms->setValue(yaml["R_BACK4_DELAY"].as<int>());
    }

    void BackButtonsPage::writeMapping(const QSharedPointer<Controller> &gpd) {
        if (!gpd->setLBack1(l1Btn->text().toStdString()))
            emit logSent("failed to set back left slot 1 key");

        if (!gpd->setLBack2(l2Btn->text().toStdString()))
            emit logSent("failed to set back left slot 2 key");

        if (!gpd->setLBack3(l3Btn->text().toStdString()))
            emit logSent("failed to set back left slot 3 key");

        if (!gpd->setLBack4(l4Btn->text().toStdString()))
            emit logSent("failed to set back left slot 4 key");

        if (!gpd->setRBack1(r1Btn->text().toStdString()))
            emit logSent("failed to set back right slot 1 key");

        if (!gpd->setRBack2(r2Btn->text().toStdString()))
            emit logSent("failed to set back right slot 2 key");

        if (!gpd->setRBack3(r3Btn->text().toStdString()))
            emit logSent("failed to set back right slot 3 key");

        if (!gpd->setRBack4(r4Btn->text().toStdString()))
            emit logSent("failed to set back right slot 4 key");

        gpd->setLBack1Delay(l1Ms->value());
        gpd->setLBack2Delay(l2Ms->value());
        gpd->setLBack3Delay(l3Ms->value());
        gpd->setLBack4Delay(l4Ms->value());
        gpd->setRBack1Delay(r1Ms->value());
        gpd->setRBack2Delay(r2Ms->value());
        gpd->setRBack3Delay(r3Ms->value());
        gpd->setRBack4Delay(r4Ms->value());
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

    void BackButtonsPage::onkeyButtonPressed() {
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

    void BackButtonsPage::onCharMapKeyPressed(const QString &key) {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }
}
