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
#include <QScrollBar>
#include <QScrollArea>
#include <QScroller>
#include <QLabel>
#include <QMessageBox>
#include <QDirListing>
#include <QFile>
#include <QDir>

#include "YamlBrowserPage.h"

namespace OWC {
    YamlBrowserPage::YamlBrowserPage(const QString &dataPath, const int type) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QVBoxLayout *ymlContLyt = new QVBoxLayout();
        QHBoxLayout *contLyt = new QHBoxLayout();
        QVBoxLayout *viewLyt = new QVBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QScrollArea *scrollArea = new QScrollArea();
        QLabel *localTitle = new QLabel("Local");
        QLabel *ghTitle = new QLabel("GitHub");
        QFont titleFont = localTitle->font();

        appDataPath = dataPath;
        controllerType = QString("v%1").arg(type);
        ghYmlTmpPath = QString("%1/ghyml").arg(dataPath);
        ymlsPath = QString("%1/profiles").arg(dataPath);
        dbPath = QString("%1/owcdb").arg(appDataPath);
        localYmlContainer = new FlowLayout();
        ghYmlContainer = new FlowLayout();
        backBtn = new QPushButton("Home");
        refreshBtn = new QPushButton("Refresh");
        importYmlBtn = new QPushButton("Import");
        downloadYmlBtn = new QPushButton("Download");
        profileView = new QTextEdit();

        if (dataPath.isEmpty() || (!QDir().exists(ymlsPath) && !QDir().mkdir(ymlsPath))) {
            emit logSent(QStringLiteral("failed to create profiles folder"));
            ymlsPath.clear();
            refreshBtn->setEnabled(false);

        } else {
            fsWatcher = new QFileSystemWatcher(this);

            fsWatcher->addPath(ymlsPath);
            QObject::connect(fsWatcher, &QFileSystemWatcher::directoryChanged, this, &YamlBrowserPage::onFsFolderChanged);
        }

        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
        QScroller::grabGesture(profileView->viewport(), QScroller::LeftMouseButtonGesture);
        profileView->setFixedWidth(300);
        profileView->setReadOnly(true);
        profileView->setWordWrapMode(QTextOption::WordWrap);
        localTitle->setAlignment(Qt::AlignCenter);
        ghTitle->setAlignment(Qt::AlignCenter);
        titleFont.setBold(true);
        localTitle->setFont(titleFont);
        ghTitle->setFont(titleFont);

        scrollArea->widget()->setLayout(ymlContLyt);

