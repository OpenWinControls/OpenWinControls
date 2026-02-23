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

#include "BackButtonsV2Page.h"
#include "../extern/libOpenWinControls/src/controller/ControllerV2.h"

namespace OWC {
    BackButtonsV2Page::BackButtonsV2Page(): BackButtonsPage(QStringLiteral("key slots, start times and hold times"), CharMapMode::Mixed) {
        activeSlotsL = new QSpinBox();
        activeSlotsR = new QSpinBox();

        activeSlotsL->setRange(0, 32);
        activeSlotsL->setValue(0);
        activeSlotsR->setRange(0, 32);
        activeSlotsR->setValue(0);

        backBtnLyt->addLayout(makeBackButtonUI("l4", activeSlotsL, lBtnList));
        backBtnLyt->addLayout(makeBackButtonUI("r4", activeSlotsR, rBtnList));

        QObject::connect(activeSlotsL, &QSpinBox::valueChanged, this, &BackButtonsV2Page::onActiveSlotsLChanged);
        QObject::connect(activeSlotsR, &QSpinBox::valueChanged, this, &BackButtonsV2Page::onActiveSlotsRChanged);
    }

    QVBoxLayout *BackButtonsV2Page::makeBackButtonUI(const QString &icon,  QSpinBox *activeSlotsInpt, QList<KeySlot> &slotList) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *iconLyt = new QHBoxLayout();
        QHBoxLayout *activeSlotsLyt = new QHBoxLayout();
        QLabel *lIcon = new QLabel();

        lIcon->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        iconLyt->setAlignment(Qt::AlignCenter);
        iconLyt->addWidget(lIcon);
        activeSlotsLyt->setAlignment(Qt::AlignCenter);
        activeSlotsLyt->addWidget(new QLabel("Active: "));
        activeSlotsLyt->addWidget(activeSlotsInpt);
        lyt->setAlignment(Qt::AlignCenter);
        lyt->addLayout(iconLyt);
        lyt->addSpacing(15);
        lyt->addLayout(activeSlotsLyt);
        lyt->addSpacing(8);

        for (int i=0; i<32; ++i) {
            QHBoxLayout *slotLyt = new QHBoxLayout();
            QPushButton *slot = new QPushButton();
            QSpinBox *time = new QSpinBox();
            QSpinBox *hold = new QSpinBox();

            slot->setFixedWidth(150);
            time->setRange(0, INT16_MAX - 1);
            hold->setRange(0, INT16_MAX - 1);
            slot->setEnabled(false);
            time->setEnabled(false);
            hold->setEnabled(false);

            slotLyt->setAlignment(Qt::AlignLeft);
            slotLyt->addWidget(new QLabel(QString::number(i + 1).rightJustified(2, '0')));
            slotLyt->addWidget(slot);
            slotLyt->addSpacing(6);
            slotLyt->addWidget(time);
            slotLyt->addWidget(new QLabel("ms"));
            slotLyt->addSpacing(4);
            slotLyt->addWidget(hold);
            slotLyt->addWidget(new QLabel("ms"));

            lyt->addLayout(slotLyt);

            slotList.append({.btn = slot, .startTime = time, .holdTime = hold});
            QObject::connect(slot, &QPushButton::clicked, this, &BackButtonsV2Page::onkeyButtonPressed);
        }

