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
#include <QColorDialog>

#include "SettingsPage.h"
#include "../include/ControllerFeature.h"

namespace OWC {
    SettingsPage::SettingsPage() {
        QHBoxLayout *buttonsLyt = new QHBoxLayout();

        lyt = new QVBoxLayout();
        backBtn = new QPushButton("Home");
        resetBtn = new QPushButton("Reset");

        buttonsLyt->addStretch();
        buttonsLyt->addWidget(backBtn);
        buttonsLyt->addWidget(resetBtn);

        lyt->addStretch();
        lyt->addLayout(buttonsLyt);

        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &SettingsPage::onBackBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &SettingsPage::onResetBtnClicked);
    }

    void SettingsPage::makeRumbleV1Settings() {
        QHBoxLayout *rlyt = new QHBoxLayout();
        QLabel *title = new QLabel("Rumble");
        QFont titleFont = title->font();

        rumble = new QComboBox();

        titleFont.setBold(true);
        title->setAlignment(Qt::AlignCenter);
        title->setFont(titleFont);
        rumble->addItems({"off", "low", "high"});

        rlyt->addWidget(new QLabel("Vibration intensity:"));
        rlyt->addSpacing(4);
        rlyt->addWidget(rumble);
        rlyt->addStretch();

        lyt->insertWidget(0, title);
        lyt->insertSpacing(1, 6);
        lyt->insertLayout(2, rlyt);
        lyt->insertSpacing(3, 20);
    }

    void SettingsPage::makeShoulderLedsV1Settings() {
        QHBoxLayout *ledLyt = new QHBoxLayout();
        QLabel *title = new QLabel("Shoulder leds");
        QFont titleFont = title->font();

        ledMode = new QComboBox();
        ledColorLbl = new QLabel();
        ledColorChooserBtn = new QPushButton("Color picker");

        titleFont.setBold(true);
        title->setAlignment(Qt::AlignCenter);
        title->setFont(titleFont);
        ledMode->addItem("off", 0);
        ledMode->addItem("solid", 1);
        ledMode->addItem("breathe", 0x11);
        ledMode->addItem("rotate", 0x21);
        ledColorLbl->setAutoFillBackground(true);
        ledColorLbl->setFixedWidth(80);
        ledColorLbl->setFrameShape(QFrame::Box);

        ledLyt->addWidget(new QLabel("Mode:"));
        ledLyt->addSpacing(8);
        ledLyt->addWidget(ledMode);
        ledLyt->addSpacing(10);
        ledLyt->addWidget(ledColorLbl);
        ledLyt->addSpacing(10);
        ledLyt->addWidget(ledColorChooserBtn);
        ledLyt->addStretch();

        lyt->insertWidget(0, title);
        lyt->insertSpacing(1, 6);
        lyt->insertLayout(2, ledLyt);
        lyt->insertSpacing(3, 20);

        QObject::connect(ledMode, &QComboBox::currentIndexChanged, this, &SettingsPage::onLedModeChanged);
        QObject::connect(ledColorChooserBtn, &QPushButton::clicked, this, &SettingsPage::onLedColorChooserBtnClicked);
    }

    void SettingsPage::makeDeadzoneV1Settings() {
        QHBoxLayout *dzLyt = new QHBoxLayout();
        QVBoxLayout *leftLyt = new QVBoxLayout();
        QHBoxLayout *leftContLyt = new QHBoxLayout();
        QVBoxLayout *lCenterLyt = new QVBoxLayout();
        QHBoxLayout *lCenterLblLyt = new QHBoxLayout();
        QVBoxLayout *lBoundaryLyt = new QVBoxLayout();
        QHBoxLayout *lBoundaryLblLyt = new QHBoxLayout();
        QVBoxLayout *rightLyt = new QVBoxLayout();
        QHBoxLayout *rightContLyt = new QHBoxLayout();
        QVBoxLayout *rCenterLyt = new QVBoxLayout();
        QHBoxLayout *rCenterLblLyt = new QHBoxLayout();
        QVBoxLayout *rBoundaryLyt = new QVBoxLayout();
        QHBoxLayout *rBoundaryLblLyt = new QHBoxLayout();
        QLabel *title = new QLabel("Deadzone");
        QFont titleFont = title->font();
        QLabel *lsIcon = new QLabel();
        QLabel *rsIcon = new QLabel();

        dzLeftCenter = new QSlider(Qt::Horizontal);
        dzLeftCenterLbl = new QLabel();
        dzLeftBoundary = new QSlider(Qt::Horizontal);
        dzLeftBoundaryLbl = new QLabel();
        dzRightCenter = new QSlider(Qt::Horizontal);
        dzRightCenterLbl = new QLabel();
        dzRightBoundary = new QSlider(Qt::Horizontal);
        dzRightBoundaryLbl = new QLabel();

        titleFont.setBold(true);
        title->setAlignment(Qt::AlignCenter);
        title->setFont(titleFont);
        dzLeftCenter->setRange(-10, 10);
        dzLeftBoundary->setRange(-10, 10);
        dzRightCenter->setRange(-10, 10);
        dzRightBoundary->setRange(-10, 10);
        lsIcon->setAlignment(Qt::AlignCenter);
        lsIcon->setPixmap(QPixmap(":/icons/ls").scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        rsIcon->setAlignment(Qt::AlignCenter);
        rsIcon->setPixmap(QPixmap(":/icons/rs").scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        lCenterLblLyt->addWidget(new QLabel("center"));
        lCenterLblLyt->addStretch();
        lCenterLblLyt->addWidget(dzLeftCenterLbl);

        lBoundaryLblLyt->addWidget(new QLabel("boundary"));
        lBoundaryLblLyt->addStretch();
        lBoundaryLblLyt->addWidget(dzLeftBoundaryLbl);

        lCenterLyt->addWidget(dzLeftCenter);
        lCenterLyt->addLayout(lCenterLblLyt);

        lBoundaryLyt->addWidget(dzLeftBoundary);
        lBoundaryLyt->addLayout(lBoundaryLblLyt);

        leftContLyt->addLayout(lCenterLyt);
        leftContLyt->addSpacing(8);
        leftContLyt->addLayout(lBoundaryLyt);

        leftLyt->addWidget(lsIcon);
        leftLyt->addSpacing(8);
        leftLyt->addLayout(leftContLyt);

        rCenterLblLyt->addWidget(new QLabel("center"));
        rCenterLblLyt->addStretch();
        rCenterLblLyt->addWidget(dzRightCenterLbl);

        rBoundaryLblLyt->addWidget(new QLabel("boundary"));
        rBoundaryLblLyt->addStretch();
        rBoundaryLblLyt->addWidget(dzRightBoundaryLbl);

        rCenterLyt->addWidget(dzRightCenter);
        rCenterLyt->addLayout(rCenterLblLyt);

        rBoundaryLyt->addWidget(dzRightBoundary);
        rBoundaryLyt->addLayout(rBoundaryLblLyt);

        rightContLyt->addLayout(rCenterLyt);
        rightContLyt->addSpacing(8);
        rightContLyt->addLayout(rBoundaryLyt);

        rightLyt->addWidget(rsIcon);
        rightLyt->addSpacing(8);
        rightLyt->addLayout(rightContLyt);

        dzLyt->addLayout(leftLyt);
        dzLyt->addSpacing(20);
        dzLyt->addLayout(rightLyt);

        lyt->insertWidget(0, title);
        lyt->insertSpacing(1, 8);
        lyt->insertLayout(2, dzLyt);
        lyt->insertSpacing(3, 20);

        QObject::connect(dzLeftCenter, &QSlider::valueChanged, this, &SettingsPage::onDzLeftCenterChanged);
        QObject::connect(dzLeftBoundary, &QSlider::valueChanged, this, &SettingsPage::onDzLeftBoundaryChanged);
        QObject::connect(dzRightCenter, &QSlider::valueChanged, this, &SettingsPage::onDzRightCenterChanged);
        QObject::connect(dzRightBoundary, &QSlider::valueChanged, this, &SettingsPage::onDzRightBoundaryChanged);
    }

    void SettingsPage::initPage(const QSharedPointer<Controller> &gpd) {
        if (gpd->hasFeature(ControllerFeature::ShoulderLedsV1))
            makeShoulderLedsV1Settings();

        if (gpd->hasFeature(ControllerFeature::RumbleV1))
            makeRumbleV1Settings();

        if (gpd->hasFeature(ControllerFeature::DeadZoneControlV1))
            makeDeadzoneV1Settings();
    }

    void SettingsPage::setData(const QSharedPointer<Controller> &gpd) const {
        if (gpd->hasFeature(ControllerFeature::ShoulderLedsV1)) {
            const std::tuple<int, int, int> lcolor = gpd->getLedColor();
            const QColor color = QColor(std::get<0>(lcolor), std::get<1>(lcolor), std::get<2>(lcolor));
            const QString mode = QString::fromStdString(gpd->getLedMode());
            QPalette ledColorLblPal = ledColorLbl->palette();

            ledColorLblPal.setColor(QPalette::Window, color);
            ledColorLbl->setPalette(ledColorLblPal);
            ledMode->setCurrentIndex(ledMode->findText(mode));
        }

        if (gpd->hasFeature(ControllerFeature::RumbleV1))
            rumble->setCurrentIndex(rumble->findText(QString::fromStdString(gpd->getRumbleMode())));

        if (gpd->hasFeature(ControllerFeature::DeadZoneControlV1)) {
            dzLeftCenter->setValue(gpd->getLAnalogCenter());
            dzLeftBoundary->setValue(gpd->getLAnalogBoundary());
            dzRightCenter->setValue(gpd->getRAnalogCenter());
            dzRightBoundary->setValue(gpd->getRAnalogBoundary());
        }
    }

    void SettingsPage::writeSettings(const QSharedPointer<Controller> &gpd) const {
        if (gpd->hasFeature(ControllerFeature::RumbleV1))
            gpd->setRumble(rumble->currentText().toStdString());

        if (gpd->hasFeature(ControllerFeature::DeadZoneControlV1)) {
            gpd->setLAnalogCenter(dzLeftCenter->value());
            gpd->setLAnalogBoundary(dzLeftBoundary->value());
            gpd->setRAnalogCenter(dzRightCenter->value());
            gpd->setRAnalogBoundary(dzRightBoundary->value());
        }

        if (gpd->hasFeature(ControllerFeature::ShoulderLedsV1)) {
            const QColor color = ledColorLbl->palette().color(QPalette::Window);

            gpd->setLedMode(ledMode->currentText().toStdString());
            gpd->setLedColor(color.red(), color.green(), color.blue());
        }
    }

    void SettingsPage::onBackBtnClicked() {
        emit backToHome();
    }

    void SettingsPage::onResetBtnClicked() {
        emit resetSettings();
    }

    void SettingsPage::onLedColorChooserBtnClicked() {
        const QColor color = QColorDialog::getColor(Qt::white, this, "Select led color");

        if (!color.isValid())
            return;

        QPalette colorLblPal = ledColorLbl->palette();

        colorLblPal.setColor(QPalette::Window, color);
        ledColorLbl->setPalette(colorLblPal);
    }

    void SettingsPage::onLedModeChanged(const int idx) const {
        const bool hasColor = idx == 1 || idx == 2;

        ledColorLbl->setVisible(hasColor);
        ledColorChooserBtn->setVisible(hasColor);
    }

    void SettingsPage::onDzLeftCenterChanged(const int v) const {
        dzLeftCenterLbl->setNum(v);
    }

    void SettingsPage::onDzLeftBoundaryChanged(const int v) const {
        dzLeftBoundaryLbl->setNum(v);
    }

    void SettingsPage::onDzRightCenterChanged(const int v) const {
        dzRightCenterLbl->setNum(v);
    }

    void SettingsPage::onDzRightBoundaryChanged(const int v) const {
        dzRightBoundaryLbl->setNum(v);
    }
}
