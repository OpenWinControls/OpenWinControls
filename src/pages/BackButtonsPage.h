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

#include <QVBoxLayout>
#include <QSpinBox>
#include <QKeyEvent>

#include "Widgets/CharMapWidget.h"
#include "../extern/libOpenWinControls/src/controller/Controller.h"
#include "../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class BackButtonsPage final: public QWidget {
        Q_OBJECT

    private:
        struct KeySlot final {
            QPushButton *btn = nullptr;
            QSpinBox *startTime = nullptr;
        };

        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QPushButton *charMapBtn = nullptr;
        CharMapWidget *charMap = nullptr;
        QList<KeySlot> lBtnList;
        QList<KeySlot> rBtnList;
        QPushButton *pendingBtn = nullptr;
        QString oldPendingBtnText;

        [[nodiscard]] QVBoxLayout *makeBackButtonUI(const QString &icon, QList<KeySlot> &slotList);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    public:
        BackButtonsPage();

        void setMapping(const QSharedPointer<Controller> &gpd) const;
        [[nodiscard]] QString exportMappingToYaml() const;
        void importMappingFromYaml(const YAML::Node &yaml) const;
        void writeMapping(const QSharedPointer<Controller> &gpd);

    private slots:
        void onBackBtnClicked();
        void onResetBtnClicked();
        void onCharMapBtnClicked() const;
        void onkeyButtonPressed();
        void onCharMapKeyPressed(const QString &key);

    signals:
        void backToHome();
        void resetBackButtons();
        void logSent(const QString &msg);
    };
}
