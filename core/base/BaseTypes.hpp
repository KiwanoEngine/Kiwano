// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "macros.h"
#include "Color.h"
#include "Size.h"
#include "Rect.hpp"
#include "../math/vector.hpp"

namespace easy2d
{
	// 坐标
	//
	// Usage:
	//     表示一个二维空间的坐标: Point origin(0, 0);
	//     计算两点间距离: float distance = p1.Distance(p2);
	//     坐标可以相加减: Point p = Point(10, 10) + Point(20, 20);  // p 的坐标是 (30, 30)
	//
	using Point = math::Vector2;

	using String = std::wstring;

	// 方向
	enum class Direction : int
	{
		Up,		/* 上 */
		Down,	/* 下 */
		Left,	/* 左 */
		Right	/* 右 */
	};

	// 画笔样式
	enum class StrokeStyle : int
	{
		Miter = 0,	/* 斜切 */
		Bevel = 1,	/* 斜角 */
		Round = 2	/* 圆角 */
	};

	// 键盘键值
	enum class KeyCode : int
	{
		Unknown	= 0,
		Up		= VK_UP,
		Left	= VK_LEFT,
		Right	= VK_RIGHT,
		Down	= VK_DOWN,
		Enter	= VK_RETURN,
		Space	= VK_SPACE,
		Esc		= VK_ESCAPE,
		Ctrl	= VK_CONTROL,
		Shift	= VK_SHIFT,

		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Num0 = 0x30,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,

		F1 = VK_F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
	};

	// 鼠标键值
	enum class MouseCode : int
	{
		Left	= VK_LBUTTON,	/* 鼠标左键 */
		Right	= VK_RBUTTON,	/* 鼠标右键 */
		Middle	= VK_MBUTTON	/* 鼠标中键 */
	};

	// 图层属性
	struct LayerProperties
	{
		Rect area;
		float opacity;
	};
}
