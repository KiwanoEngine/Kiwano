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
#include <kiwano/renderer/Font.h>

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
	* @brief �����ϸֵ
	*/
	struct FontWeight
	{
		enum Value : uint32_t
		{
			Thin		= 100U,
			ExtraLight	= 200U,
			Light		= 300U,
			Normal		= 400U,	///< ����
			Medium		= 500U,
			Bold		= 700U,
			ExtraBold	= 800U,
			Black		= 900U,
			ExtraBlack	= 950U
		};
	};

	/**
	* \~chinese
	* @brief �ı���ʽ
	*/
	class KGE_API TextStyle
	{
	public:
		FontPtr		font;				///< ����
		String		font_family;		///< ������
		float		font_size;			///< �ֺ�
		uint32_t	font_weight;		///< ��ϸֵ
		bool		italic;				///< �Ƿ�б��
		Color		color;				///< ��ɫ
		TextAlign	alignment;			///< ���뷽ʽ
		float		wrap_width;			///< �Զ����п��
		float		line_spacing;		///< �м��
		bool		outline;			///< ���
		Color		outline_color;		///< �����ɫ
		float		outline_width;		///< ����߿�
		StrokeStyle	outline_stroke;		///< ������ཻ��ʽ

	public:
		/**
		* \~chinese
		* @brief �����ı���ʽ
		* @param font ����
		* @param font_family ������
		* @param font_size �����С
		* @param font_weight �����ϸ
		* @param italic б��
		* @param color ��ɫ
		* @param alignment ���뷽ʽ
		* @param wrap_width �Զ����п��
		* @param line_spacing �м��
		* @param outline ���
		* @param outline_color �����ɫ
		* @param outline_width ����߿�
		* @param outline_stroke ������ཻ��ʽ
		*/
		TextStyle(
			FontPtr			font			= nullptr,
			const String&	font_family		= String(),
			float			font_size		= 18,
			uint32_t		font_weight		= FontWeight::Normal,
			bool			italic			= false,
			Color			color			= Color::White,
			TextAlign		alignment		= TextAlign::Left,
			float			wrap_width		= 0.f,
			float			line_spacing	= 0.f,
			bool			outline			= false,
			Color			outline_color	= Color(Color::Black, 0.5),
			float			outline_width	= 1.f,
			StrokeStyle		outline_stroke	= StrokeStyle::Round
		)
			: font(nullptr)
			, font_family(font_family)
			, font_size(font_size)
			, font_weight(font_weight)
			, italic(italic)
			, color(color)
			, alignment(alignment)
			, wrap_width(wrap_width)
			, line_spacing(line_spacing)
			, outline(outline)
			, outline_color(outline_color)
			, outline_width(outline_width)
			, outline_stroke(outline_stroke)
		{}
	};
}
