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
#include <QStandardPaths>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pages/backButtons/BackButtonsV1Page.h"
#include "pages/backButtons/BackButtonsV2Page.h"
#include "version.h"
#include "include/GPDProducts.h"
#include "extern/libOpenWinControls/src/include/ControllerFeature.h"
#include "extern/libOpenWinControls/src/controller/ControllerV1.h"
#include "extern/libOpenWinControls/src/controller/ControllerV2.h"
#include "extern/SDL/include/SDL3/SDL_events.h"

using namespace Qt::StringLiterals;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(QString("%1 %2.%3").arg(APP_NAME).arg(APP_VER_MAJOR).arg(APP_VER_MINOR));
    setWindowIcon(QIcon(u":/app/icon"_s));

    QVBoxLayout *lyt = new QVBoxLayout();
    QHBoxLayout *bottomLyt = new QHBoxLayout();
    QLabel *repoLinkLbl = new QLabel(u"([sources](https://github.com/OpenWinControls/OpenWinControls))"_s);
    QFont appFont = font();

    appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    stackedWidget = new QStackedWidget();
    homePage = new OWC::HomePage();
    logsPage = new OWC::LogsPage();
    settingsPage = new OWC::SettingsPage();
    controllerVersionLbl = new QLabel(u"0.0"_s);

    appFont.setPointSize(12);
    setFont(appFont);
    repoLinkLbl->setTextFormat(Qt::MarkdownText);
    repoLinkLbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    repoLinkLbl->setOpenExternalLinks(true);
    repoLinkLbl->setAlignment(Qt::AlignCenter);
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(logsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->setCurrentIndex(0);

    if (!QDir().exists(appDataPath) && !QDir().mkdir(appDataPath)) {
        logsPage->writeLog(QString("failed to create data folder: %1").arg(appDataPath));
        appDataPath.clear();

    } else {
        logsPage->writeLog(QString("data path: %1").arg(appDataPath));
    }

    bottomLyt->addWidget(new QLabel(u"Controller version:"_s));
    bottomLyt->addWidget(controllerVersionLbl);
    bottomLyt->addStretch();
    bottomLyt->addWidget(new QLabel(u"kylon - GPLv3"_s));
    bottomLyt->addWidget(repoLinkLbl);
    lyt->addWidget(stackedWidget);
    lyt->addLayout(bottomLyt);

    ui->centralwidget->setLayout(lyt);

    QObject::connect(homePage, &OWC::HomePage::showLogs, this, &MainWindow::onHomeShowLogsClicked);
    QObject::connect(logsPage, &OWC::LogsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(settingsPage, &OWC::SettingsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(settingsPage, &OWC::SettingsPage::resetSettings, this, &MainWindow::onResetSettings);
    QObject::connect(settingsPage, &OWC::SettingsPage::configReset, this, &MainWindow::onSettingsConfigResetClicked);

    initApp();
}

MainWindow::~MainWindow() {
    if (gamepadThread != nullptr)
        quitGamepadThread();

    delete ui;
}

QString MainWindow::getProduct() const {
#ifdef __linux__
    QFile prodF(u"/sys/class/dmi/id/board_name"_s);

    if (!prodF.open(QFile::ReadOnly | QFile::Text)) {
        logsPage->writeLog(QString("failed to read product name: %1").arg(prodF.errorString()));
        return "";
    }

    const QString prod = prodF.readAll().trimmed();

    prodF.close();
    return prod;
#elif defined(_WIN32)
    DWORD bufSz = 0;
    std::unique_ptr<wchar_t[]> buf;
    LSTATUS ret;
    HKEY rkey;

    ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, LR"(HARDWARE\DESCRIPTION\System\BIOS)", 0, KEY_READ, &rkey);
    if (ret != ERROR_SUCCESS) {
        logsPage->writeLog(QString("failed to open bios subkey, code: %1").arg(ret));
        return "";
    }

    ret = RegGetValueW(rkey, nullptr, L"BaseBoardProduct", RRF_RT_REG_SZ, nullptr, nullptr, &bufSz);
    if (ret != ERROR_SUCCESS) {
        logsPage->writeLog(QString("failed to read size for reg value, code %1").arg(ret));
        RegCloseKey(rkey);
        return "";
    }

    bufSz += sizeof(wchar_t);
    buf = std::make_unique<wchar_t[]>(bufSz);

    ret = RegGetValueW(rkey, nullptr, L"BaseBoardProduct", RRF_RT_REG_SZ, nullptr, buf.get(), &bufSz);
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
    else if (product == OWC::mini24 || product == OWC::max2_22 || product == OWC::max2_25)
        device = QSharedPointer<OWC::ControllerV1>::create(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::RumbleV1);
    /*else if (product == OWC::win3)
        device = QSharedPointer<OWC::ControllerV1>::create();*/
    else if (product == OWC::win5 || product == OWC::max3)
        device = QSharedPointer<OWC::ControllerV2>::create(OWC::ControllerFeature::RumbleV1 | OWC::ControllerFeature::XinputMappingV1 | OWC::ControllerFeature::BackButton4);
    else if (product == OWC::mini25 || product == OWC::mini25L)
        device = QSharedPointer<OWC::ControllerV2>::create(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::RumbleV1 | OWC::ControllerFeature::XinputMappingV1);
    else
        logsPage->writeLog(QString("unknown device: %1").arg(product));

    return device;
}

bool MainWindow::isCompatible(const QString &product) const {
    const auto [major, minor] = gpd->getKVersion();
    bool isSupported = false;

    /*if (product == OWC::win3)
       return true;
   else*/
    if (product == OWC::win4)
       isSupported = major >= 0x4 && minor >= 0x7;
    else if (product == OWC::mini24)
        isSupported = major >= 0x5 && minor >= 0x3;
    else if (product == OWC::max2_22 || product == OWC::max2_25)
        isSupported = major >= 1 && minor >= 0x23;
    else if (product == OWC::win5)
        isSupported = major >= 1 && minor >= 0x8;
    else if (product == OWC::mini25 || product == OWC::mini25L)
        isSupported = major >= 1 && minor >= 0x22;
    else if (product == OWC::max3)
        return true;

    if (!isSupported)
        logsPage->writeLog(QString("version %1.%2 is not supported").arg(major).arg(minor));

    return isSupported;
}

void MainWindow::initApp() {
    const std::function<void(const std::wstring &)> logCB = [&](const std::wstring &msg) { logsPage->writeLog(QString::fromStdWString(msg)); };
    const QString prod = getProduct();

    gpd = getDevice(prod);
    if (gpd.isNull())
        return;

    gpd->enableLogging(logCB);

    if (!gpd->init()) {
        logsPage->writeLog(u"device initialization failed"_s);
        return;

    } else if (!gpd->readVersion()) {
        logsPage->writeLog(u"failed to read firmware version"_s);
        return;

    } else if (!isCompatible(prod)) {
        logsPage->writeLog(u"no compatible controller found"_s);
        return;

    } else if (!gpd->readConfig()) {
        logsPage->writeLog(u"failed to read firmware config"_s);
        return;
    }

    const auto [xmin, xmax] = gpd->getXVersion();
    const auto [kmin, kmax] = gpd->getKVersion();

    if (gpd->getControllerType() == 1) {
        backButtonsPage = new OWC::BackButtonsV1Page();

    } else if (gpd->getControllerType() == 2) {
        const QSharedPointer<OWC::ControllerV2> gpdV2 = qSharedPointerCast<OWC::ControllerV2>(gpd);

        backButtonsPage = new OWC::BackButtonsV2Page();
        xinputPage = new OWC::XinputButtonsPage();

        stackedWidget->addWidget(xinputPage);
        xinputPage->setMapping(gpd);
        homePage->setEmulationMode(gpdV2->getEmulationMode());
        initGamepadThread();

        QObject::connect(xinputPage, &OWC::FaceButtonsPage::showCharMap, this, &MainWindow::onXinputCharMapClicked);
        QObject::connect(xinputPage, &OWC::FaceButtonsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
        QObject::connect(xinputPage, &OWC::XinputButtonsPage::resetXinputButtons, this, &MainWindow::onResetXinputButtons);
        QObject::connect(xinputPage, &OWC::FaceButtonsPage::logSent, this, &MainWindow::onLogSent);
    }

    charMapPage = new OWC::CharMapPage(gpd->getControllerType() == 2);
    kbdMousePage = new OWC::KeyboardMouseButtonsPage();
    yamlBrowserPage = new OWC::YamlBrowserPage(appDataPath, gpd->getControllerType());

    controllerVersionLbl->setText(QString("X%1.%2, K%3.%4").arg(QString::number(xmin, 16))
                                                                .arg(QString::number(xmax, 16))
                                                                .arg(QString::number(kmin, 16))
                                                                .arg(QString::number(kmax, 16)));

    stackedWidget->addWidget(charMapPage);
    stackedWidget->addWidget(kbdMousePage);
    stackedWidget->addWidget(backButtonsPage);
    stackedWidget->addWidget(yamlBrowserPage);
    backButtonsPage->initPage(gpd);
    settingsPage->initPage(gpd);
    kbdMousePage->setMapping(gpd);
    backButtonsPage->setMapping(gpd);
    settingsPage->setData(gpd);
    homePage->setDevice(prod);

    QObject::connect(homePage, &OWC::HomePage::keyboardMouseMap, this, &MainWindow::onHomeKeyboardMouseMapClicked);
    QObject::connect(homePage, &OWC::HomePage::xinputMap, this, &MainWindow::onHomeXinputMapClicked);
    QObject::connect(homePage, &OWC::HomePage::backButtonsMap, this, &MainWindow::onHomeBackButtonsMapClicked);
    QObject::connect(homePage, &OWC::HomePage::yamlBrowser, this, &MainWindow::onHomeYamlBrowserClicked);
    QObject::connect(homePage, &OWC::HomePage::exportYaml, this, &MainWindow::onHomeExportYamlClicked);
    QObject::connect(homePage, &OWC::HomePage::importYaml, this, &MainWindow::onHomeImportYamlClicked);
    QObject::connect(homePage, &OWC::HomePage::settingsPage, this, &MainWindow::onHomeSettingsPageClicked);
    QObject::connect(homePage, &OWC::HomePage::applyChanges, this, &MainWindow::onHomeApplyChanges);
    QObject::connect(charMapPage, &OWC::CharMapPage::hideCharMap, this, &MainWindow::onHideCharMapClicked);
    QObject::connect(charMapPage, &OWC::CharMapPage::keyPressed, this, &MainWindow::onCharMapKeyPressed);
    QObject::connect(kbdMousePage, &OWC::FaceButtonsPage::showCharMap, this, &MainWindow::onKeyboardMouseCharMapClicked);
    QObject::connect(kbdMousePage, &OWC::FaceButtonsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(kbdMousePage, &OWC::KeyboardMouseButtonsPage::resetKeyboardMouseButtons, this, &MainWindow::onResetKeyboardMouseButtons);
    QObject::connect(kbdMousePage, &OWC::FaceButtonsPage::logSent, this, &MainWindow::onLogSent);
    QObject::connect(backButtonsPage, &OWC::BackButtonsPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(backButtonsPage, &OWC::BackButtonsPage::showCharMap, this, &MainWindow::onBackButtonsCharMapClicked);
    QObject::connect(backButtonsPage, &OWC::BackButtonsPage::resetBackButtons, this, &MainWindow::onResetBackButtons);
    QObject::connect(backButtonsPage, &OWC::BackButtonsPage::logSent, this, &MainWindow::onLogSent);
    QObject::connect(yamlBrowserPage, &OWC::YamlBrowserPage::backToHome, this, &MainWindow::onBackToHomeClicked);
    QObject::connect(yamlBrowserPage, &OWC::YamlBrowserPage::logSent, this, &MainWindow::onLogSent);
    QObject::connect(yamlBrowserPage, &OWC::YamlBrowserPage::importProfile, this, &MainWindow::onYamlBrowserImportProfile);
}

void MainWindow::initGamepadThread() {
    gamepadThread = new QThread();
    gamepadWorker = new OWC::GamepadWorker();

    gamepadWorker->moveToThread(gamepadThread);

    QObject::connect(gamepadThread, &QThread::started, gamepadWorker, &OWC::GamepadWorker::startSDLEventsThread);
    QObject::connect(this, &MainWindow::enableSDLEvents, gamepadWorker, &OWC::GamepadWorker::enableEvents);
    QObject::connect(gamepadWorker, &OWC::GamepadWorker::initFail, this, &MainWindow::onGamepadInitFail);
    QObject::connect(gamepadWorker, &OWC::GamepadWorker::logSent, this, &MainWindow::onLogSent);
    QObject::connect(gamepadWorker, &OWC::GamepadWorker::gamepadButton, this, &MainWindow::onGamepadButton);

    gamepadThread->start();
}

void MainWindow::quitGamepadThread() {
    SDL_Event sdlEvt {.type = SDL_EVENT_QUIT};

    SDL_PushEvent(&sdlEvt);
    gamepadThread->quit();
    gamepadThread->wait();
    delete gamepadThread;
    delete gamepadWorker;

    gamepadThread = nullptr;
    gamepadWorker = nullptr;
}

void MainWindow::importYamlMapping(const YAML::Node &yaml) const {
    kbdMousePage->importMappingFromYaml(yaml);
    backButtonsPage->importMappingFromYaml(yaml);

    if (xinputPage != nullptr)
        xinputPage->importMappingFromYaml(yaml);
}

void MainWindow::onLogSent(const QString& msg) const {
    logsPage->writeLog(msg);
}

void MainWindow::onHomeKeyboardMouseMapClicked() const {
    stackedWidget->setCurrentWidget(kbdMousePage);
}

void MainWindow::onHomeXinputMapClicked() {
    emit enableSDLEvents(true);
    stackedWidget->setCurrentWidget(xinputPage);
}

void MainWindow::onHomeBackButtonsMapClicked() {
    if (gamepadThread != nullptr)
        emit enableSDLEvents(true);

    stackedWidget->setCurrentWidget(backButtonsPage);
}

void MainWindow::onHomeShowLogsClicked() const {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::onHomeYamlBrowserClicked() const {
    stackedWidget->setCurrentWidget(yamlBrowserPage);
}

void MainWindow::onHomeSettingsPageClicked() const {
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::onHomeApplyChanges() {
    bool writeFlash = true;

    homePage->enableButtons(false);
    kbdMousePage->writeMapping(gpd);
    backButtonsPage->writeMapping(gpd);
    settingsPage->writeSettings(gpd);

    if (xinputPage != nullptr)
        xinputPage->writeMapping(gpd);

    if (gpd->getControllerType() == 2) {
        QMessageBox mbox(this);

        mbox.setWindowTitle(u"Write config"_s);
        mbox.setText(u"Do you want to flash current config to controller?"_s);
        mbox.setStandardButtons(QMessageBox::Yes);
        mbox.addButton(QMessageBox::No);
        mbox.setDefaultButton(QMessageBox::Yes);
        mbox.setDetailedText(
            u"Yes: write changes to controller flash memory to make them permanent.\n\n"
            "No: write changes to controller temp memory, reverted after sleep/reboot/shutdown.\n\n"
            "Saying No here, allows you to save some writes while doing experiments, or keep a base permanent config."_s
        );

        writeFlash = mbox.exec() == QMessageBox::Yes;
    }

    if (!(writeFlash ? gpd->writeConfig() : gpd->writeConfigMem()))
        QMessageBox::critical(this, u"Error"_s, u"Unable to write controller!"_s);

    homePage->enableButtons(true);
}

void MainWindow::onHomeExportYamlClicked() {
    const QString out = QFileDialog::getSaveFileName(this, u"Export mapping to file"_s, "", u"Yaml (*.yaml)"_s);

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
    const QString map = QFileDialog::getOpenFileName(this, u"Import mapping from file"_s, "", u"Yaml (*.yaml)"_s);

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
            logsPage->writeLog(u"Imported file does not contain a valid yaml map"_s);
            return;
        }

        const int type = yaml["MAPPING_TYPE"].as<int>();

        if (gpd->getControllerType() != type) {
            logsPage->writeLog(QString("incompatible mapping type, cannot import %1").arg(type));
            return;
        }

        mappF.close();
        importYamlMapping(yaml);

    } catch (const YAML::Exception &e) {
        logsPage->writeLog(e.what());
        mappF.close();
        return;
    }

    logsPage->writeLog(QString("imported mapping from file: %1").arg(map));
}

void MainWindow::onKeyboardMouseCharMapClicked() {
    previousPage = kbdMousePage;

    charMapPage->setMode(OWC::CharMapMode::Keyboard);
    stackedWidget->setCurrentWidget(charMapPage);
}

void MainWindow::onXinputCharMapClicked() {
    previousPage = xinputPage;

    if (gamepadThread != nullptr)
        emit enableSDLEvents(false);

    charMapPage->setMode(OWC::CharMapMode::Xinput);
    stackedWidget->setCurrentWidget(charMapPage);
}

void MainWindow::onBackButtonsCharMapClicked() {
    previousPage = backButtonsPage;

    if (gamepadThread != nullptr)
        emit enableSDLEvents(false);

    charMapPage->setMode(gpd->getControllerType() == 1 ? OWC::CharMapMode::Keyboard : OWC::CharMapMode::Mixed);
    stackedWidget->setCurrentWidget(charMapPage);
}

void MainWindow::onHideCharMapClicked() {
    if (gamepadThread != nullptr && previousPage != kbdMousePage)
        emit enableSDLEvents(true);

    stackedWidget->setCurrentWidget(previousPage);
    previousPage = nullptr;
}

void MainWindow::onCharMapKeyPressed(const QString &key) const {
    if (previousPage == nullptr)
        return;

    if (previousPage == kbdMousePage)
        kbdMousePage->setPendingButton(key);
    else if (previousPage == xinputPage)
        xinputPage->setPendingButton(key);
    else if (previousPage == backButtonsPage)
        backButtonsPage->setPendingButton(key);
}

void MainWindow::onSettingsConfigResetClicked() {
    if (!gpd->resetConfig())
        QMessageBox::critical(this, u"Configuration reset"_s, u"Failed"_s);
    else
        QMessageBox::information(this, u"Configuration reset"_s, u"Success"_s);
}

void MainWindow::onYamlBrowserImportProfile(const QString &yml) const {
    try {
        const YAML::Node yaml = YAML::Load(yml.toStdString());

        importYamlMapping(yaml);

    } catch (const YAML::Exception &e) {
        logsPage->writeLog(e.what());
        return;
    }

    logsPage->writeLog(u"imported mapping from profile"_s);
}

void MainWindow::onBackToHomeClicked() {
    emit enableSDLEvents(false);
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

void MainWindow::onGamepadButton(const QString &key) const {
    const QWidget *curPage = stackedWidget->currentWidget();

    if (curPage == xinputPage)
        xinputPage->setPendingButton(key);
    else if (curPage == backButtonsPage)
        backButtonsPage->setPendingButton(key);
}

void MainWindow::onGamepadInitFail() {
    quitGamepadThread();
}
