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

#include "BackButtonsV1Page.h"

namespace OWC {
    BackButtonsV1Page::BackButtonsV1Page(): BackButtonsPage(QStringLiteral("macro key slots and start times")) {
        backBtnLyt->addLayout(makeBackButtonUI("l4", lBtnList));
        backBtnLyt->addLayout(makeBackButtonUI("r4", rBtnList));
    }

    QVBoxLayout *BackButtonsV1Page::makeBackButtonUI(const QString &icon, QList<KeySlot> &slotList) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *iconLyt = new QHBoxLayout();
        QHBoxLayout *macroTimeLyt = new QHBoxLayout();
        QLabel *lIcon = new QLabel();

        lIcon->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        iconLyt->setAlignment(Qt::AlignCenter);
        iconLyt->addWidget(lIcon);
        lyt->setAlignment(Qt::AlignCenter);
        lyt->addLayout(iconLyt);
        lyt->addSpacing(15);

        for (int i=0; i<4; ++i) {
            QHBoxLayout *slotLyt = new QHBoxLayout();
            QPushButton *slot = new QPushButton();
            QSpinBox *time = new QSpinBox();

            slot->setFixedWidth(150);
            time->setRange(0, INT16_MAX - 1);

            slotLyt->setAlignment(Qt::AlignLeft);
            slotLyt->addWidget(new QLabel(QString::number(i + 1)));
            slotLyt->addWidget(slot);

            if (i < 3) {
                slotLyt->addSpacing(6);
                slotLyt->addWidget(time);
                slotLyt->addWidget(new QLabel("ms"));
            }

            lyt->addLayout(slotLyt);
            slotList.append({.btn = slot, .startTime = time});
            QObject::connect(slot, &QPushButton::clicked, this, &BackButtonsV1Page::onkeyButtonPressed);
        }

        macroTimeLyt->addWidget(new QLabel("Macro start time"));
        macroTimeLyt->addSpacing(10);
        macroTimeLyt->addWidget(slotList[3].startTime);
        macroTimeLyt->addWidget(new QLabel("ms"));
        lyt->addSpacing(25);
        lyt->addLayout(macroTimeLyt);

        return lyt;
    }

    void BackButtonsV1Page::setMapping(const QSharedPointer<Controller> &gpd) const {
        for (int i=0; i<4; ++i) {
            lBtnList[i].btn->setText(QString::fromStdString(gpd->getBackButton(1, i+1)));
            lBtnList[i].startTime->setValue(gpd->getBackButtonStartTime(1, i+1));
        }

        for (int i=0; i<4; ++i) {
            rBtnList[i].btn->setText(QString::fromStdString(gpd->getBackButton(2, i+1)));
            rBtnList[i].startTime->setValue(gpd->getBackButtonStartTime(2, i+1));
        }
    }

    QString BackButtonsV1Page::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts(&yaml);

        for (int i=0; i<4; ++i) {
            ts << "L4_K" << (i+1) << ": " << lBtnList[i].btn->text() << "\n";

            if (i < 3)
                ts << "L4_K" << (i+1) << "_START_TIME: " << lBtnList[i].startTime->value() << "\n";
        }

        for (int i=0; i<4; ++i) {
            ts << "R4_K" << (i+1) << ": " << rBtnList[i].btn->text() << "\n";

            if (i < 3)
                ts << "R4_K" << (i+1) << "_START_TIME: " << rBtnList[i].startTime->value() << "\n";
        }

        ts << "L4_MACRO_START_TIME: " << lBtnList[3].startTime->value() << "\n"
            "R4_MACRO_START_TIME: " << rBtnList[3].startTime->value() << "\n";

        return yaml;
    }

    void BackButtonsV1Page::importMappingFromYaml(const YAML::Node &yaml) const {
        for (int i=0; i<4; ++i) {
            const std::string key = std::format("L4_K{}", i+1);

            if (yaml[key])
                lBtnList[i].btn->setText(QString::fromStdString(yaml[key].as<std::string>()).toUpper());

            if (i < 3) {
                const std::string time = std::format("L4_K{}_START_TIME", i+1);

                if (yaml[time])
                    lBtnList[i].startTime->setValue(yaml[time].as<int>());
            }
        }

        for (int i=0; i<4; ++i) {
            const std::string key = std::format("R4_K{}", i+1);

            if (yaml[key])
                rBtnList[i].btn->setText(QString::fromStdString(yaml[key].as<std::string>()).toUpper());

            if (i < 3) {
                const std::string time = std::format("R4_K{}_START_TIME", i+1);

                if (yaml[time])
                    rBtnList[i].startTime->setValue(yaml[time].as<int>());
            }
        }

        if (yaml["L4_MACRO_START_TIME"])
            lBtnList[3].startTime->setValue(yaml["L4_MACRO_START_TIME"].as<int>());

        if (yaml["R4_MACRO_START_TIME"])
            rBtnList[3].startTime->setValue(yaml["R4_MACRO_START_TIME"].as<int>());
    }

    void BackButtonsV1Page::writeMapping(const QSharedPointer<Controller> &gpd) {
        for (int i=0; i<4; ++i) {
            if (!gpd->setBackButton(1, i+1, lBtnList[i].btn->text().toStdString()))
                emit logSent(QString("failed to set L4 key slot %1").arg(i+1));

            gpd->setBackButtonStartTime(1, i+1, lBtnList[i].startTime->value());
        }

        for (int i=0; i<4; ++i) {
            if (!gpd->setBackButton(2, i+1, rBtnList[i].btn->text().toStdString()))
                emit logSent(QString("failed to set R4 key slot %1").arg(i+1));

            gpd->setBackButtonStartTime(2, i+1, rBtnList[i].startTime->value());
        }
    }
}
