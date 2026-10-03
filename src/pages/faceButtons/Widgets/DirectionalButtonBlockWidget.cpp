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
#include <QVBoxLayout>
#include <QLabel>

#include "DirectionalButtonBlockWidget.h"
#include "../../Include/KeyButtonEventFilter.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    DirectionalButtonBlockWidget::DirectionalButtonBlockWidget(const Button topOwcBtn, const Button leftOwcBtn,
                                                const Button rightOwcBtn, const Button bottomOwcBtn,
                                                const std::string_view topYamlKey, const std::string_view leftYamlKey,
                                                const std::string_view rightYamlKey, const std::string_view bottomYamlKey,
                                                const QString &icon, const int iconScale) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *topLyt = new QHBoxLayout();
        QHBoxLayout *centerLyt = new QHBoxLayout();
        QHBoxLayout *bottomLyt = new QHBoxLayout();
        QLabel *controlIcon = new QLabel();

        topOwcButton = topOwcBtn;
        leftOwcButton = leftOwcBtn;
        rightOwcButton = rightOwcBtn;
        bottomOwcButton = bottomOwcBtn;
        topYmlKey = topYamlKey;
        leftYmlKey = leftYamlKey;
        rightYmlKey = rightYamlKey;
        bottomYmlKey = bottomYamlKey;
        topBtn = new QPushButton();
        leftBtn = new QPushButton();
        rightBtn = new QPushButton();
        bottomBtn = new QPushButton();

        topBtn->setFixedWidth(buttonWidth);
        topBtn->installEventFilter(new KeyButtonEventFilter(this));
        leftBtn->setFixedWidth(buttonWidth);
        leftBtn->installEventFilter(new KeyButtonEventFilter(this));
        rightBtn->setFixedWidth(buttonWidth);
        rightBtn->installEventFilter(new KeyButtonEventFilter(this));
        bottomBtn->setFixedWidth(buttonWidth);
        bottomBtn->installEventFilter(new KeyButtonEventFilter(this));
        controlIcon->setPixmap(QPixmap(u":/icons/%1"_s.arg(icon)).scaled(iconScale, iconScale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        topLyt->setAlignment(Qt::AlignCenter);
        topLyt->addWidget(topBtn);

        centerLyt->setAlignment(Qt::AlignCenter);
        centerLyt->addWidget(leftBtn);
        centerLyt->addWidget(controlIcon);
        centerLyt->addWidget(rightBtn);

        bottomLyt->setAlignment(Qt::AlignCenter);
        bottomLyt->addWidget(bottomBtn);

        lyt->addLayout(topLyt);
        lyt->addLayout(centerLyt);
        lyt->addLayout(bottomLyt);

        setLayout(lyt);

        QObject::connect(topBtn, &QPushButton::clicked, this, &DirectionalButtonBlockWidget::onKeyBtnPressed);
        QObject::connect(leftBtn, &QPushButton::clicked, this, &DirectionalButtonBlockWidget::onKeyBtnPressed);
        QObject::connect(rightBtn, &QPushButton::clicked, this, &DirectionalButtonBlockWidget::onKeyBtnPressed);
        QObject::connect(bottomBtn, &QPushButton::clicked, this, &DirectionalButtonBlockWidget::onKeyBtnPressed);
    }

    void DirectionalButtonBlockWidget::setMapping(const std::shared_ptr<Controller> &gpd) const {
        topBtn->setText(QString::fromStdString(gpd->getButton(topOwcButton)));
        leftBtn->setText(QString::fromStdString(gpd->getButton(leftOwcButton)));
        rightBtn->setText(QString::fromStdString(gpd->getButton(rightOwcButton)));
        bottomBtn->setText(QString::fromStdString(gpd->getButton(bottomOwcButton)));
    }

    void DirectionalButtonBlockWidget::writeMapping(const std::shared_ptr<Controller> &gpd) {
        if (!gpd->setButton(topOwcButton, topBtn->text().toStdString()))
            emit logSent(u"failed to set %1"_s.arg(topYmlKey.data()));

        if (!gpd->setButton(leftOwcButton, leftBtn->text().toStdString()))
            emit logSent(u"failed to set %1"_s.arg(leftYmlKey.data()));

        if (!gpd->setButton(rightOwcButton, rightBtn->text().toStdString()))
            emit logSent(u"failed to set %1"_s.arg(rightYmlKey.data()));

        if (!gpd->setButton(bottomOwcButton, bottomBtn->text().toStdString()))
            emit logSent(u"failed to set %1"_s.arg(bottomYmlKey.data()));
    }

    QString DirectionalButtonBlockWidget::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts (&yaml);

        ts << topYmlKey.data() << ": " << topBtn->text() << "\n" <<
            leftYmlKey.data() << ": " << leftBtn->text() << "\n" <<
            rightYmlKey.data() << ": " << rightBtn->text() << "\n" <<
            bottomYmlKey.data() << ": " << bottomBtn->text() << "\n";

        return yaml;
    }

    void DirectionalButtonBlockWidget::importMappingFromYaml(const YAML::Node &yaml) const {
        if (yaml[topYmlKey])
            topBtn->setText(QString::fromStdString(yaml[topYmlKey].as<std::string>()).toUpper());

        if (yaml[leftYmlKey])
            leftBtn->setText(QString::fromStdString(yaml[leftYmlKey].as<std::string>()).toUpper());

        if (yaml[rightYmlKey])
            rightBtn->setText(QString::fromStdString(yaml[rightYmlKey].as<std::string>()).toUpper());

        if (yaml[bottomYmlKey])
            bottomBtn->setText(QString::fromStdString(yaml[bottomYmlKey].as<std::string>()).toUpper());
    }
}
