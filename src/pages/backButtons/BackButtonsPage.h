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
#include <QPushButton>

#include "../../extern/libOpenWinControls/src/controller/Controller.h"
#include "../../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class BackButtonsPage: public QWidget {
        Q_OBJECT

    private:
        mutable QPushButton *pendingBtn = nullptr; // clicked, waiting for new key
        mutable QString oldPendingBtnText; // text backup to restore on cancel

    protected:
        QHBoxLayout *backBtnLyt = nullptr;

        void keyReleaseEvent(QKeyEvent *event) override;

    public:
        explicit BackButtonsPage(const QString &helpLbl);

        void setPendingButton(const QString &key) const;
        virtual void writeMapping() = 0;
        [[nodiscard]] virtual QString exportToYaml() const = 0;
        virtual void importFromYaml(const YAML::Node &yaml) const = 0;

    private slots:
        void onBackBtnClicked();
        void onCharMapBtnClicked();

    protected slots:
        void onkeyButtonPressed(QPushButton *btn) const;

    public slots:
        virtual void refresh() const = 0;

    signals:
        void backToHome();
        void showCharMap();
        void logSent(const QString &msg);
    };
}
