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
    class ShoulderButtonBlockWidget final: public ButtonBlockWidget {
        Q_OBJECT

    private:
        QPushButton *topBtn = nullptr;
        QPushButton *bottomBtn = nullptr;
        Button topOwcButton;
        Button bottomOwcButton;
        std::string_view topYmlKey;
        std::string_view bottomYmlKey;

    public:
        ShoulderButtonBlockWidget(Button topOwcBtn, Button bottomOwcBtn,
                                    std::string_view topYamlKey, std::string_view bottomYamlKey,
                                    const QString &topIcon, const QString &bottomIcon,
                                    int topIconScaleW, int topIconScaleH,
                                    int bottomIconScaleW, int bottomIconScaleH);

        void setMapping(const QSharedPointer<Controller> &gpd) const override;
        void writeMapping(const QSharedPointer<Controller> &gpd) override;
        [[nodiscard]] QString exportMappingToYaml() const override;
        void importMappingFromYaml(const YAML::Node &yaml) const override;
    };
}
