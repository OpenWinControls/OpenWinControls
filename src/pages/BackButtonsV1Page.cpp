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
#include <QLabel>

#include "BackButtonsV1Page.h"

namespace OWC {
    BackButtonsV1Page::BackButtonsV1Page(): BackButtonsPage(QStringLiteral("macro key slots and start times"), CharMapMode::Keyboard) {
        l4 = new BackButtonV1Widget(1, 3, "l4");
        r4 = new BackButtonV1Widget(2, 3, "r4");

        backBtnLyt->addWidget(l4);
        backBtnLyt->addWidget(r4);

        QObject::connect(l4, &BackButtonV1Widget::logSent, this, &BackButtonsV1Page::onBackButtonLogSent);
        QObject::connect(r4, &BackButtonV1Widget::logSent, this, &BackButtonsV1Page::onBackButtonLogSent);
    }

    void BackButtonsV1Page::setMapping(const QSharedPointer<Controller> &gpd) const {
        l4->setMapping(gpd);
        r4->setMapping(gpd);
    }

    void BackButtonsV1Page::writeMapping(const QSharedPointer<Controller> &gpd) {
        l4->writeMapping(gpd);
        r4->setMapping(gpd);
    }

    QString BackButtonsV1Page::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts(&yaml);

        ts << l4->exportToYaml() <<
            r4->exportToYaml();

        return yaml;
    }

    void BackButtonsV1Page::importMappingFromYaml(const YAML::Node &yaml) const {
        l4->importFromYaml(yaml);
        r4->importFromYaml(yaml);
    }

    void BackButtonsV1Page::onBackButtonLogSent(const QString &msg) {
        emit logSent(msg);
    }
}
