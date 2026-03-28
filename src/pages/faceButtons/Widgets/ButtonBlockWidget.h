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

#include <QPushButton>

#include "../../../extern/libOpenWinControls/src/controller/Controller.h"
#include "../../../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class ButtonBlockWidget: public QWidget {
        Q_OBJECT

    protected:
        static constexpr int buttonWidth = 122;

    public:
        virtual void setMapping(const QSharedPointer<Controller> &gpd) const = 0;
        virtual void writeMapping(const QSharedPointer<Controller> &gpd) = 0;
        [[nodiscard]] virtual QString exportMappingToYaml() const = 0;
        virtual void importMappingFromYaml(const YAML::Node &yaml) const = 0;

    protected slots:
        void onKeyBtnPressed() {
            emit pendingEditBtn(qobject_cast<QPushButton *>(QObject::sender()));
        }

    signals:
        void logSent(const QString &msg);
        void pendingEditBtn(QPushButton *keyBtn);
    };
}
