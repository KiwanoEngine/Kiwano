// Copyright (c) 2016-2019 Kiwano - Nomango
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
#include "../math/Rect.hpp"

namespace kiwano
{
	// 线条样式
	enum class StrokeStyle : int
	{
		Miter = 0,	/* 斜切 */
		Bevel = 1,	/* 斜角 */
		Round = 2	/* 圆角 */
	};

	// 方向
	enum class Direction : int
	{
		Up,		/* 上 */
		Down,	/* 下 */
		Left,	/* 左 */
		Right	/* 右 */
	};

	// 鼠标指针
	enum class MouseCursor : int
	{
		Arrow,		/* 指针 */
		TextInput,	/* 输入文本 */
		Hand,		/* 手指 */
		SizeAll,
		SizeNESW,
		SizeNS,
		SizeNWSE,
		SizeWE,
	};

	// 文字抗锯齿属性
	enum class TextAntialias
	{
		Default,	// 系统默认
		ClearType,	// ClearType 抗锯齿
		GrayScale,	// 灰度抗锯齿
		None		// 不启用抗锯齿
	};

	// 图层属性
	struct LayerProperties
	{
		Rect area;
		float opacity;
	};
}
