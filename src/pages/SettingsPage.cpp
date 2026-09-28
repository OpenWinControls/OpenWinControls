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
#include <QColorDialog>
#include <QMessageBox>

#include "SettingsPage.h"
#include "../include/ControllerFeature.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    SettingsPage::SettingsPage(const std::shared_ptr<Controller> &gpd) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonsLyt = new QHBoxLayout();
        QPushButton *backBtn = new QPushButton(u"Home"_s);
        QPushButton *resetBtn = new QPushButton(u"Reset"_s);
        QPushButton *restoreBtn = new QPushButton(u"Restore"_s);

        controller = gpd;

        buttonsLyt->addWidget(restoreBtn);
        buttonsLyt->addWidget(resetBtn);
        buttonsLyt->addStretch();
        buttonsLyt->addWidget(backBtn);

        if (gpd->hasFeature(ControllerFeature::ShoulderLedsV1))
            lyt->addLayout(makeShoulderLedsV1());

        if (gpd->hasFeature(ControllerFeature::RumbleV1))
            lyt->addLayout(makeRumbleV1());

        if (gpd->hasFeature(ControllerFeature::DeadZoneControlV1))
            lyt->addLayout(makeDeadzoneV1());

        lyt->addStretch();
        lyt->addLayout(buttonsLyt);

        setLayout(lyt);

        QObject::connect(restoreBtn, &QPushButton::clicked, this, &SettingsPage::onRestoreBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &SettingsPage::refresh);
        QObject::connect(backBtn, &QPushButton::clicked, this, &SettingsPage::onBackBtnClicked);
    }

    QVBoxLayout *SettingsPage::makeRumbleV1() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *settLyt = new QHBoxLayout();
        QLabel *title = new QLabel(u"Rumble"_s);
        QFont titleFont = title->font();

        rumble = new QComboBox();

        titleFont.setBold(true);
        title->setAlignment(Qt::AlignCenter);
        title->setFont(titleFont);
        rumble->addItems({u"off"_s, u"low"_s, u"high"_s});

        settLyt->addWidget(new QLabel(u"Vibration intensity:"_s));
        settLyt->addSpacing(4);
        settLyt->addWidget(rumble);
        settLyt->addStretch();

        lyt->addWidget(title);
        lyt->addSpacing(6);
        lyt->addLayout(settLyt);
        lyt->addSpacing(20);

        return lyt;
    }

    QVBoxLayout *SettingsPage::makeShoulderLedsV1() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *ledLyt = new QHBoxLayout();
        QLabel *title = new QLabel(u"Shoulder leds"_s);
        QFont titleFont = title->font();

        ledMode = new QComboBox();
        ledColorLbl = new QLabel();
        ledColorPickBtn = new QPushButton(u"Color picker"_s);

        titleFont.setBold(true);
        title->setAlignment(Qt::AlignCenter);
        title->setFont(titleFont);
        ledMode->addItems({u"off"_s, u"solid"_s, u"breathe"_s, u"rotate"_s});
        ledColorLbl->setAutoFillBackground(true);
        ledColorLbl->setFixedWidth(80);
        ledColorLbl->setFrameShape(QFrame::Box);

        ledLyt->addWidget(new QLabel(u"Mode:"_s));
        ledLyt->addSpacing(8);
        ledLyt->addWidget(ledMode);
        ledLyt->addSpacing(10);
        ledLyt->addWidget(ledColorLbl);
        ledLyt->addSpacing(10);
        ledLyt->addWidget(ledColorPickBtn);
        ledLyt->addStretch();

        lyt->addWidget(title);
        lyt->addSpacing(6);
        lyt->addLayout(ledLyt);
        lyt->addSpacing(20);

        QObject::connect(ledMode, &QComboBox::currentIndexChanged, this, &SettingsPage::onLedModeChanged);
        QObject::connect(ledColorPickBtn, &QPushButton::clicked, this, &SettingsPage::onLedColorPickBtnClicked);

        return lyt;
    }

    QVBoxLayout *SettingsPage::makeDeadzoneV1() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *settLyt = new QHBoxLayout();
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
        QLabel *title = new QLabel(u"Deadzone"_s);
        QLabel *leftCenterLbl = new QLabel(u"0"_s);
        QLabel *leftBoundaryLbl = new QLabel(u"0"_s);
        QLabel *rightCenterLbl = new QLabel(u"0"_s);
        QLabel *rightBoundaryLbl = new QLabel(u"0"_s);
        QFont titleFont = title->font();
        QLabel *lsIcon = new QLabel();
        QLabel *rsIcon = new QLabel();

        dzLeftCenter = new QSlider(Qt::Horizontal);
        dzLeftBoundary = new QSlider(Qt::Horizontal);
        dzRightCenter = new QSlider(Qt::Horizontal);
        dzRightBoundary = new QSlider(Qt::Horizontal);

        titleFont.setBold(true);
        title->setAlignment(Qt::AlignCenter);
        title->setFont(titleFont);
        dzLeftCenter->setRange(-10, 10);
        dzLeftBoundary->setRange(-10, 10);
        dzRightCenter->setRange(-10, 10);
        dzRightBoundary->setRange(-10, 10);
        lsIcon->setAlignment(Qt::AlignCenter);
        lsIcon->setPixmap(QPixmap(u":/icons/ls"_s).scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        rsIcon->setAlignment(Qt::AlignCenter);
        rsIcon->setPixmap(QPixmap(u":/icons/rs"_s).scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        lCenterLblLyt->addWidget(new QLabel(u"center"_s));
        lCenterLblLyt->addStretch();
        lCenterLblLyt->addWidget(leftCenterLbl);

        lBoundaryLblLyt->addWidget(new QLabel(u"boundary"_s));
        lBoundaryLblLyt->addStretch();
        lBoundaryLblLyt->addWidget(leftBoundaryLbl);

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

        rCenterLblLyt->addWidget(new QLabel(u"center"_s));
        rCenterLblLyt->addStretch();
        rCenterLblLyt->addWidget(rightCenterLbl);

        rBoundaryLblLyt->addWidget(new QLabel(u"boundary"_s));
        rBoundaryLblLyt->addStretch();
        rBoundaryLblLyt->addWidget(rightBoundaryLbl);

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

        settLyt->addLayout(leftLyt);
        settLyt->addSpacing(20);
        settLyt->addLayout(rightLyt);

        lyt->addWidget(title);
        lyt->addSpacing(8);
        lyt->addLayout(settLyt);
        lyt->addSpacing(20);

        QObject::connect(dzLeftCenter, &QSlider::valueChanged, leftCenterLbl, qOverload<int>(&QLabel::setNum));
        QObject::connect(dzLeftBoundary, &QSlider::valueChanged, leftBoundaryLbl, qOverload<int>(&QLabel::setNum));
        QObject::connect(dzRightCenter, &QSlider::valueChanged, rightCenterLbl, qOverload<int>(&QLabel::setNum));
        QObject::connect(dzRightBoundary, &QSlider::valueChanged, rightBoundaryLbl, qOverload<int>(&QLabel::setNum));

        return lyt;
    }

    void SettingsPage::refresh() const {
        if (controller->hasFeature(ControllerFeature::ShoulderLedsV1)) {
            const std::tuple<int, int, int> lcolor = controller->getLedColor();
            const QColor color = QColor(std::get<0>(lcolor), std::get<1>(lcolor), std::get<2>(lcolor));
            QPalette pal = ledColorLbl->palette();

            pal.setColor(QPalette::Window, color);
            ledColorLbl->setPalette(pal);
            ledMode->setCurrentIndex(static_cast<int>(controller->getLedMode()));
        }

        if (controller->hasFeature(ControllerFeature::RumbleV1))
            rumble->setCurrentIndex(static_cast<int>(controller->getRumbleMode()));

        if (controller->hasFeature(ControllerFeature::DeadZoneControlV1)) {
            dzLeftCenter->setValue(controller->getAnalogCenter(true));
            dzLeftBoundary->setValue(controller->getAnalogBoundary(true));
            dzRightCenter->setValue(controller->getAnalogCenter(false));
            dzRightBoundary->setValue(controller->getAnalogBoundary(false));
        }
    }

    void SettingsPage::writeSettings() const {
        if (controller->hasFeature(ControllerFeature::RumbleV1))
            controller->setRumble(static_cast<RumbleMode>(rumble->currentIndex()));

        if (controller->hasFeature(ControllerFeature::DeadZoneControlV1)) {
            controller->setAnalogCenter(dzLeftCenter->value(), true);
            controller->setAnalogBoundary(dzLeftBoundary->value(), true);
            controller->setAnalogCenter(dzRightCenter->value(), false);
            controller->setAnalogBoundary(dzRightBoundary->value(), false);
        }

        if (controller->hasFeature(ControllerFeature::ShoulderLedsV1)) {
            const QColor color = ledColorLbl->palette().color(QPalette::Window);

            controller->setLedMode(static_cast<LedMode>(ledMode->currentIndex()));
            controller->setLedColor(color.red(), color.green(), color.blue());
        }
    }

    void SettingsPage::onRestoreBtnClicked() {
        QMessageBox *mbox = new QMessageBox(this);
        const QPushButton *yesBtn = mbox->addButton(u"Yes"_s, QMessageBox::YesRole);
        QPushButton *noBtn = mbox->addButton(u"No"_s, QMessageBox::NoRole);

        mbox->setWindowTitle(u"Controller configuration reset"_s);
        mbox->setText(u"Reset controller configuration data?"_s);
        mbox->setDetailedText(u"Try to repair corrupted controller memory. (Only needed if something, not official OpenWinControls, messed it)"_s);
        mbox->setDefaultButton(noBtn);
        mbox->exec();

        if (mbox->clickedButton() == yesBtn) {
            if (!controller->resetConfig()) {
                QMessageBox::critical(this, u"Configuration reset"_s, u"Failed"_s);

            } else {
                QMessageBox::information(this, u"Configuration reset"_s, u"Success"_s);
                emit configRestore();
            }
        }

        mbox->deleteLater();
    }

    void SettingsPage::onBackBtnClicked() {
        emit backToHome();
    }

    void SettingsPage::onLedColorPickBtnClicked() {
        const QColor color = QColorDialog::getColor(Qt::white, this, u"Select led color"_s);

        if (!color.isValid())
            return;

        QPalette colorLblPal = ledColorLbl->palette();

        colorLblPal.setColor(QPalette::Window, color);
        ledColorLbl->setPalette(colorLblPal);
    }

    void SettingsPage::onLedModeChanged(const int idx) const {
        const bool hasColor = idx == 1 || idx == 2;

        ledColorLbl->setVisible(hasColor);
        ledColorPickBtn->setVisible(hasColor);
    }
}
