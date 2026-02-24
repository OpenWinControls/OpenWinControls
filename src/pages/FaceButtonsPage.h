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

#include "Widgets/CharMapWidget.h"
#include "../extern/libOpenWinControls/src/controller/Controller.h"
#include "../extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWC {
    class FaceButtonsPage: public QWidget {
        Q_OBJECT

    private:
        static constexpr int buttonWidth = 122;
        QPushButton *backBtn = nullptr;
        QPushButton *resetBtn = nullptr;
        QPushButton *charMapBtn = nullptr;
        CharMapWidget *charMap = nullptr;
        mutable QString oldPendingBtnText; // text backup to restore on cancel

    protected:
        QVBoxLayout *controlsLyt = nullptr;
        QPushButton *dpadUpBtn = nullptr;
        QPushButton *dpadDownBtn = nullptr;
        QPushButton *dpadLeftBtn = nullptr;
        QPushButton *dpadRight = nullptr;
        QPushButton *aBtn = nullptr;
        QPushButton *bBtn = nullptr;
        QPushButton *xBtn = nullptr;
        QPushButton *yBtn = nullptr;
        QPushButton *lAnalogUpBtn = nullptr;
        QPushButton *lAnalogDownBtn = nullptr;
        QPushButton *lAnalogLeftBtn = nullptr;
        QPushButton *lAnalogRightBtn = nullptr;
        QPushButton *startBtn = nullptr;
        QPushButton *selectBtn = nullptr;
        QPushButton *menuBtn = nullptr;
        QPushButton *l1Btn = nullptr;
        QPushButton *l2Btn = nullptr;
        QPushButton *l3Btn = nullptr;
        QPushButton *r1Btn = nullptr;
        QPushButton *r2Btn = nullptr;
        QPushButton *r3Btn = nullptr;
        mutable QPushButton *pendingBtn = nullptr; // clicked, waiting for new key
        QMap<std::string_view, std::pair<QPushButton *, Button>> btnMap;

        [[nodiscard]] QVBoxLayout *makeDirectionalBlock(QPushButton *upBtn, QPushButton *leftBtn,
                                                        QPushButton *rightBtn, QPushButton *downBtn,
                                                        const QString &icon, int iconSize) const;
        [[nodiscard]] QVBoxLayout *makeShoulderBlock(QPushButton *btn1, QPushButton *btn2, const QString &icon1,
                                                     const QString &icon2, int icon1W, int icon1H, int icon2W, int icon2H) const;
        [[nodiscard]] QVBoxLayout *makeSingleBtnBlock(QPushButton *btn, const QString &icon, int iconW, int iconH) const;

    public:
        explicit FaceButtonsPage(CharMapMode charMapMode);

        void setMapping(const QSharedPointer<Controller> &gpd) const;
        [[nodiscard]] QString exportMappingToYaml() const;
        void importMappingFromYaml(const YAML::Node &yaml) const;
        void writeMapping(const QSharedPointer<Controller> &gpd);

    private slots:
        void onBackBtnClicked();
        void onCharMapBtnClicked() const;
        void onCharMapKeyPressed(const QString &key) const;
        void onkeyButtonPressed() const;

    protected slots:
        virtual void onResetBtnClicked() = 0;

    signals:
        void backToHome();
        void logSent(const QString &msg);
    };
}
