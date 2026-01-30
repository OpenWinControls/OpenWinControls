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
#include <QLabel>

namespace OWC {
    class HomePage final: public QWidget {
        Q_OBJECT

    private:
        QPushButton *faceButtonsMapBtn = nullptr;
        QPushButton *backButtonsMapBtn = nullptr;
        QPushButton *showLogsBtn = nullptr;
        QPushButton *settingsBtn = nullptr;
        QPushButton *applyBtn = nullptr;
        QPushButton *exportYamlBtn = nullptr;
        QPushButton *importYamlBtn = nullptr;
        QLabel *frontPic = nullptr;
        QLabel *backPic = nullptr;

    public:
        HomePage();

        void setDevice(const QString &product) const;
        void enableButtons(bool enable) const;

    private slots:
        void onFaceButtonsMapClicked();
        void onBackButtonsMapClicked();
        void onShowLogsBtnClicked();
        void onSettingsClicked();
        void onApplyBtnClicked();
        void onExportYamlBtnClicked();
        void onImportYamlBtnClicked();

    signals:
        void faceButtonsMap();
        void backButtonsMap();
        void showLogs();
        void settingsPage();
        void applyChanges();
        void exportYaml();
        void importYaml();
    };
}
