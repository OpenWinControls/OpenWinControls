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
#include "DownloadWorker.h"

namespace OWC {
    void DownloadWorker::downloadFile(const QString &url) {
        QNetworkRequest req = QNetworkRequest(QUrl(url));
        QNetworkReply *reply;

        netAccess.reset(new QNetworkAccessManager);
        netAccess->setAutoDeleteReplies(true);

        req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:145.0) Gecko/20100101 Firefox/145.0");

        QObject::connect(netAccess.get(), &QNetworkAccessManager::finished, this, &DownloadWorker::onDownloadFinisched);

        reply = netAccess->get(req);

        QObject::connect(reply, &QNetworkReply::errorOccurred, this, &DownloadWorker::onReplyError);
    }

    void DownloadWorker::onDownloadFinisched(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError)
            emit success(reply->readAll());
    }

    void DownloadWorker::onReplyError(const QNetworkReply::NetworkError code) {
        emit logSent(QString("download failed, code: %1").arg(code));
        emit failed();
    }
}
