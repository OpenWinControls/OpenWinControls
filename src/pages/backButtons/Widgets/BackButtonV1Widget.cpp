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
#include <QLabel>

#include "BackButtonV1Widget.h"

namespace OWC {
    BackButtonV1Widget::BackButtonV1Widget(const int buttonNum, const int maxKeySlots, const QString &icon) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *macroTimeLyt = new QHBoxLayout();
        QLabel *iconLbl = new QLabel();

        name = icon.toUpper();
        num = buttonNum;
        macroStartTime = new QSpinBox();

        macroStartTime->setRange(0, INT16_MAX - 1);

        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        macroTimeLyt->addWidget(new QLabel("Macro start time:"));
        macroTimeLyt->addSpacing(10);
        macroTimeLyt->addWidget(macroStartTime);
        macroTimeLyt->addWidget(new QLabel("ms"));

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(iconLbl);
        lyt->addSpacing(15);

        for (int i=1; i<=maxKeySlots; ++i) {
            KeySlotV1Widget *ks = new KeySlotV1Widget(i);

            lyt->addWidget(ks);
            keySlots.append(ks);
        }

        lyt->addSpacing(25);
        lyt->addLayout(macroTimeLyt);

        setLayout(lyt);
    }

    void BackButtonV1Widget::setMapping(const QSharedPointer<Controller> &gpd) const {
        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;

            keySlots[i]->setKey(QString::fromStdString(gpd->getBackButton(num, kslot)));
            keySlots[i]->setStartTime(gpd->getBackButtonStartTime(num, kslot));
        }

        macroStartTime->setValue(gpd->getBackButtonStartTime(num, 4));
    }

    void BackButtonV1Widget::writeMapping(const QSharedPointer<Controller> &gpd) {
        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;

            if (!gpd->setBackButton(num, kslot, keySlots[i]->getKey().toStdString()))
                emit logSent(QString("failed to set %1 key slot %2").arg(name, kslot));

            gpd->setBackButtonStartTime(num, kslot, keySlots[i]->getStartTime());
        }

        gpd->setBackButtonStartTime(num, 4, macroStartTime->value());
    }

    QString BackButtonV1Widget::exportToYaml() const {
        QString yml;
        QTextStream ts(&yml);

        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;

            ts << name << "_K" << kslot << ": " << keySlots[i]->getKey() << "\n" <<
                name << "_K" << kslot << "_START_TIME: " << keySlots[i]->getStartTime() << "\n";
        }

        ts << name << "_MACRO_START_TIME: " << macroStartTime->value() << "\n";

        return yml;
    }

    void BackButtonV1Widget::importFromYaml(const YAML::Node &yaml) const {
        const std::string bname = name.toStdString();
        const std::string macroTimeKey = std::format("{}_MACRO_START_TIME", bname);

        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;
            const std::string key = std::format("{}_K{}", bname, kslot);
            const std::string time = std::format("{}_K{}_START_TIME", bname, kslot);

            if (yaml[key])
                keySlots[i]->setKey(QString::fromStdString(yaml[key].as<std::string>()).toUpper());

            if (yaml[time])
                keySlots[i]->setStartTime(yaml[time].as<int>());
        }

        if (yaml[macroTimeKey])
            macroStartTime->setValue(yaml[macroTimeKey].as<int>());
    }
}
