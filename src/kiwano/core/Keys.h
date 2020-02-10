// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <kiwano/macros.h>

namespace kiwano
{
/// \~chinese
/// @brief 鼠标按键
enum class MouseButton
{
    Left,    ///< 鼠标左键
    Right,   ///< 鼠标右键
    Middle,  ///< 鼠标中键

    Last
};

/// \~chinese
/// @brief 按键键值
enum class KeyCode
{
    Unknown,  ///< 未知
    Up,       ///< 上键
    Left,     ///< 左键
    Right,    ///< 右键
    Down,     ///< 下键
    Enter,    ///< 回车键
    Space,    ///< 空格键
    Esc,      ///< 退出键
    Ctrl,     ///< CTRL键
    Shift,    ///< SHIFT键
    Alt,      ///< ALT键
    Tab,      ///< TAB键
    Delete,   ///< 删除键
    Back,     ///< 退格键
    Super,    ///< Cmd|Super|Windows键

    A,  ///< A键
    B,  ///< B键
    C,  ///< C键
    D,  ///< D键
    E,  ///< E键
    F,  ///< F键
    G,  ///< G键
    H,  ///< H键
    I,  ///< I键
    J,  ///< J键
    K,  ///< K键
    L,  ///< L键
    M,  ///< M键
    N,  ///< N键
    O,  ///< O键
    P,  ///< P键
    Q,  ///< Q键
    R,  ///< R键
    S,  ///< S键
    T,  ///< T键
    U,  ///< U键
    V,  ///< V键
    W,  ///< W键
    X,  ///< X键
    Y,  ///< Y键
    Z,  ///< Z键

    Num0,  ///< 数字0键
    Num1,  ///< 数字1键
    Num2,  ///< 数字2键
    Num3,  ///< 数字3键
    Num4,  ///< 数字4键
    Num5,  ///< 数字5键
    Num6,  ///< 数字6键
    Num7,  ///< 数字7键
    Num8,  ///< 数字8键
    Num9,  ///< 数字9键

    Numpad0,  ///< 数字小键盘0键
    Numpad1,  ///< 数字小键盘1键
    Numpad2,  ///< 数字小键盘2键
    Numpad3,  ///< 数字小键盘3键
    Numpad4,  ///< 数字小键盘4键
    Numpad5,  ///< 数字小键盘5键
    Numpad6,  ///< 数字小键盘6键
    Numpad7,  ///< 数字小键盘7键
    Numpad8,  ///< 数字小键盘8键
    Numpad9,  ///< 数字小键盘9键

    F1,   ///< F1键
    F2,   ///< F2键
    F3,   ///< F3键
    F4,   ///< F4键
    F5,   ///< F5键
    F6,   ///< F6键
    F7,   ///< F7键
    F8,   ///< F8键
    F9,   ///< F9键
    F10,  ///< F10键
    F11,  ///< F11键
    F12,  ///< F12键

    Last
};
}  // namespace kiwano
