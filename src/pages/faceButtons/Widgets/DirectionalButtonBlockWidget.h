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

#include "ButtonBlockWidget.h"

namespace OWC {
    class DirectionalButtonBlockWidget final: public ButtonBlockWidget {
        Q_OBJECT

    protected:
        QPushButton *topBtn = nullptr;
        QPushButton *leftBtn = nullptr;
        QPushButton *rightBtn = nullptr;
        QPushButton *bottomBtn = nullptr;
        Button topOwcButton;
        Button leftOwcButton;
        Button rightOwcButton;
        Button bottomOwcButton;
        std::string_view topYmlKey;
        std::string_view leftYmlKey;
        std::string_view rightYmlKey;
        std::string_view bottomYmlKey;

    public:
        DirectionalButtonBlockWidget(Button topOwcBtn, Button leftOwcBtn, Button rightOwcBtn, Button bottomOwcBtn,
                                    std::string_view topYamlKey, std::string_view leftYamlKey,
                                    std::string_view rightYamlKey, std::string_view bottomYamlKey,
                                    const QString &icon, int iconScale);

        void setMapping(const QSharedPointer<Controller> &gpd) const override;
        void writeMapping(const QSharedPointer<Controller> &gpd) override;
        [[nodiscard]] QString exportMappingToYaml() const override;
        void importMappingFromYaml(const YAML::Node &yaml) const override;
    };
}
