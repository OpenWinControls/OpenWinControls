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

#include <QPushButton>
#include <QSpinBox>

namespace OWC {
    class KeySlotV1Widget final: public QWidget {
        Q_OBJECT

    private:
        QPushButton *keySlotBtn = nullptr;
        QSpinBox *startTime = nullptr;

    public:
        explicit KeySlotV1Widget(int slotNum);

        void setKey(const QString &key) const { keySlotBtn->setText(key); }
        [[nodiscard]] QString getKey() const { return keySlotBtn->text(); }
        void setStartTime(const int time) const { startTime->setValue(time); }
        [[nodiscard]] int getStartTime() const { return startTime->value(); }

    private slots:
        void onKeySlotBtnClicked();

    signals:
        void pendingEditBtn(QPushButton *keyBtn);
    };
}
