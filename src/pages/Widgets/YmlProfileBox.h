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

#include <QFrame>
#include <QPushButton>

namespace OWC {
    class YmlProfileBox final: public QFrame {
        Q_OBJECT

    private:
        QPushButton *viewBtn = nullptr;
        QPushButton *deleteBtn = nullptr;
        QString ymlName;

    public:
        YmlProfileBox(const QString &name, bool isLocal);

    private slots:
        void onViewBtnClicked();
        void onDeleteBtnClicked();

    signals:
        void viewProfile(const QString &name);
        void deleteProfile(const QString &name, const YmlProfileBox *wdg);
    };
}
