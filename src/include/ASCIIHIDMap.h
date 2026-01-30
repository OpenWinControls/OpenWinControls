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

#include <QMap>

namespace OWC {
    static const QMap<int, int> ASCIIHIDMap = {
        {Qt::Key_unknown, 0},
        {Qt::Key_A, 0x04},
        {Qt::Key_B, 0x05},
        {Qt::Key_C, 0x06},
        {Qt::Key_D, 0x07},
        {Qt::Key_E, 0x08},
        {Qt::Key_F, 0x09},
        {Qt::Key_G, 0x0A},
        {Qt::Key_H, 0x0B},
        {Qt::Key_I, 0x0C},
        {Qt::Key_J, 0x0D},
        {Qt::Key_K, 0x0E},
        {Qt::Key_L, 0x0F},
        {Qt::Key_M, 0x10},
        {Qt::Key_N, 0x11},
        {Qt::Key_O, 0x12},
        {Qt::Key_P, 0x13},
        {Qt::Key_Q, 0x14},
        {Qt::Key_R, 0x15},
        {Qt::Key_S, 0x16},
        {Qt::Key_T, 0x17},
        {Qt::Key_U, 0x18},
        {Qt::Key_V, 0x19},
        {Qt::Key_W, 0x1A},
        {Qt::Key_X, 0x1B},
        {Qt::Key_Y, 0x1C},
        {Qt::Key_Z, 0x1D},

        {Qt::Key_1, 0x1E},
        {Qt::Key_2, 0x1F},
        {Qt::Key_3, 0x20},
        {Qt::Key_4, 0x21},
        {Qt::Key_5, 0x22},
        {Qt::Key_6, 0x23},
        {Qt::Key_7, 0x24},
        {Qt::Key_8, 0x25},
        {Qt::Key_9, 0x26},
        {Qt::Key_0, 0x27},

        {Qt::Key_Return, 0x28},
        {Qt::Key_Enter, 0x28},
        {Qt::Key_Escape, 0x29},
        {Qt::Key_Backspace, 0x2A},
        {Qt::Key_Tab, 0x2B},
        {Qt::Key_Space, 0x2C},

        {Qt::Key_Minus, 0x2D},
        {Qt::Key_Equal, 0x2E},
        {Qt::Key_BracketLeft, 0x2F},
        {Qt::Key_BracketRight, 0x30},
        {Qt::Key_Backslash, 0x31},
        {Qt::Key_Semicolon, 0x33},
        {Qt::Key_Apostrophe, 0x34},
        {Qt::Key_QuoteLeft, 0x35},
        {Qt::Key_Comma, 0x36},
        {Qt::Key_Period, 0x37},
        {Qt::Key_Slash, 0x38},

        {Qt::Key_CapsLock, 0x39},
        {Qt::Key_F1, 0x3A},
        {Qt::Key_F2, 0x3B},
        {Qt::Key_F3, 0x3C},
        {Qt::Key_F4, 0x3D},
        {Qt::Key_F5, 0x3E},
        {Qt::Key_F6, 0x3F},
        {Qt::Key_F7, 0x40},
        {Qt::Key_F8, 0x41},
        {Qt::Key_F9, 0x42},
        {Qt::Key_F10, 0x43},
        {Qt::Key_F11, 0x44},
        {Qt::Key_F12, 0x45},

        {Qt::Key_Right, 0x4F},
        {Qt::Key_Left, 0x50},
        {Qt::Key_Down, 0x51},
        {Qt::Key_Up, 0x52}
    };
}
