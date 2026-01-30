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

#include <QMainWindow>
#include <QStackedWidget>

#include "pages/HomePage.h"
#include "pages/FaceButtonsPage.h"
#include "pages/BackButtonsPage.h"
#include "pages/LogsPage.h"
#include "pages/SettingsPage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final: public QMainWindow {
    Q_OBJECT

private:
    QStackedWidget *stackedWidget = nullptr;
    OWC::HomePage *homePage = nullptr;
    OWC::FaceButtonsPage *faceButtonsPage = nullptr;
    OWC::BackButtonsPage *backButtonsPage = nullptr;
    OWC::LogsPage *logsPage = nullptr;
    OWC::SettingsPage *settingsPage = nullptr;
    QLabel *controllerVersionLbl = nullptr;
    QSharedPointer<OWC::Controller> gpd;
    Ui::MainWindow *ui;

    [[nodiscard]] QString getProduct() const;
    [[nodiscard]] QSharedPointer<OWC::Controller> getDevice(const QString &product) const;
    [[nodiscard]] bool isCompatible(const QString &product) const;
    void initController(const QString &product);
    void initUI(const QString &product) const;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLogSent(const QString &msg) const;
    void onHomeFaceButtonsMapClicked() const;
    void onHomeBackButtonsMapClicked() const;
    void onHomeShowLogsClicked() const;
    void onHomeSettingsPageClicked() const;
    void onHomeApplyChanges();
    void onHomeExportYamlClicked();
    void onHomeImportYamlClicked();
    void onBackToHomeClicked() const;
    void onResetFaceButtons() const;
    void onResetBackButtons() const;
    void onResetSettings() const;
};
