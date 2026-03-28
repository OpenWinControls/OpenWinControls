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

#include "SingleButtonBlockWidget.h"

namespace OWC {
    SingleButtonBlockWidget::SingleButtonBlockWidget(const Button owcBtn, const std::string_view yamlKey, const QString &icon, const int iconScaleW, const int iconScaleH) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *startIcon = new QLabel();

        owcButton = owcBtn;
        ymlKey = yamlKey;
        keyBtn = new QPushButton();

        keyBtn->setFixedWidth(buttonWidth);
        startIcon->setPixmap(QPixmap(QString(":/icons/%1").arg(icon)).scaled(iconScaleW, iconScaleH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        startIcon->setAlignment(Qt::AlignCenter);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(keyBtn);
        lyt->addWidget(startIcon);

        setLayout(lyt);

        QObject::connect(keyBtn, &QPushButton::clicked, this, &SingleButtonBlockWidget::onKeyBtnPressed);
    }

    void SingleButtonBlockWidget::setMapping(const QSharedPointer<Controller> &gpd) const {
        keyBtn->setText(QString::fromStdString(gpd->getButton(owcButton)));
    }

    void SingleButtonBlockWidget::writeMapping(const QSharedPointer<Controller> &gpd) {
        if (!gpd->setButton(owcButton, keyBtn->text().toStdString()))
            emit logSent(QString("failed to set %1").arg(ymlKey.data()));
    }

    QString SingleButtonBlockWidget::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts (&yaml);

        ts << ymlKey.data() << ": " << keyBtn->text() << "\n";

        return yaml;
    }

    void SingleButtonBlockWidget::importMappingFromYaml(const YAML::Node &yaml) const {
        if (yaml[ymlKey])
            keyBtn->setText(QString::fromStdString(yaml[ymlKey].as<std::string>()).toUpper());
    }
}
