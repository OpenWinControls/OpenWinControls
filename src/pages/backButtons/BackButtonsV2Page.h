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
#include "BackButtonsPage.h"
#include "Widgets/BackButtonV2Widget.h"

namespace OWC {
    class BackButtonsV2Page final: public BackButtonsPage {
        Q_OBJECT

    private:
        std::shared_ptr<ControllerV2> controller;
        BackButtonV2Widget *l4 = nullptr;
        BackButtonV2Widget *r4 = nullptr;
        BackButtonV2Widget *r5 = nullptr;

    public:
        explicit BackButtonsV2Page(const std::shared_ptr<Controller> &gpd);

        void writeMapping() override;
        [[nodiscard]] QString exportToYaml() const override;
        void importFromYaml(const YAML::Node &yaml) const override;

    private slots:
        void onLogSent(const QString &msg);

    public slots:
        void refresh() const override;
    };
}