        return lyt;
    }

    void BackButtonsV2Page::enableSlots(const int count, const bool l4) const {
        int i = 0;

        for (const KeySlot &slot: (l4 ? lBtnList : rBtnList)) {
            const bool enable = i < count;

            slot.btn->setEnabled(enable);
            slot.startTime->setEnabled(enable);
            slot.holdTime->setEnabled(enable);
            ++i;
        }
    }

    void BackButtonsV2Page::setMapping(const QSharedPointer<Controller> &gpd) const {
        const QSharedPointer<OWC::ControllerV2> gpdV2 = qSharedPointerCast<OWC::ControllerV2>(gpd);

        for (int i=0; i<32; ++i) {
            lBtnList[i].btn->setText(QString::fromStdString(gpd->getBackButton(1, i+1)));
            lBtnList[i].startTime->setValue(gpd->getBackButtonStartTime(1, i+1));
            lBtnList[i].holdTime->setValue(gpdV2->getBackButtonHoldTime(1, i+1));
        }

        for (int i=0; i<32; ++i) {
            rBtnList[i].btn->setText(QString::fromStdString(gpd->getBackButton(2, i+1)));
            rBtnList[i].startTime->setValue(gpd->getBackButtonStartTime(2, i+1));
            rBtnList[i].holdTime->setValue(gpdV2->getBackButtonHoldTime(2, i+1));
        }

        activeSlotsL->setValue(gpdV2->getBackButtonActiveSlots(1));
        activeSlotsR->setValue(gpdV2->getBackButtonActiveSlots(2));
    }

    QString BackButtonsV2Page::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts(&yaml);

        for (int i=0; i<32; ++i) {
            ts << "L4_K" << (i+1) << ": " << lBtnList[i].btn->text() << "\n"
                "L4_K" << (i+1) << "_START_TIME: " << lBtnList[i].startTime->value() << "\n"
                "L4_K" << (i+1) << "_HOLD_TIME: " << lBtnList[i].holdTime->value() << "\n";
        }

        for (int i=0; i<32; ++i) {
            ts << "R4_K" << (i+1) << ": " << rBtnList[i].btn->text() << "\n"
                "R4_K" << (i+1) << "_START_TIME: " << rBtnList[i].startTime->value() << "\n"
                "R4_K" << (i+1) << "_HOLD_TIME: " << rBtnList[i].holdTime->value() << "\n";
        }

        ts << "L4_ACTIVE_SLOTS: " << activeSlotsL->value() << "\n"
            "R4_ACTIVE_SLOTS: " << activeSlotsR->value() << "\n";

        return yaml;
    }

    void BackButtonsV2Page::importMappingFromYaml(const YAML::Node &yaml) const {
        for (int i=0; i<32; ++i) {
            const std::string key = std::format("L4_K{}", i+1);
            const std::string time = std::format("L4_K{}_START_TIME", i+1);
            const std::string hold = std::format("L4_K{}_HOLD_TIME", i+1);

            if (yaml[key])
                lBtnList[i].btn->setText(QString::fromStdString(yaml[key].as<std::string>()).toUpper());

            if (yaml[time])
                lBtnList[i].startTime->setValue(yaml[time].as<int>());

            if (yaml[hold])
                lBtnList[i].holdTime->setValue(yaml[hold].as<int>());
        }

        for (int i=0; i<32; ++i) {
            const std::string key = std::format("R4_K{}", i+1);
            const std::string time = std::format("R4_K{}_START_TIME", i+1);
            const std::string hold = std::format("R4_K{}_HOLD_TIME", i+1);

            if (yaml[key])
                rBtnList[i].btn->setText(QString::fromStdString(yaml[key].as<std::string>()).toUpper());

            if (yaml[time])
                rBtnList[i].startTime->setValue(yaml[time].as<int>());

            if (yaml[hold])
                rBtnList[i].holdTime->setValue(yaml[hold].as<int>());
        }

        if (yaml["L4_ACTIVE_SLOTS"])
            activeSlotsL->setValue(std::clamp(yaml["L4_ACTIVE_SLOTS"].as<int>(), 0, 32));

        if (yaml["R4_ACTIVE_SLOTS"])
            activeSlotsR->setValue(std::clamp(yaml["R4_ACTIVE_SLOTS"].as<int>(), 0, 32));
    }

    void BackButtonsV2Page::writeMapping(const QSharedPointer<Controller> &gpd) {
        const QSharedPointer<OWC::ControllerV2> gpdV2 = qSharedPointerCast<OWC::ControllerV2>(gpd);

        for (int i=0; i<32; ++i) {
            if (!gpd->setBackButton(1, i+1, lBtnList[i].btn->text().toStdString()))
                emit logSent(QString("failed to set L4 key slot %1").arg(i+1));

            gpd->setBackButtonStartTime(1, i+1, lBtnList[i].startTime->value());
            gpdV2->setBackButtonHoldTime(1, i+1, lBtnList[i].holdTime->value());
        }

        for (int i=0; i<32; ++i) {
            if (!gpd->setBackButton(2, i+1, rBtnList[i].btn->text().toStdString()))
                emit logSent(QString("failed to set R4 key slot %1").arg(i+1));

            gpd->setBackButtonStartTime(2, i+1, rBtnList[i].startTime->value());
            gpdV2->setBackButtonHoldTime(2, i+1, rBtnList[i].holdTime->value());
        }

        gpdV2->setBackButtonActiveSlots(1, activeSlotsL->value());
        gpdV2->setBackButtonActiveSlots(2, activeSlotsR->value());
    }

    void BackButtonsV2Page::onActiveSlotsLChanged(const int val) const {
        enableSlots(val, true);
    }

    void BackButtonsV2Page::onActiveSlotsRChanged(const int val) const {
        enableSlots(val, false);
    }
}
