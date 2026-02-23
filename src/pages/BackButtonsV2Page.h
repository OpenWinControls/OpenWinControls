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

#include <QSpinBox>

#include "BackButtonsPage.h"

namespace OWC {
    class BackButtonsV2Page final: public BackButtonsPage {
        Q_OBJECT

    private:
        struct KeySlot final {
            QPushButton *btn = nullptr;
            QSpinBox *startTime = nullptr;
            QSpinBox *holdTime = nullptr;
        };

        QSpinBox *activeSlotsL = nullptr;
        QSpinBox *activeSlotsR = nullptr;
        QList<KeySlot> lBtnList;
        QList<KeySlot> rBtnList;

        [[nodiscard]] QVBoxLayout *makeBackButtonUI(const QString &icon, QSpinBox *activeSlotsInpt, QList<KeySlot> &slotList);
        void enableSlots(int count, bool l4) const;

    public:
        BackButtonsV2Page();

        void setMapping(const QSharedPointer<Controller> &gpd) const override;
        [[nodiscard]] QString exportMappingToYaml() const override;
        void importMappingFromYaml(const YAML::Node &yaml) const override;
        void writeMapping(const QSharedPointer<Controller> &gpd) override;

    private slots:
        void onActiveSlotsLChanged(int val) const;
        void onActiveSlotsRChanged(int val) const;
    };
}
