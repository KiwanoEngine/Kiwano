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
#include <kiwano/renderer/Color.h>
#include <kiwano/renderer/StrokeStyle.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief �ı����뷽ʽ
	*/
	enum class TextAlign
	{
		Left,	///< �����
		Right,	///< �Ҷ���
		Center	///< ���ж���
	};

	/**
	* \~chinese
	* @brief �ı���ʽ
	*/
	class KGE_API TextStyle
	{
	public:
		Color		color;				///< ��ɫ
		TextAlign	alignment;			///< ���뷽ʽ
		float		wrap_width;			///< �Զ����п��
		float		line_spacing;		///< �м��
		bool		underline;			///< �»���
		bool		strikethrough;		///< ɾ����
		bool		outline;			///< ���
		Color		outline_color;		///< �����ɫ
		float		outline_width;		///< ����߿�
		StrokeStyle	outline_stroke;		///< ������ཻ��ʽ

	public:
		/**
		* \~chinese
		* @brief �����ı���ʽ
		* @param color ��ɫ
		* @param alignment ���뷽ʽ
		* @param wrap_width �Զ����п��
		* @param line_spacing �м��
		* @param underline �»���
		* @param strikethrough ɾ����
		* @param outline ���
		* @param outline_color �����ɫ
		* @param outline_width ����߿�
		* @param outline_stroke ������ཻ��ʽ
		*/
		TextStyle(
			Color color					= Color::White,
			TextAlign alignment			= TextAlign::Left,
			float wrap_width			= 0.f,
			float line_spacing			= 0.f,
			bool underline				= false,
			bool strikethrough			= false,
			bool outline				= false,
			Color outline_color			= Color(Color::Black, 0.5),
			float outline_width			= 1.f,
			StrokeStyle outline_stroke	= StrokeStyle::Round
		)
			: color(color)
			, alignment(alignment)
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
