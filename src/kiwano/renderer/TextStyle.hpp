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
#include <kiwano/renderer/Font.h>
#include <kiwano/renderer/Brush.h>
#include <kiwano/renderer/StrokeStyle.h>

namespace kiwano
{
	/**
	* \addtogroup Render
	* @{
	*/

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
		TextAlign	alignment;			///< ���뷽ʽ
		float		wrap_width;			///< �Զ����п��
		float		line_spacing;		///< �м��
		BrushPtr	fill_brush;			///< ��仭ˢ
		BrushPtr	outline_brush;		///< ��߻�ˢ
		float		outline_width;		///< ����߿�
		StrokeStyle	outline_stroke;		///< �������ʽ

	public:
		/**
		* \~chinese
		* @brief ����Ĭ���ı���ʽ
		*/
		TextStyle();

		/**
		* \~chinese
		* @brief �����ı���ʽ
		* @param font_family ������
		* @param font_size �����С
		* @param font_weight �����ϸ
		*/
		TextStyle(const String& font_family, float font_size, uint32_t font_weight = FontWeight::Normal);
	};

	/** @} */

	inline TextStyle::TextStyle()
		: TextStyle(String(), 18, FontWeight::Normal)
	{
	}

	inline TextStyle::TextStyle(const String& font_family, float font_size, uint32_t font_weight)
		: font(nullptr)
		, font_family(font_family)
		, font_size(font_size)
		, font_weight(font_weight)
		, italic(false)
		, alignment(TextAlign::Left)
		, wrap_width(0)
		, line_spacing(0)
		, outline_width(1.0f)
		, outline_stroke()
	{
	}

}
