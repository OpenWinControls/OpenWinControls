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
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "../extern/libOpenWinControls/src/controller/Controller.h"

namespace OWC {
    class SettingsPage final: public QWidget {
        Q_OBJECT

    private:
        std::shared_ptr<Controller> controller;
        QComboBox *rumble = nullptr;
        QSlider *dzLeftCenter = nullptr;
        QSlider *dzLeftBoundary = nullptr;
        QSlider *dzRightCenter = nullptr;
        QSlider *dzRightBoundary = nullptr;
        QComboBox *ledMode = nullptr;
        QLabel *ledColorLbl = nullptr;
        QPushButton *ledColorPickBtn = nullptr;

        [[nodiscard]] QVBoxLayout *makeRumbleV1();
        [[nodiscard]] QVBoxLayout *makeShoulderLedsV1();
        [[nodiscard]] QVBoxLayout *makeDeadzoneV1();

    public:
        explicit SettingsPage(const std::shared_ptr<Controller> &gpd);

        void writeSettings() const;

    private slots:
        void onRestoreBtnClicked();
        void onBackBtnClicked();
        void onLedColorPickBtnClicked();
        void onLedModeChanged(int idx) const;

    public slots:
        void refresh() const;

    signals:
        void backToHome();
        void configRestore();
    };
}
