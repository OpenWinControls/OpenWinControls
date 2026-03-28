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

#include "ShoulderButtonBlockWidget.h"

namespace OWC {
    ShoulderButtonBlockWidget::ShoulderButtonBlockWidget(const Button topOwcBtn, const Button bottomOwcBtn,
                                            const std::string_view topYamlKey, const std::string_view bottomYamlKey,
                                            const QString &topIcon, const QString &bottomIcon,
                                            const int topIconScaleW, const int topIconScaleH,
                                            const int bottomIconScaleW, const int bottomIconScaleH) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QVBoxLayout *topLyt = new QVBoxLayout();
        QVBoxLayout *bottomLyt = new QVBoxLayout();
        QLabel *topIcn = new QLabel();
        QLabel *bottomIcn = new QLabel();

        topOwcButton = topOwcBtn;
        bottomOwcButton = bottomOwcBtn;
        topYmlKey = topYamlKey;
        bottomYmlKey = bottomYamlKey;
        topBtn = new QPushButton();
        bottomBtn = new QPushButton();

        topBtn->setFixedWidth(buttonWidth);
        bottomBtn->setFixedWidth(buttonWidth);
        topIcn->setPixmap(QPixmap(QString(":/icons/%1").arg(topIcon)).scaled(topIconScaleW, topIconScaleH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        topIcn->setAlignment(Qt::AlignCenter);
        bottomIcn->setPixmap(QPixmap(QString(":/icons/%1").arg(bottomIcon)).scaled(bottomIconScaleW, bottomIconScaleH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        bottomIcn->setAlignment(Qt::AlignCenter);

        topLyt->addWidget(topBtn);
        topLyt->addWidget(topIcn);

        bottomLyt->addWidget(bottomBtn);
        bottomLyt->addWidget(bottomIcn);

        lyt->addLayout(topLyt);
        lyt->addSpacing(8);
        lyt->addLayout(bottomLyt);

        setLayout(lyt);

        QObject::connect(topBtn, &QPushButton::clicked, this, &ShoulderButtonBlockWidget::onKeyBtnPressed);
        QObject::connect(bottomBtn, &QPushButton::clicked, this, &ShoulderButtonBlockWidget::onKeyBtnPressed);
    }

    void ShoulderButtonBlockWidget::setMapping(const QSharedPointer<Controller> &gpd) const {
        topBtn->setText(QString::fromStdString(gpd->getButton(topOwcButton)));
        bottomBtn->setText(QString::fromStdString(gpd->getButton(bottomOwcButton)));
    }

    void ShoulderButtonBlockWidget::writeMapping(const QSharedPointer<Controller> &gpd) {
        if (!gpd->setButton(topOwcButton, topBtn->text().toStdString()))
            emit logSent(QString("failed to set %1").arg(topYmlKey.data()));

        if (!gpd->setButton(bottomOwcButton, bottomBtn->text().toStdString()))
            emit logSent(QString("failed to set %1").arg(bottomYmlKey.data()));
    }

    QString ShoulderButtonBlockWidget::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts (&yaml);

        ts << bottomYmlKey.data() << ": " << bottomBtn->text() << "\n" <<
                topYmlKey.data() << ": " << topBtn->text() << "\n";

        return yaml;
    }

    void ShoulderButtonBlockWidget::importMappingFromYaml(const YAML::Node &yaml) const {
        if (yaml[topYmlKey])
            topBtn->setText(QString::fromStdString(yaml[topYmlKey].as<std::string>()).toUpper());

        if (yaml[bottomYmlKey])
            bottomBtn->setText(QString::fromStdString(yaml[bottomYmlKey].as<std::string>()).toUpper());
    }
}
