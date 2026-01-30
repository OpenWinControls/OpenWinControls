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

#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "../extern/libOpenWinControls/src/controller/Controller.h"

namespace OWC {
    class SettingsPage: public QWidget {
        Q_OBJECT

    private:
        QVBoxLayout *lyt = nullptr;
        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QComboBox *rumble = nullptr;
        QSlider *dzLeftCenter = nullptr;
        QLabel *dzLeftCenterLbl = nullptr;
        QSlider *dzLeftBoundary = nullptr;
        QLabel *dzLeftBoundaryLbl = nullptr;
        QSlider *dzRightCenter = nullptr;
        QLabel *dzRightCenterLbl = nullptr;
        QSlider *dzRightBoundary = nullptr;
        QLabel *dzRightBoundaryLbl = nullptr;
        QComboBox *ledMode = nullptr;
        QLabel *ledColorLbl = nullptr;
        QPushButton *ledColorChooserBtn = nullptr;

        void makeRumbleV1Settings();
        void makeShoulderLedsV1Settings();
        void makeDeadzoneV1Settings();

    public:
        SettingsPage();

        void initPage(const QSharedPointer<Controller> &gpd);
        void setData(const QSharedPointer<Controller> &gpd) const;
        void writeSettings(const QSharedPointer<Controller> &gpd) const;

    private slots:
        void onBackBtnClicked();
        void onResetBtnClicked();
        void onLedColorChooserBtnClicked();
        void onLedModeChanged(int idx) const;
        void onDzLeftCenterChanged(int v) const;
        void onDzLeftBoundaryChanged(int v) const;
        void onDzRightCenterChanged(int v) const;
        void onDzRightBoundaryChanged(int v) const;

    signals:
        void backToHome();
        void resetSettings();
    };
}
