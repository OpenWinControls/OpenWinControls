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

#include "BackButtonV2Widget.h"

namespace OWC {
    BackButtonV2Widget::BackButtonV2Widget(const int buttonNum, const int maxKeySlots, const QString &icon) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *activeSlotsLyt = new QHBoxLayout();
        QLabel *iconLbl = new QLabel();

        name = icon.toUpper();
        num = buttonNum;
        activeSlots = new QSpinBox();

        activeSlots->setRange(0, maxKeySlots);
        activeSlots->setValue(0);

        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        activeSlotsLyt->setAlignment(Qt::AlignCenter);
        activeSlotsLyt->addWidget(new QLabel("Active: "));
        activeSlotsLyt->addWidget(activeSlots);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(iconLbl);
        lyt->addSpacing(15);
        lyt->addLayout(activeSlotsLyt);
        lyt->addSpacing(8);

        for (int i=1; i<=maxKeySlots; ++i) {
            KeySlotV2Widget *ks = new KeySlotV2Widget(i);

            ks->setEnabled(false);
            lyt->addWidget(ks);
            keySlots.append(ks);

            QObject::connect(ks, &KeySlotV2Widget::pendingEditBtn, this, &BackButtonV2Widget::onPendingEditBtn);
        }

        setLayout(lyt);

        QObject::connect(activeSlots, &QSpinBox::valueChanged, this, &BackButtonV2Widget::onActiveSlotsChanged);
    }

    void BackButtonV2Widget::setMapping(const QSharedPointer<ControllerV2> &gpd) const {
        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;

            keySlots[i]->setKey(QString::fromStdString(gpd->getBackButton(num, kslot)));
            keySlots[i]->setStartTime(gpd->getBackButtonStartTime(num, kslot));
            keySlots[i]->setHoldTime(gpd->getBackButtonHoldTime(num, kslot));
        }

        activeSlots->setValue(gpd->getBackButtonActiveSlots(num));
    }

    void BackButtonV2Widget::writeMapping(const QSharedPointer<ControllerV2> &gpd) {
        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;

            if (!gpd->setBackButton(num, kslot, keySlots[i]->getKey().toStdString()))
                emit logSent(QString("failed to set %1 key slot %2").arg(name, kslot));

            gpd->setBackButtonStartTime(num, kslot, keySlots[i]->getStartTime());
            gpd->setBackButtonHoldTime(num, kslot, keySlots[i]->getHoldTime());
        }

        gpd->setBackButtonActiveSlots(num, activeSlots->value());
    }

    QString BackButtonV2Widget::exportToYaml() const {
        QString yml;
        QTextStream ts(&yml);

        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;

            ts << name << "_K" << kslot << ": " << keySlots[i]->getKey() << "\n" <<
                name << "_K" << kslot << "_START_TIME: " << keySlots[i]->getStartTime() << "\n" <<
                name << "_K" << kslot << "_HOLD_TIME: " << keySlots[i]->getHoldTime() << "\n";
        }

        ts << name << "_ACTIVE_SLOTS: " << activeSlots->value() << "\n";
        return yml;
    }

    void BackButtonV2Widget::importFromYaml(const YAML::Node &yaml) const {
        const std::string bname = name.toStdString();
        const std::string activeKey = std::format("{}_ACTIVE_SLOTS", bname);

        for (int i=0,l=keySlots.length(); i<l; ++i) {
            const int kslot = i + 1;
            const std::string key = std::format("{}_K{}", bname, kslot);
            const std::string time = std::format("{}_K{}_START_TIME", bname, kslot);
            const std::string hold = std::format("{}_K{}_HOLD_TIME", bname, kslot);

            if (yaml[key])
                keySlots[i]->setKey(QString::fromStdString(yaml[key].as<std::string>()).toUpper());

            if (yaml[time])
                keySlots[i]->setStartTime(yaml[time].as<int>());

            if (yaml[hold])
                keySlots[i]->setHoldTime(yaml[hold].as<int>());
        }

        if (yaml[activeKey])
            activeSlots->setValue(std::clamp(yaml[activeKey].as<int>(), 0, 32));
    }

    void BackButtonV2Widget::onActiveSlotsChanged(const int val) const {
        for (int i=0,l=keySlots.length(); i<l; ++i)
            keySlots[i]->setEnabled(i < val);
    }

    void BackButtonV2Widget::onPendingEditBtn(QPushButton *keyBtn) {
        emit pendingEditBtn(keyBtn);
    }
}
