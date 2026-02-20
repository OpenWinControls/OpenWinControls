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
#ifdef _WIN32
#include "include/win.h"
#endif
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pages/BackButtonsV1Page.h"
#include "pages/BackButtonsV2Page.h"
#include "version.h"
#include "include/GPDProducts.h"
#include "extern/libOpenWinControls/src/include/ControllerFeature.h"
#include "extern/libOpenWinControls/src/controller/ControllerV1.h"
#include "extern/libOpenWinControls/src/controller/ControllerV2.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(QString("%1 %2.%3").arg(APP_NAME).arg(APP_VER_MAJOR).arg(APP_VER_MINOR));
    setWindowIcon(QIcon(":/app/icon"));
    setMinimumSize(1040, 700);

    QVBoxLayout *lyt = new QVBoxLayout();
    QHBoxLayout *bottomLyt = new QHBoxLayout();
    QFont appFont = font();

    stackedWidget = new QStackedWidget();
    homePage = new OWC::HomePage();
    logsPage = new OWC::LogsPage();
    settingsPage = new OWC::SettingsPage();
    controllerVersionLbl = new QLabel("0.0");

    appFont.setPointSize(12);
    setFont(appFont);
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(logsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->setCurrentIndex(0);

    bottomLyt->addWidget(new QLabel("Controller version:"));
    bottomLyt->addWidget(controllerVersionLbl);
    bottomLyt->addStretch();
    bottomLyt->addWidget(new QLabel("kylon - GPLv3"));
    lyt->addWidget(stackedWidget);
    lyt->addLayout(bottomLyt);

    ui->centralwidget->setLayout(lyt);

    QObject::connect(homePage, &OWC::HomePage::keyboardMouseMap, this, &MainWindow::onHomeKeyboardMouseMapClicked);
    QObject::connect(homePage, &OWC::HomePage::xinputMap, this, &MainWindow::onHomeXinputMapClicked);
    QObject::connect(homePage, &OWC::HomePage::backButtonsMap, this, &MainWindow::onHomeBackButtonsMapClicked);
    QObject::connect(homePage, &OWC::HomePage::showLogs, this, &MainWindow::onHomeShowLogsClicked);
    QObject::connect(homePage, &OWC::HomePage::settingsPage, this, &MainWindow::onHomeSettingsPageClicked);
    QObject::connect(homePage, &OWC::HomePage::applyChanges, this, &MainWindow::onHomeApplyChanges);
    QObject::connect(homePage, &OWC::HomePage::exportYaml, this, &MainWindow::onHomeExportYamlClicked);
    QObject::connect(homePage, &OWC::HomePage::importYaml, this, &MainWindow::onHomeImportYamlClicked);
    QObject::connect(logsPage, &OWC::LogsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(settingsPage, &OWC::SettingsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(settingsPage, &OWC::SettingsPage::resetSettings, this, &MainWindow::onResetSettings);

    initApp();
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::getProduct() const {
#ifdef __linux__
    QFile prodF("/sys/class/dmi/id/board_name");

    if (!prodF.open(QFile::ReadOnly | QFile::Text)) {
        logsPage->writeLog(QString("failed to read product name: %1").arg(prodF.errorString()));
        return "";
    }

    const QString prod = prodF.readAll().trimmed();

    prodF.close();
    return prod;
#elif defined(_WIN32)
    DWORD bufSz = 0;
    std::unique_ptr<TCHAR[]> buf;
    LSTATUS ret;
    HKEY rkey;

    ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(R"(HARDWARE\DESCRIPTION\System\BIOS)"), 0, KEY_READ, &rkey);
    if (ret != ERROR_SUCCESS) {
        logsPage->writeLog(QString("failed to open bios subkey, code: %1").arg(ret));
        return "";
    }

    ret = RegGetValue(rkey, nullptr, TEXT("BaseBoardProduct"), RRF_RT_REG_SZ, nullptr, nullptr, &bufSz);
    if (ret != ERROR_SUCCESS) {
        logsPage->writeLog(QString("failed to read size for reg value, code %1").arg(ret));
        RegCloseKey(rkey);
        return "";
    }

    bufSz += sizeof(TCHAR);
    buf = std::make_unique<TCHAR[]>(bufSz);

    ret = RegGetValue(rkey, nullptr, TEXT("BaseBoardProduct"), RRF_RT_REG_SZ, nullptr, buf.get(), &bufSz);
    if (ret != ERROR_SUCCESS) {
        logsPage->writeLog(QString("failed to read reg value, code %1").arg(ret));
        RegCloseKey(rkey);
        return "";
    }

    RegCloseKey(rkey);
    return QString(buf.get());
#else
    return "";
#endif
}

QSharedPointer<OWC::Controller> MainWindow::getDevice(const QString &product) const {
    QSharedPointer<OWC::Controller> device;

    if (product == OWC::win4)
        device = QSharedPointer<OWC::ControllerV1>::create(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::ShoulderLedsV1 | OWC::ControllerFeature::RumbleV1);
    else if (product == OWC::mini23 || product == OWC::mini24 || product == OWC::max2_22 || product == OWC::max2_25)
        device = QSharedPointer<OWC::ControllerV1>::create(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::RumbleV1);
    /*else if (product == OWC::win3)
        device = QSharedPointer<OWC::ControllerV1>::create();*/
    else if (product == OWC::win5)
        device = QSharedPointer<OWC::ControllerV2>::create(OWC::ControllerFeature::RumbleV1 | OWC::ControllerFeature::XinputMappingV1);
    else if (product == OWC::mini25)
        device = QSharedPointer<OWC::ControllerV2>::create(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::RumbleV1 | OWC::ControllerFeature::XinputMappingV1);
    else
        logsPage->writeLog(QString("unknown device: %1").arg(product));

    return device;
}

bool MainWindow::isCompatible(const QString &product) const {
    std::pair<int, int> version = {0, 0};
    bool compCheck = false;

    /*if (product == OWC::win3) {
       return true;

   } else*/ if (product == OWC::win4) {
       version = qSharedPointerCast<OWC::ControllerV1>(gpd)->getKVersion();
       compCheck = version.first >= 4 && version.second >= 7;

    } else if (product == OWC::mini23 || product == OWC::mini24) {
        version = qSharedPointerCast<OWC::ControllerV1>(gpd)->getKVersion();
        compCheck = version.first >= 5 && version.second >= 3;

    } else if (product == OWC::max2_22 || product == OWC::max2_25) {
        version = qSharedPointerCast<OWC::ControllerV1>(gpd)->getKVersion();
        compCheck = version.first >= 1 && version.second >= 23;

    } else if (product == OWC::win5) {
        version = qSharedPointerCast<OWC::ControllerV2>(gpd)->getVersion();
        compCheck = version.first >= 1 && version.second >= 8;

    } else if (product == OWC::mini25) {
        version = qSharedPointerCast<OWC::ControllerV2>(gpd)->getVersion();
        compCheck = version.first >= 1 && version.second >= 22;
    }

    if (!compCheck)
        logsPage->writeLog(QString("version %1.%2 is not supported").arg(version.first).arg(version.second));

    return compCheck;
}

void MainWindow::initApp() {
    const std::function<void(const std::wstring &)> logCB = [&](const std::wstring &msg) { logsPage->writeLog(QString::fromStdWString(msg)); };
    const QString prod = getProduct();

    gpd = getDevice(prod);
    if (gpd.isNull())
        return;

    gpd->enableLogging(logCB);

    if (!gpd->init()) {
        logsPage->writeLog(QStringLiteral("device initialization failed"));
        return;

    } else if (!gpd->readConfig()) {
        logsPage->writeLog(QStringLiteral("failed to read firmware config"));
        return;

    } else if (!isCompatible(prod)) {
        logsPage->writeLog(QStringLiteral("no compatible controller found"));
        return;
    }

    if (gpd->getControllerType() == 1) {
        const QSharedPointer<OWC::ControllerV1> gpdV1 = qSharedPointerCast<OWC::ControllerV1>(gpd);
        const auto [xmin, xmax] = gpdV1->getXVersion();
        const auto [kmin, kmax] = gpdV1->getKVersion();

        backButtonsPage = new OWC::BackButtonsV1Page();

        controllerVersionLbl->setText(QString("X%1.%2, K%3.%4").arg(xmin).arg(xmax).arg(kmin).arg(kmax));

    } else if (gpd->getControllerType() == 2) {
        const QSharedPointer<OWC::ControllerV2> gpdV2 = qSharedPointerCast<OWC::ControllerV2>(gpd);
        const auto [major, minor] = gpdV2->getVersion();

        backButtonsPage = new OWC::BackButtonsV2Page();
        xinputPage = new OWC::XinputButtonsPage();
        xinputPageIdx = stackedWidget->addWidget(xinputPage);

        controllerVersionLbl->setText(QString("%1.%2").arg(major).arg(minor));
        xinputPage->setMapping(gpd);
        homePage->setEmulationMode(gpdV2->getEmulationMode());

        QObject::connect(xinputPage, &OWC::FaceButtonsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
        QObject::connect(xinputPage, &OWC::XinputButtonsPage::resetXinputButtons, this, &MainWindow::onResetXinputButtons);
        QObject::connect(xinputPage, &OWC::FaceButtonsPage::logSent, this, &MainWindow::onLogSent);
    }

    kbdMousePage = new OWC::KeyboardMouseButtonsPage();
    keyboardMousePageIdx = stackedWidget->addWidget(kbdMousePage);
    backButtonsPageIdx = stackedWidget->addWidget(backButtonsPage);

    kbdMousePage->setMapping(gpd);
    backButtonsPage->setMapping(gpd);
    settingsPage->initPage(gpd);
    settingsPage->setData(gpd);
    homePage->setDevice(prod);

    QObject::connect(kbdMousePage, &OWC::FaceButtonsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(kbdMousePage, &OWC::KeyboardMouseButtonsPage::resetKeyboardMouseButtons, this, &MainWindow::onResetKeyboardMouseButtons);
    QObject::connect(kbdMousePage, &OWC::FaceButtonsPage::logSent, this, &MainWindow::onLogSent);
    QObject::connect(backButtonsPage, &OWC::BackButtonsV1Page::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(backButtonsPage, &OWC::BackButtonsV1Page::resetBackButtons, this, &MainWindow::onResetBackButtons);
    QObject::connect(backButtonsPage, &OWC::BackButtonsV1Page::logSent, this, &MainWindow::onLogSent);
}

void MainWindow::onLogSent(const QString& msg) const {
    logsPage->writeLog(msg);
}

void MainWindow::onHomeKeyboardMouseMapClicked() const {
    stackedWidget->setCurrentIndex(keyboardMousePageIdx);
}

void MainWindow::onHomeXinputMapClicked() const {
    stackedWidget->setCurrentIndex(xinputPageIdx);
}

void MainWindow::onHomeBackButtonsMapClicked() const {
    stackedWidget->setCurrentIndex(backButtonsPageIdx);
}

void MainWindow::onHomeShowLogsClicked() const {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::onHomeSettingsPageClicked() const {
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::onHomeApplyChanges() {
    homePage->enableButtons(false);
    kbdMousePage->writeMapping(gpd);
    backButtonsPage->writeMapping(gpd);
    settingsPage->writeSettings(gpd);

    if (xinputPage != nullptr)
        xinputPage->writeMapping(gpd);

    if (!gpd->writeConfig())
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Unable to write controller!"));

    homePage->enableButtons(true);
}

void MainWindow::onHomeExportYamlClicked() {
    const QString out = QFileDialog::getSaveFileName(this, "Export mapping to file", "", "Yaml (*.yaml)");

    if (out.isEmpty())
        return;

    QFile outF(out);
    QTextStream ts(&outF);

    if (!outF.open(QFile::WriteOnly | QFile::Text)) {
        logsPage->writeLog(QString("failed to export mapping: %1").arg(outF.errorString()));
        return;
    }

    ts << "MAPPING_TYPE: " << gpd->getControllerType() << "\n" <<
        kbdMousePage->exportMappingToYaml() <<
        backButtonsPage->exportMappingToYaml();

    if (xinputPage != nullptr)
        ts << xinputPage->exportMappingToYaml();

    ts.flush();
    outF.close();
    logsPage->writeLog(QString("exported mapping to file: %1").arg(out));
}

void MainWindow::onHomeImportYamlClicked() {
    const QString map = QFileDialog::getOpenFileName(this, "Import mapping from file", "", "Yaml (*.yaml)");

    if (map.isEmpty())
        return;

    QFile mappF(map);

    if (!mappF.open(QFile::ReadOnly | QFile::Text)) {
        logsPage->writeLog(QString("failed to import mapping: %1").arg(mappF.errorString()));
        return;
    }

    try {
        const QByteArray content = mappF.readAll();
        const YAML::Node yaml = YAML::Load(content.toStdString());

        if (!yaml.IsMap()) {
            logsPage->writeLog("Imported file does not contain a valid yaml map");
            return;
        }

        const int type = yaml["MAPPING_TYPE"].as<int>();

        if (gpd->getControllerType() != type) {
            logsPage->writeLog(QString("incompatible mapping type, cannot import %1").arg(type));
            return;
        }

        mappF.close();
        kbdMousePage->importMappingFromYaml(yaml);
        backButtonsPage->importMappingFromYaml(yaml);

        if (xinputPage != nullptr)
            xinputPage->importMappingFromYaml(yaml);

    } catch (const YAML::ParserException &e) {
        logsPage->writeLog(e.what());
        mappF.close();
        return;
    }

    logsPage->writeLog(QString("imported mapping from file: %1").arg(map));
}

void MainWindow::onBackToHomeClicked() const {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::onResetKeyboardMouseButtons() const {
    kbdMousePage->setMapping(gpd);
}

void MainWindow::onResetXinputButtons() const {
    xinputPage->setMapping(gpd);
}

void MainWindow::onResetBackButtons() const {
    backButtonsPage->setMapping(gpd);
}

void MainWindow::onResetSettings() const {
    settingsPage->setData(gpd);
}
