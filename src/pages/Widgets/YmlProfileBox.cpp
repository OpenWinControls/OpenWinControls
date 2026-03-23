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

#include "YmlProfileBox.h"

namespace OWC {
    YmlProfileBox::YmlProfileBox(const QString &name, const bool isLocal) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *title = new QLabel(name.length() > 13 ? name.first(13).append("..") : name);

        viewBtn = new QPushButton("View");
        ymlName = name;

        title->setAlignment(Qt::AlignCenter);
        title->setToolTip(name);
        viewBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        lyt->setAlignment(Qt::AlignCenter);
        lyt->addWidget(title);
        lyt->addStretch();
        lyt->addWidget(viewBtn);

        setFixedSize(160, 110);
        setFrameStyle(QFrame::Box);
        setLayout(lyt);

        if (isLocal) {
            deleteBtn = new QPushButton("Delete");

            deleteBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            lyt->addWidget(deleteBtn);
            QObject::connect(deleteBtn, &QPushButton::clicked, this, &YmlProfileBox::onDeleteBtnClicked);
        }

        QObject::connect(viewBtn, &QPushButton::clicked, this, &YmlProfileBox::onViewBtnClicked);
    }

    void YmlProfileBox::onViewBtnClicked() {
        emit viewProfile(ymlName);
    }

    void YmlProfileBox::onDeleteBtnClicked() {
        emit deleteProfile(ymlName, this);
    }
}
