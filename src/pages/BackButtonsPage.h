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
        static constexpr int msMax = INT16_MAX - 1;
        static constexpr int buttonWidth = 120;
        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QPushButton *charMapBtn = nullptr;
        CharMapWidget *charMap = nullptr;
        QPushButton *l1Btn = nullptr;
        QPushButton *l2Btn = nullptr;
        QPushButton *l3Btn = nullptr;
        QPushButton *l4Btn = nullptr;
        QPushButton *r1Btn = nullptr;
        QPushButton *r2Btn = nullptr;
        QPushButton *r3Btn = nullptr;
        QPushButton *r4Btn = nullptr;
        QSpinBox *l1Ms = nullptr;
        QSpinBox *l2Ms = nullptr;
        QSpinBox *l3Ms = nullptr;
        QSpinBox *l4Ms = nullptr;
        QSpinBox *r1Ms = nullptr;
        QSpinBox *r2Ms = nullptr;
        QSpinBox *r3Ms = nullptr;
        QSpinBox *r4Ms = nullptr;
        QPushButton *pendingBtn = nullptr;
        QString oldPendingBtnText;

        [[nodiscard]] QHBoxLayout *makeLBlock();
        [[nodiscard]] QHBoxLayout *makeRBlock();

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
