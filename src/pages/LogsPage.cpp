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
#include <QScroller>

#include "LogsPage.h"

namespace OWC {
    LogsPage::LogsPage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonLyt = new QHBoxLayout();

        clearBtn = new QPushButton("Clear");
        backBtn = new QPushButton("Home");
        logContainer = new QTextEdit();

        logContainer->setWordWrapMode(QTextOption::NoWrap);
        logContainer->setReadOnly(true);
        logContainer->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
        QScroller::grabGesture(logContainer->viewport(), QScroller::LeftMouseButtonGesture);

        buttonLyt->addWidget(backBtn);
        buttonLyt->addStretch();
        buttonLyt->addWidget(clearBtn);
        lyt->addWidget(logContainer);
        lyt->addLayout(buttonLyt);

        setLayout(lyt);

        QObject::connect(clearBtn, &QPushButton::clicked, this, &LogsPage::onClearBtnClicked);
        QObject::connect(backBtn, &QPushButton::clicked, this, &LogsPage::onBackBtnClicked);
    }

    void LogsPage::writeLog(const QString &msg) const {
        logContainer->append(msg);
    }

    void LogsPage::onClearBtnClicked() const {
        logContainer->clear();
    }

    void LogsPage::onBackBtnClicked() {
        emit backToHome();
    }
}
