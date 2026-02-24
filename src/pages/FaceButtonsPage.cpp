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

#include "FaceButtonsPage.h"

namespace OWC {
    FaceButtonsPage::FaceButtonsPage(const CharMapMode charMapMode) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonsLyt = new QHBoxLayout();
        QHBoxLayout *row1Lyt = new QHBoxLayout();
        QHBoxLayout *row2Lyt = new QHBoxLayout();
        QScrollArea *scrollArea = new QScrollArea();

        controlsLyt = new QVBoxLayout();
        backBtn = new QPushButton("Home");
        resetBtn = new QPushButton("Reset");
        charMapBtn = new QPushButton("Char Map");
        charMap = new CharMapWidget(charMapMode);
        dpadUpBtn = new QPushButton();
        dpadDownBtn = new QPushButton();
        dpadLeftBtn = new QPushButton();
        dpadRight = new QPushButton();
        lAnalogUpBtn = new QPushButton();
        lAnalogDownBtn = new QPushButton();
        lAnalogLeftBtn = new QPushButton();
        lAnalogRightBtn = new QPushButton();
        aBtn = new QPushButton();
        bBtn = new QPushButton();
        xBtn = new QPushButton();
        yBtn = new QPushButton();
        l1Btn = new QPushButton();
        l2Btn = new QPushButton();
        r1Btn = new QPushButton();
        r2Btn = new QPushButton();
        startBtn = new QPushButton();
        selectBtn = new QPushButton();
        menuBtn = new QPushButton();
        l3Btn = new QPushButton();
        r3Btn = new QPushButton();

        charMap->setVisible(false);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

        row1Lyt->addLayout(makeDirectionalBlock(dpadUpBtn, dpadLeftBtn, dpadRight, dpadDownBtn, "dpad", 75));
        row1Lyt->addStretch();
        row1Lyt->addLayout(makeShoulderBlock(l1Btn, l2Btn, "l1", "l2", 70, 30, 55, 55));
        row1Lyt->addLayout(makeShoulderBlock(r1Btn, r2Btn, "r1", "r2", 70, 30, 55, 55));
        row1Lyt->addStretch();
        row1Lyt->addLayout(makeDirectionalBlock(yBtn, xBtn, bBtn, aBtn, "face", 80));

        row2Lyt->addLayout(makeDirectionalBlock(lAnalogUpBtn, lAnalogLeftBtn, lAnalogRightBtn, lAnalogDownBtn, "ls", 62));
        row2Lyt->addStretch();
        row2Lyt->addLayout(makeSingleBtnBlock(l3Btn, "l3", 55, 55));
        row2Lyt->addLayout(makeSingleBtnBlock(r3Btn, "r3", 55, 55));
        row2Lyt->addStretch();
        row2Lyt->addLayout(makeSingleBtnBlock(startBtn, "start", 70, 30));
        row2Lyt->addLayout(makeSingleBtnBlock(selectBtn, "select", 70, 30));
        row2Lyt->addLayout(makeSingleBtnBlock(menuBtn, "menu", 70, 30));

        controlsLyt->addLayout(row1Lyt);
        controlsLyt->addSpacing(12);
        controlsLyt->addLayout(row2Lyt);

        buttonsLyt->addStretch();
        buttonsLyt->addWidget(charMapBtn);
        buttonsLyt->addWidget(backBtn);
        buttonsLyt->addWidget(resetBtn);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(charMap);
        lyt->addSpacing(12);
        lyt->addWidget(scrollArea);
        lyt->addLayout(buttonsLyt);

        scrollArea->widget()->setLayout(controlsLyt);
        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &FaceButtonsPage::onBackBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &FaceButtonsPage::onResetBtnClicked);
        QObject::connect(charMapBtn, &QPushButton::clicked, this, &FaceButtonsPage::onCharMapBtnClicked);
        QObject::connect(charMap, &CharMapWidget::keyPressed, this, &FaceButtonsPage::onCharMapKeyPressed);
    }

    QVBoxLayout *FaceButtonsPage::makeDirectionalBlock(QPushButton *upBtn, QPushButton *leftBtn, QPushButton *rightBtn, QPushButton *downBtn, const QString &icon, const int iconSize) const {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *topLyt = new QHBoxLayout();
        QHBoxLayout *centerLyt = new QHBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QLabel *controlIcon = new QLabel();

        upBtn->setFixedWidth(buttonWidth);
        leftBtn->setFixedWidth(buttonWidth);
        rightBtn->setFixedWidth(buttonWidth);
        downBtn->setFixedWidth(buttonWidth);
        controlIcon->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(iconSize, iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        topLyt->setAlignment(Qt::AlignCenter);
        topLyt->addWidget(upBtn);

        centerLyt->setAlignment(Qt::AlignCenter);
        centerLyt->addWidget(leftBtn);
        centerLyt->addWidget(controlIcon);
        centerLyt->addWidget(rightBtn);

        bottomLyt->setAlignment(Qt::AlignCenter);
        bottomLyt->addWidget(downBtn);

        lyt->addLayout(topLyt);
        lyt->addLayout(centerLyt);
        lyt->addLayout(bottomLyt);

        QObject::connect(upBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(leftBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(rightBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(downBtn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeShoulderBlock(QPushButton *btn1, QPushButton *btn2, const ::QString &icon1, const ::QString &icon2, const int icon1W, const int icon1H, const int icon2W, const int icon2H) const {
        QVBoxLayout *btn1Lyt = new QVBoxLayout();
        QVBoxLayout *btn2Lyt = new QVBoxLayout();
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *sIcon1 = new QLabel();
        QLabel *sIcon2 = new QLabel();

        btn1->setFixedWidth(buttonWidth);
        btn2->setFixedWidth(buttonWidth);
        sIcon1->setPixmap(QPixmap(QString(":/icons/%1").arg(icon1)).scaled(icon1W, icon1H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        sIcon1->setAlignment(Qt::AlignCenter);
        sIcon2->setPixmap(QPixmap(QString(":/icons/%1").arg(icon2)).scaled(icon2W, icon2H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        sIcon2->setAlignment(Qt::AlignCenter);

        btn1Lyt->addWidget(btn1);
        btn1Lyt->addWidget(sIcon1);

        btn2Lyt->addWidget(btn2);
        btn2Lyt->addWidget(sIcon2);

        lyt->addLayout(btn2Lyt);
        lyt->addSpacing(8);
        lyt->addLayout(btn1Lyt);

        QObject::connect(btn1, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);
        QObject::connect(btn2, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    QVBoxLayout *FaceButtonsPage::makeSingleBtnBlock(QPushButton *btn, const QString &icon, const int iconW, const int iconH) const {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *startIcon = new QLabel();

        btn->setFixedWidth(buttonWidth);
        startIcon->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(iconW, iconH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        startIcon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(btn);
        lyt->addWidget(startIcon);

        QObject::connect(btn, &QPushButton::clicked, this, &FaceButtonsPage::onkeyButtonPressed);

        return lyt;
    }

    void FaceButtonsPage::setMapping(const QSharedPointer<Controller> &gpd) const {
        for (const auto &[key, btnPair]: btnMap.asKeyValueRange())
            btnPair.first->setText(QString::fromStdString(gpd->getButton(btnPair.second)));
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
                btnPair.first->setText(QString::fromStdString(yaml[key].as<std::string>()).toUpper());
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

    void FaceButtonsPage::onCharMapBtnClicked() const {
        charMap->setVisible(!charMap->isVisible());
    }

    void FaceButtonsPage::onkeyButtonPressed() const {
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

    void FaceButtonsPage::onCharMapKeyPressed(const QString &key) const {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }
}