        ymlContLyt->addWidget(localTitle);
        ymlContLyt->addLayout(localYmlContainer);
        ymlContLyt->addSpacing(8);
        ymlContLyt->addWidget(ghTitle);
        ymlContLyt->addLayout(ghYmlContainer);
        ymlContLyt->addStretch();
        viewLyt->addWidget(profileView);
        viewLyt->addWidget(importYmlBtn);
        viewLyt->addWidget(downloadYmlBtn);
        contLyt->addWidget(scrollArea);
        contLyt->addLayout(viewLyt);
        bottomLyt->addStretch();
        bottomLyt->addWidget(backBtn);
        bottomLyt->addWidget(refreshBtn);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addLayout(contLyt);
        lyt->addLayout(bottomLyt);

        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &YamlBrowserPage::onBackBtnClicked);
        QObject::connect(refreshBtn, &QPushButton::clicked, this, &YamlBrowserPage::onRefreshBtnClicked);
        QObject::connect(importYmlBtn, &QPushButton::clicked, this, &YamlBrowserPage::onYmlImportClicked);
        QObject::connect(downloadYmlBtn, &QPushButton::clicked, this, &YamlBrowserPage::onYmlDownloadClicked);

        listLocalProfiles();
        onRefreshBtnClicked();
    }

    void YamlBrowserPage::startDownloadThread() {
        if (downloadThd != nullptr)
            return;

        downloadThd = new QThread();
        downloadWrk = new DownloadWorker();

        downloadWrk->moveToThread(downloadThd);

        QObject::connect(downloadWrk, &DownloadWorker::failed, this, &YamlBrowserPage::onDownloadFailed);
        QObject::connect(this, &YamlBrowserPage::startDownload, downloadWrk, &DownloadWorker::downloadFile);

        downloadThd->start();
    }

    void YamlBrowserPage::stopDownloadThread() {
        if (downloadThd == nullptr)
            return;

        downloadThd->quit();
        downloadThd->wait();
        delete downloadWrk;
        delete downloadThd;

        downloadWrk = nullptr;
        downloadThd = nullptr;
    }

    void YamlBrowserPage::clearYmlLayout(FlowLayout *lyt) const {
        while (true) {
            const QLayoutItem *itm = lyt->takeAt(0);

            if (itm == nullptr)
                return;

            delete itm->widget();
            delete itm;
        }
    }

    void YamlBrowserPage::listLocalProfiles() {
        for (const QDirListing::DirEntry &entry: QDirListing(ymlsPath, QDirListing::IteratorFlag::FilesOnly)) {
            if (entry.suffix() != "yaml")
                continue;

            YmlProfileBox *ymlBox = new YmlProfileBox(entry.baseName().remove(".yaml"), true);

            localYmlContainer->addWidget(ymlBox);
            QObject::connect(ymlBox, &YmlProfileBox::viewProfile, this, &YamlBrowserPage::onViewLocalProfile);
            QObject::connect(ymlBox, &YmlProfileBox::deleteProfile, this, &YamlBrowserPage::onDeleteLocalProfile);
        }
    }

    void YamlBrowserPage::listGHProfiles() {
        QFile dbF(dbPath);
        QTextStream ts(&dbF);

        if (!dbF.open(QFile::Text | QFile::ReadOnly)) {
            emit logSent(QString("failed to read db file: %1").arg(dbF.errorString()));
            return;
        }

        while (!ts.atEnd()) {
            const QList<QString> lineData = ts.readLine().split('/');

            if (lineData.isEmpty() || lineData[0] != controllerType)
                continue;

            YmlProfileBox *ymlBox = new YmlProfileBox(lineData[1], false);

            ghYmlContainer->addWidget(ymlBox);
            QObject::connect(ymlBox, &YmlProfileBox::viewProfile, this, &YamlBrowserPage::onViewGHProfile);
        }
    }

    void YamlBrowserPage::onLogSent(const QString &msg) {
        emit logSent(msg);
    }

    void YamlBrowserPage::onBackBtnClicked() {
        profileView->clear();
        importYmlBtn->setEnabled(false);
        downloadYmlBtn->setEnabled(false);
        emit backToHome();
    }

    void YamlBrowserPage::onRefreshBtnClicked() {
        refreshBtn->setEnabled(false);
        importYmlBtn->setEnabled(false);
        downloadYmlBtn->setEnabled(false);
        profileView->clear();
        startDownloadThread();

        QObject::connect(downloadWrk, &DownloadWorker::success, this, &YamlBrowserPage::onDBDownloadSuccess);

        emit startDownload(dbUrl, QString("%1/owcdb").arg(appDataPath));
    }

    void YamlBrowserPage::onDownloadFailed() {
        emit logSent(QStringLiteral("Failed to download from repo"));
        stopDownloadThread();
        profileView->clear();
        refreshBtn->setEnabled(true);
    }

    void YamlBrowserPage::onDBDownloadSuccess() {
        QFile dbF(dbPath);

        stopDownloadThread();

        if (dbF.open(QFile::Text | QFile::ReadOnly)) {
            QTextStream ts(&dbF);
            const QString ghVer = ts.readLine();

            if (ghVer != dbVersion) {
                dbVersion = ghVer;

                clearYmlLayout(ghYmlContainer);
                listGHProfiles();
            }
        } else {
            emit logSent(QString("failed to read version string: %1").arg(dbF.errorString()));
            QMessageBox::critical(this, "Download", "Failed, see logs");
        }

        refreshBtn->setEnabled(true);
    }

    void YamlBrowserPage::onViewLocalProfile(const QString &name) {
        QFile yml(QString("%1/%2.yaml").arg(ymlsPath, name));

        if (!yml.open(QFile::Text | QFile::ReadOnly)) {
            importYmlBtn->setEnabled(false);
            profileView->clear();
            emit logSent(QString("failed to read local yaml file: %1").arg(yml.errorString()));
            return;
        }

        importYmlBtn->setEnabled(true);
        downloadYmlBtn->setEnabled(false);
        profileView->setText(yml.readAll());
    }

    void YamlBrowserPage::onDeleteLocalProfile(const QString &name, const YmlProfileBox *wdg) {
        const QSignalBlocker sblock {fsWatcher};
        QFile yml(QString("%1/%2.yaml").arg(ymlsPath, name));

        if (!yml.remove()) {
            emit logSent(QString("failed to delete profile: %1").arg(yml.errorString()));
            return;
        }

        const QLayoutItem *itm = localYmlContainer->takeAt(localYmlContainer->indexOf(wdg));

        delete itm->widget();
        delete itm;
    }

    void YamlBrowserPage::onViewGHProfile(const QString &name) {
        curViewGHProfileName = name;

        refreshBtn->setEnabled(false);
        importYmlBtn->setEnabled(false);
        downloadYmlBtn->setEnabled(false);
        profileView->setText("Downloading..");
        startDownloadThread();

        QObject::connect(downloadWrk, &DownloadWorker::success, this, &YamlBrowserPage::onGHYmlDownloadSuccess);

        emit startDownload(QString("%1/%2/%3.yaml").arg(baseYmlUrl, controllerType, name), ghYmlTmpPath);
    }

    void YamlBrowserPage::onGHYmlDownloadSuccess() {
        QFile yml(ghYmlTmpPath);

        stopDownloadThread();
        refreshBtn->setEnabled(true);

        if (!yml.open(QFile::Text | QFile::ReadOnly)) {
            emit logSent(QString("failed to read github yaml file: %1").arg(yml.errorString()));
            return;
        }

        profileView->setText(yml.readAll());
        importYmlBtn->setEnabled(true);
        downloadYmlBtn->setEnabled(true);
        yml.remove();
    }

    void YamlBrowserPage::onYmlImportClicked() {
        emit importProfile(profileView->toPlainText());
    }

    void YamlBrowserPage::onYmlDownloadClicked() {
        downloadYmlBtn->setEnabled(false);

        QFile yml(QString("%1/%2.yaml").arg(ymlsPath, curViewGHProfileName));

        if (!yml.open(QFile::WriteOnly | QFile::Text)) {
            emit logSent(QString("failed to open destination file: %1").arg(yml.errorString()));
            QMessageBox::critical(this, "Download", "Failed, see logs");
            downloadYmlBtn->setEnabled(true);
            return;
        }

        if (yml.write(profileView->toPlainText().toUtf8()) == -1) {
            emit logSent(QString("failed to write file: %1").arg(yml.errorString()));
            QMessageBox::critical(this, "Download", "Failed, see logs");
            downloadYmlBtn->setEnabled(true);
        }
    }

    void YamlBrowserPage::onFsFolderChanged([[maybe_unused]] const QString &path) {
        clearYmlLayout(localYmlContainer);
        listLocalProfiles();
    }
}
