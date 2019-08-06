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
	// �ı����뷽ʽ
	enum class TextAlign
	{
		Left,		/* ����� */
		Right,		/* �Ҷ��� */
		Center		/* ���ж��� */
	};

	// �ı���ʽ
	class TextStyle
	{
	public:
		Color		color;				// ��ɫ
		TextAlign	alignment;			// ���뷽ʽ
		bool		wrap;				// ���Զ�����
		float		wrap_width;			// �Զ����п��
		float		line_spacing;		// �м��
		bool		underline;			// �»���
		bool		strikethrough;		// ɾ����
		bool		outline;			// ��ʾ���
		Color		outline_color;		// �����ɫ
		float		outline_width;		// ����߿�
		StrokeStyle	outline_stroke;		// ������ཻ��ʽ

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