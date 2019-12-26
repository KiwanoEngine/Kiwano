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
	* @brief 文本对齐方式
	*/
	enum class TextAlign
	{
		Left,	///< 左对齐
		Right,	///< 右对齐
		Center	///< 居中对齐
	};

	/**
	* \~chinese
	* @brief 字体粗细值
	*/
	struct FontWeight
	{
		enum Value : uint32_t
		{
			Thin		= 100U,
			ExtraLight	= 200U,
			Light		= 300U,
			Normal		= 400U,	///< 正常
			Medium		= 500U,
			Bold		= 700U,
			ExtraBold	= 800U,
			Black		= 900U,
			ExtraBlack	= 950U
		};
	};

	/**
	* \~chinese
	* @brief 文本样式
	*/
	class KGE_API TextStyle
	{
	public:
		FontPtr		font;				///< 字体
		String		font_family;		///< 字体族
		float		font_size;			///< 字号
		uint32_t	font_weight;		///< 粗细值
		bool		italic;				///< 是否斜体
		Color		color;				///< 颜色
		TextAlign	alignment;			///< 对齐方式
		float		wrap_width;			///< 自动换行宽度
		float		line_spacing;		///< 行间距
		bool		outline;			///< 描边
		Color		outline_color;		///< 描边颜色
		float		outline_width;		///< 描边线宽
		StrokeStyle	outline_stroke;		///< 描边线相交样式

	public:
		/**
		* \~chinese
		* @brief 构建文本样式
		* @param font 字体
		* @param font_family 字体族
		* @param font_size 字体大小
		* @param font_weight 字体粗细
		* @param italic 斜体
		* @param color 颜色
		* @param alignment 对齐方式
		* @param wrap_width 自动换行宽度
		* @param line_spacing 行间距
		* @param outline 描边
		* @param outline_color 描边颜色
		* @param outline_width 描边线宽
		* @param outline_stroke 描边线相交样式
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
