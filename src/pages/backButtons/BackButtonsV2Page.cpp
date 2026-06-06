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

#include "BackButtonsV2Page.h"
#include "../../extern/libOpenWinControls/src/include/ControllerFeature.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    BackButtonsV2Page::BackButtonsV2Page(): BackButtonsPage(u"key slots, start times and hold times"_s) {
        l4 = new BackButtonV2Widget(1, 32, u"l4"_s);
        r4 = new BackButtonV2Widget(2, 32, u"r4"_s);

        backBtnLyt->addWidget(l4);
        backBtnLyt->addWidget(r4);

        QObject::connect(l4, &BackButtonV2Widget::logSent, this, &BackButtonsV2Page::onBackButtonLogSent);
        QObject::connect(l4, &BackButtonV2Widget::pendingEditBtn, this, &BackButtonsV2Page::onkeyButtonPressed);
        QObject::connect(r4, &BackButtonV2Widget::logSent, this, &BackButtonsV2Page::onBackButtonLogSent);
        QObject::connect(r4, &BackButtonV2Widget::pendingEditBtn, this, &BackButtonsV2Page::onkeyButtonPressed);
    }

    void BackButtonsV2Page::initPage(const QSharedPointer<Controller> &gpd) {
        if (gpd->hasFeature(ControllerFeature::BackButton4)) {
            r5 = new BackButtonV2Widget(4, 32, u"r5"_s);

            backBtnLyt->addWidget(r5);
            QObject::connect(r5, &BackButtonV2Widget::logSent, this, &BackButtonsV2Page::onBackButtonLogSent);
            QObject::connect(r5, &BackButtonV2Widget::pendingEditBtn, this, &BackButtonsV2Page::onkeyButtonPressed);
        }
    }

    void BackButtonsV2Page::setMapping(const QSharedPointer<Controller> &gpd) const {
        const QSharedPointer<ControllerV2> gpdV2 = qSharedPointerCast<OWC::ControllerV2>(gpd);

        l4->setMapping(gpdV2);
        r4->setMapping(gpdV2);

        if (r5 != nullptr)
            r5->setMapping(gpdV2);
    }

    void BackButtonsV2Page::writeMapping(const QSharedPointer<Controller> &gpd) {
        const QSharedPointer<ControllerV2> gpdV2 = qSharedPointerCast<OWC::ControllerV2>(gpd);

        l4->writeMapping(gpdV2);
        r4->writeMapping(gpdV2);

        if (r5 != nullptr)
            r5->writeMapping(gpdV2);
    }

    QString BackButtonsV2Page::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts(&yaml);

        ts << l4->exportToYaml() <<
                r4->exportToYaml();

        if (r5 != nullptr)
            ts << r5->exportToYaml();

        return yaml;
    }

    void BackButtonsV2Page::importMappingFromYaml(const YAML::Node &yaml) const {
        l4->importFromYaml(yaml);
        r4->importFromYaml(yaml);

        if (r5 != nullptr)
            r5->importFromYaml(yaml);
    }

    void BackButtonsV2Page::onBackButtonLogSent(const QString &msg) {
        emit logSent(msg);
    }
}
