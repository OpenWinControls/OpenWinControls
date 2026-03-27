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

#include <QThread>
#include <QTextEdit>
#include <QFileSystemWatcher>

#include "../Widgets/FlowLayout.h"
#include "Widgets/YmlProfileBox.h"
#include "../Include/DownloadWorker.h"

namespace OWC {
    class YamlBrowserPage final: public QWidget {
        Q_OBJECT

    private:
        static constexpr char dbUrl[] = "https://github.com/OpenWinControls/CommunityProfiles/releases/latest/download/owcdb.txt";
        static constexpr char baseYmlUrl[] = "https://raw.githubusercontent.com/OpenWinControls/CommunityProfiles/refs/heads/main/";
        FlowLayout *localYmlContainer = nullptr;
        FlowLayout *ghYmlContainer = nullptr;
        QPushButton *backBtn = nullptr;
        QPushButton *refreshBtn = nullptr;
        QPushButton *importYmlBtn = nullptr;
        QPushButton *downloadYmlBtn = nullptr;
        DownloadWorker *downloadWrk = nullptr;
        QThread *downloadThd = nullptr;
        QTextEdit *profileView = nullptr;
        QFileSystemWatcher *fsWatcher = nullptr;
        QList<YmlProfileBox *> ymlList;
        QString appDataPath;
        QString controllerType;
        QString dbVersion;
        QString curViewGHProfileName;
        QString ghYmlTmpPath;
        QString ymlsPath;
        QString dbPath;

        void startDownloadThread();
        void stopDownloadThread();
        void clearYmlLayout(FlowLayout *lyt) const;
        void listLocalProfiles();
        void listGHProfiles();

    public:
        YamlBrowserPage(const QString &dataPath, int type);

    private slots:
        void onLogSent(const QString &msg);
        void onBackBtnClicked();
        void onRefreshBtnClicked();
        void onDownloadFailed();
        void onDBDownloadSuccess();
        void onViewLocalProfile(const QString &name);
        void onDeleteLocalProfile(const QString &name, const YmlProfileBox *wdg);
        void onViewGHProfile(const QString &name);
        void onGHYmlDownloadSuccess();
        void onYmlImportClicked();
        void onYmlDownloadClicked();
        void onFsFolderChanged(const QString &path);

    signals:
        void logSent(const QString &msg);
        void backToHome();
        void startDownload(const QString &url, const QString &path);
        void importProfile(const QString &yml);
    };
}
