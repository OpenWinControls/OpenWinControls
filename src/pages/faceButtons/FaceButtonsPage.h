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

#include "Widgets/ButtonBlockWidget.h"
#include "../../extern/libOpenWinControls/src/controller/Controller.h"
#include "../../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class FaceButtonsPage: public QWidget {
        Q_OBJECT

    private:
        std::shared_ptr<Controller> controller;
        mutable QString oldPendingBtnText; // text backup to restore on cancel

    protected:
        QVBoxLayout *controlsLyt = nullptr;
        mutable QPushButton *pendingBtn = nullptr; // clicked, waiting for new key
        QList<ButtonBlockWidget *> buttonList;

    public:
        explicit FaceButtonsPage(const std::shared_ptr<Controller> &gpd);

        void writeMapping();
        [[nodiscard]] QString exportToYaml() const;
        void importFromYaml(const YAML::Node &yaml) const;
        void setPendingButton(const QString &key) const;

    private slots:
        void onBackBtnClicked();
        void onCharMapBtnClicked();

    protected slots:
        void onLogSent(const QString &msg);
        void onkeyButtonPressed(QPushButton *btn) const;

    public slots:
        void refresh() const;

    signals:
        void backToHome();
        void showCharMap();
        void logSent(const QString &msg);
    };
}
