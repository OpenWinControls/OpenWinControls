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
#include "BackButtonsV2Page.h"
#include "../../extern/libOpenWinControls/src/include/ControllerFeature.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    BackButtonsV2Page::BackButtonsV2Page(const std::shared_ptr<Controller> &gpd): BackButtonsPage(u"key slots, start times and hold times"_s) {
        controller = std::static_pointer_cast<ControllerV2>(gpd);
        l4 = new BackButtonV2Widget(1, 32, u"l4"_s);
        r4 = new BackButtonV2Widget(2, 32, u"r4"_s);

        backBtnLyt->addWidget(l4);
        backBtnLyt->addWidget(r4);

        if (gpd->hasFeature(ControllerFeature::BackButton4)) {
            r5 = new BackButtonV2Widget(4, 32, u"r5"_s);

            backBtnLyt->addWidget(r5);
            QObject::connect(r5, &BackButtonV2Widget::logSent, this, &BackButtonsV2Page::onLogSent);
            QObject::connect(r5, &BackButtonV2Widget::pendingEditBtn, this, &BackButtonsV2Page::onkeyButtonPressed);
        }

        QObject::connect(l4, &BackButtonV2Widget::logSent, this, &BackButtonsV2Page::onLogSent);
        QObject::connect(l4, &BackButtonV2Widget::pendingEditBtn, this, &BackButtonsV2Page::onkeyButtonPressed);
        QObject::connect(r4, &BackButtonV2Widget::logSent, this, &BackButtonsV2Page::onLogSent);
        QObject::connect(r4, &BackButtonV2Widget::pendingEditBtn, this, &BackButtonsV2Page::onkeyButtonPressed);
    }

    void BackButtonsV2Page::refresh() const {
        l4->setMapping(controller);
        r4->setMapping(controller);

        if (r5 != nullptr)
            r5->setMapping(controller);
    }

    void BackButtonsV2Page::writeMapping() {
        l4->writeMapping(controller);
        r4->writeMapping(controller);

        if (r5 != nullptr)
            r5->writeMapping(controller);
    }

    QString BackButtonsV2Page::exportToYaml() const {
        QString yaml;
        QTextStream ts(&yaml);

        ts << l4->exportToYaml() <<
                r4->exportToYaml();

        if (r5 != nullptr)
            ts << r5->exportToYaml();

        return yaml;
    }

    void BackButtonsV2Page::importFromYaml(const YAML::Node &yaml) const {
        l4->importFromYaml(yaml);
        r4->importFromYaml(yaml);

        if (r5 != nullptr)
            r5->importFromYaml(yaml);
    }

    void BackButtonsV2Page::onLogSent(const QString &msg) {
        emit logSent(msg);
    }
}
