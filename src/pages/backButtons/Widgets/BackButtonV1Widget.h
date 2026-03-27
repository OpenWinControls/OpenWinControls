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

#include "KeySlotV1Widget.h"
#include "../../../extern/libOpenWinControls/src/controller/Controller.h"
#include "../../../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class BackButtonV1Widget final: public QWidget {
        Q_OBJECT

    private:
        QSpinBox *macroStartTime = nullptr;
        QList<KeySlotV1Widget *> keySlots;
        QString name;
        int num;

    public:
        BackButtonV1Widget(int buttonNum, int maxKeySlots, const QString &icon);

        void setMapping(const QSharedPointer<Controller> &gpd) const;
        void writeMapping(const QSharedPointer<Controller> &gpd);
        [[nodiscard]] QString exportToYaml() const;
        void importFromYaml(const YAML::Node &yaml) const;

    signals:
        void logSent(const QString &msg);
    };
}
