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
#include <QScrollBar>
#include <QScrollArea>
#include <QScroller>

#include "FaceButtonsPage.h"

namespace OWC {
    using namespace Qt::StringLiterals;

    FaceButtonsPage::FaceButtonsPage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *buttonsLyt = new QHBoxLayout();
        QScrollArea *scrollArea = new QScrollArea();
        QPushButton *backBtn = new QPushButton(u"Home"_s);
        QPushButton *resetBtn = new QPushButton(u"Reset"_s);
        QPushButton *charMapBtn = new QPushButton(u"Char Map"_s);

        controlsLyt = new QVBoxLayout();

        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

        buttonsLyt->addStretch();
        buttonsLyt->addWidget(charMapBtn);
        buttonsLyt->addWidget(backBtn);
        buttonsLyt->addWidget(resetBtn);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addSpacing(12);
        lyt->addWidget(scrollArea);
        lyt->addLayout(buttonsLyt);

        scrollArea->widget()->setLayout(controlsLyt);
        setLayout(lyt);

        QObject::connect(backBtn, &QPushButton::clicked, this, &FaceButtonsPage::onBackBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &FaceButtonsPage::onResetBtnClicked);
        QObject::connect(charMapBtn, &QPushButton::clicked, this, &FaceButtonsPage::onCharMapBtnClicked);
    }

    void FaceButtonsPage::setMapping(const QSharedPointer<Controller> &gpd) const {
        for (const ButtonBlockWidget *btn: buttonList)
            btn->setMapping(gpd);
    }

    void FaceButtonsPage::writeMapping(const QSharedPointer<Controller> &gpd) {
        for (ButtonBlockWidget *btn: buttonList)
            btn->writeMapping(gpd);
    }

    QString FaceButtonsPage::exportMappingToYaml() const {
        QString yaml;
        QTextStream ts (&yaml);

        for (const ButtonBlockWidget *btn: buttonList)
            ts << btn->exportMappingToYaml();

        return yaml;
    }

    void FaceButtonsPage::importMappingFromYaml(const YAML::Node &yaml) const {
        for (const ButtonBlockWidget *btn: buttonList)
            btn->importMappingFromYaml(yaml);
    }

    void FaceButtonsPage::setPendingButton(const QString &key) const {
        if (pendingBtn == nullptr)
            return;

        pendingBtn->setText(key);
        pendingBtn = nullptr;
    }

    void FaceButtonsPage::onBackBtnClicked() {
        emit backToHome();
    }

    void FaceButtonsPage::onCharMapBtnClicked() {
        emit showCharMap();
    }

    void FaceButtonsPage::onLogSent(const QString &msg) {
        emit logSent(msg);
    }

    void FaceButtonsPage::onkeyButtonPressed(QPushButton *btn) const {
        if (pendingBtn != nullptr) {
            pendingBtn->setText(oldPendingBtnText);

            if (pendingBtn == btn) { // cancel edit
                pendingBtn = nullptr;
                return;
            }
        }

        pendingBtn = btn;
        oldPendingBtnText = pendingBtn->text();

        pendingBtn->setText(u"..."_s);
    }
}
