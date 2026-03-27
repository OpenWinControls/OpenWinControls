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

#include <QNetworkReply>

namespace OWC {
    class DownloadWorker final: public QObject {
        Q_OBJECT

    private:
        QScopedPointer<QNetworkAccessManager> netAccess;

    private slots:
        void onDownloadFinisched(QNetworkReply *reply);
        void onReplyError(QNetworkReply::NetworkError code);

    public slots:
        void downloadFile(const QString &url);

    signals:
        void logSent(const QString &msg);
        void failed();
        void success(const QByteArray &data);
    };
}
