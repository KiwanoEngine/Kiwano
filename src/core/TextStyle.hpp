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
#include "include-forwards.h"

namespace easy2d
{
	// 文本对齐方式
	enum class TextAlign
	{
		Left,		/* 左对齐 */
		Right,		/* 右对齐 */
		Center		/* 居中对齐 */
	};

	// 文本样式
	class TextStyle
	{
	public:
		Color		color;				// 颜色
		TextAlign	alignment;			// 对齐方式
		bool		wrap;				// 打开自动换行
		float		wrap_width;			// 自动换行宽度
		float		line_spacing;		// 行间距
		bool		underline;			// 下划线
		bool		strikethrough;		// 删除线
		bool		outline;			// 显示描边
		Color		outline_color;		// 描边颜色
		float		outline_width;		// 描边线宽
		StrokeStyle	outline_stroke;		// 描边线相交样式

	public:
		TextStyle()
			: color(Color::White)
			, alignment(TextAlign::Left)
			, wrap(false)
			, wrap_width(0.f)
			, line_spacing(0.f)
			, underline(false)
			, strikethrough(false)
			, outline(true)
			, outline_color(Color(Color::Black, 0.5))
			, outline_width(1.f)
			, outline_stroke(StrokeStyle::Round)
		{}

		TextStyle(
			Color color,
			TextAlign alignment			= TextAlign::Left,
			bool wrap					= false,
			float wrap_width			= 0.f,
			float line_spacing			= 0.f,
			bool underline				= false,
			bool strikethrough			= false,
			bool outline				= true,
			Color outline_color			= Color(Color::Black, 0.5),
			float outline_width			= 1.f,
			StrokeStyle outline_stroke	= StrokeStyle::Round
		)
			: color(color)
			, alignment(alignment)
			, wrap(wrap)
			, wrap_width(wrap_width)
			, line_spacing(line_spacing)
			, underline(underline)
			, strikethrough(strikethrough)
			, outline(outline)
			, outline_color(outline_color)
			, outline_width(outline_width)
			, outline_stroke(outline_stroke)
		{}
	};
}