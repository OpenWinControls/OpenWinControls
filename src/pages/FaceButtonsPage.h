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
    class FaceButtonsPage final: public QWidget {
        Q_OBJECT

    private:
        static constexpr int buttonWidth = 122;
        QMap<std::string_view, std::pair<QPushButton *, Button>> btnMap;
        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QPushButton *charMapBtn = nullptr;
        CharMapWidget *charMap = nullptr;
        QPushButton *dpadUpBtn = nullptr;
        QPushButton *dpadDownBtn = nullptr;
        QPushButton *dpadLeftBtn = nullptr;
        QPushButton *dpadRight = nullptr;
        QPushButton *aBtn = nullptr;
        QPushButton *bBtn = nullptr;
        QPushButton *xBtn = nullptr;
        QPushButton *yBtn = nullptr;
        QPushButton *LAnalogUpBtn = nullptr;
        QPushButton *LAnalogDownBtn = nullptr;
        QPushButton *LAnalogLeftBtn = nullptr;
        QPushButton *LAnalogRightBtn = nullptr;
        QPushButton *startBtn = nullptr;
        QPushButton *selectBtn = nullptr;
        QPushButton *menuBtn = nullptr;
        QPushButton *l1Btn = nullptr;
        QPushButton *l2Btn = nullptr;
        QPushButton *l3Btn = nullptr;
        QPushButton *r1Btn = nullptr;
        QPushButton *r2Btn = nullptr;
        QPushButton *r3Btn = nullptr;
        QPushButton *pendingBtn = nullptr; // clicked, waiting for new key
        QString oldPendingBtnText; // text backup to restore on cancel

        [[nodiscard]] QVBoxLayout *makeDPadBlock();
        [[nodiscard]] QVBoxLayout *makeLeftAnalogBlock();
        [[nodiscard]] QVBoxLayout *makeActionsBlock();
        [[nodiscard]] QVBoxLayout *makeL1L2Block();
        [[nodiscard]] QVBoxLayout *makeR1R2Block();
        [[nodiscard]] QVBoxLayout *makeStartBlock();
        [[nodiscard]] QVBoxLayout *makeSelectBlock();
        [[nodiscard]] QVBoxLayout *makeMenuBlock();
        [[nodiscard]] QVBoxLayout *makeL3Block();
        [[nodiscard]] QVBoxLayout *makeR3Block();

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    public:
        FaceButtonsPage();

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
        void resetFaceButtons();
        void logSent(const QString &msg);
    };
}
