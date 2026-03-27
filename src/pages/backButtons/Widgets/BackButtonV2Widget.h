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
#pragma once

#include <QList>

#include "KeySlotV2Widget.h"
#include "../../../extern/libOpenWinControls/src/controller/ControllerV2.h"
#include "../../../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class BackButtonV2Widget final: public QWidget {
        Q_OBJECT

    private:
        QSpinBox *activeSlots = nullptr;
        QList<KeySlotV2Widget *> keySlots;
        QString name;
        int num;

    public:
        BackButtonV2Widget(int buttonNum, int maxKeySlots, const QString &icon);

        void setMapping(const QSharedPointer<ControllerV2> &gpd) const;
        void writeMapping(const QSharedPointer<ControllerV2> &gpd);
        [[nodiscard]] QString exportToYaml() const;
        void importFromYaml(const YAML::Node &yaml) const;

    private slots:
        void onActiveSlotsChanged(int val) const;
        void onPendingEditBtn(QPushButton *keyBtn);

    signals:
        void logSent(const QString &msg);
        void pendingEditBtn(QPushButton *keyBtn);
    };
}
