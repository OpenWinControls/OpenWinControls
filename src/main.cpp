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
#include <QApplication>

#ifdef _WIN32
#include "include/win.h"
#endif
#include "mainwindow.h"

int main(int argc, char *argv[]) {
#ifdef _WIN32
    DWORD langCount = 2;

    // hidapi uses the preferred user lang to print logs, try to force english, if available
    SetProcessPreferredUILanguages(MUI_LANGUAGE_NAME, L"en-US\0en\0\0", &langCount);
#endif

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
