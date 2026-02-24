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
#include <QKeyEvent>

#include "Widgets/CharMapWidget.h"
#include "../extern/libOpenWinControls/src/controller/Controller.h"
#include "../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class BackButtonsPage: public QWidget {
        Q_OBJECT

    private:
        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QPushButton *charMapBtn = nullptr;
        CharMapWidget *charMap = nullptr;
        mutable QPushButton *pendingBtn = nullptr; // clicked, waiting for new key
        mutable QString oldPendingBtnText; // text backup to restore on cancel

    protected:
        QHBoxLayout *backBtnLyt = nullptr;

        void keyPressEvent(QKeyEvent *event) override;

    public:
        BackButtonsPage(const QString &helpLbl, CharMapMode charMapMode);

        void setGamepadKey(const QString &key) const;
        virtual void setMapping(const QSharedPointer<Controller> &gpd) const = 0;
        [[nodiscard]] virtual QString exportMappingToYaml() const = 0;
        virtual void importMappingFromYaml(const YAML::Node &yaml) const = 0;
        virtual void writeMapping(const QSharedPointer<Controller> &gpd) = 0;

    private slots:
        void onBackBtnClicked();
        void onResetBtnClicked();
        void onCharMapBtnClicked() const;
        void onCharMapKeyPressed(const QString &key) const;

    protected slots:
        void onkeyButtonPressed() const;

    signals:
        void backToHome();
        void resetBackButtons();
        void logSent(const QString &msg);
    };
}
