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

#include "../Widgets/CharMapWidget.h"
#include "Widgets/ButtonBlockWidget.h"
#include "../../extern/libOpenWinControls/src/controller/Controller.h"
#include "../../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class FaceButtonsPage: public QWidget {
        Q_OBJECT

    private:
        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QPushButton *charMapBtn = nullptr;
        CharMapWidget *charMap = nullptr;
        mutable QString oldPendingBtnText; // text backup to restore on cancel

    protected:
        QVBoxLayout *controlsLyt = nullptr;
        mutable QPushButton *pendingBtn = nullptr; // clicked, waiting for new key
        QList<ButtonBlockWidget *> buttonList;

    public:
        explicit FaceButtonsPage(CharMapMode charMapMode);

        void setMapping(const QSharedPointer<Controller> &gpd) const;
        void writeMapping(const QSharedPointer<Controller> &gpd);
        [[nodiscard]] QString exportMappingToYaml() const;
        void importMappingFromYaml(const YAML::Node &yaml) const;

    private slots:
        void onBackBtnClicked();
        void onCharMapBtnClicked() const;
        void onCharMapKeyPressed(const QString &key) const;

    protected slots:
        virtual void onResetBtnClicked() = 0;
        void onLogSent(const QString &msg);
        void onkeyButtonPressed(QPushButton *btn) const;

    signals:
        void backToHome();
        void logSent(const QString &msg);
    };
}
