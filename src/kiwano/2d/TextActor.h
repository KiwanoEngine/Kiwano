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
#include <kiwano/2d/Actor.h>
#include <kiwano/renderer/Color.h>
#include <kiwano/renderer/TextLayout.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(TextActor);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief 文本角色
	*/
	class KGE_API TextActor
		: public Actor
	{
	public:
		/// \~chinese
		/// @brief 构建空的文本角色
		TextActor();

		/// \~chinese
		/// @brief 构建文本角色
		/// @param text 文字内容
		explicit TextActor(const String& text);

		/// \~chinese
		/// @brief 构建文本角色
		/// @param text 文字内容
		/// @param style 文本样式
		TextActor(const String& text, const TextStyle& style);

		virtual ~TextActor();

		/// \~chinese
		/// @brief 获取文本
		const String& GetText() const;

		/// \~chinese
		/// @brief 获取文本样式
		const TextStyle& GetStyle() const;

		/// \~chinese
		/// @brief 获取文本布局
		const TextLayout& GetLayout() const;

		/// \~chinese
		/// @brief 获取字体
		FontPtr GetFont() const;

		/// \~chinese
		/// @brief 设置文本
		void SetText(String const& text);

		/// \~chinese
		/// @brief 设置文本样式
		void SetStyle(const TextStyle& style);

		/// \~chinese
		/// @brief 设置字体
		void SetFont(FontPtr font);

		/// \~chinese
		/// @brief 设置字体族
		void SetFontFamily(String const& family);

		/// \~chinese
		/// @brief 设置字号（默认值为 18）
		void SetFontSize(float size);

		/// \~chinese
		/// @brief 设置字体粗细值（默认值为 FontWeight::Normal）
		void SetFontWeight(uint32_t weight);

		/// \~chinese
		/// @brief 设置文字颜色（默认值为 Color::White）
		void SetColor(Color const& color);

		/// \~chinese
		/// @brief 设置文字斜体（默认值为 false）
		void SetItalic(bool italic);

		/// \~chinese
		/// @brief 设置文本自动换行的宽度（默认为 0）
		void SetWrapWidth(float wrap_width);

		/// \~chinese
		/// @brief 设置行间距（默认为 0）
		void SetLineSpacing(float line_spacing);

		/// \~chinese
		/// @brief 设置对齐方式（默认为 TextAlign::Left）
		void SetAlignment(TextAlign align);

		/// \~chinese
		/// @brief 设置是否显示描边
		void SetOutline(bool enable);

		/// \~chinese
		/// @brief 设置描边颜色
		void SetOutlineColor(Color const& outline_color);

		/// \~chinese
		/// @brief 设置描边线宽
		void SetOutlineWidth(float outline_width);

		/// \~chinese
		/// @brief 设置描边线相交样式
		void SetOutlineStroke(StrokeStyle outline_stroke);

		/// \~chinese
		/// @brief 设置是否显示下划线（默认值为 false）
		void SetUnderline(bool enable);

		/// \~chinese
		/// @brief 设置是否显示删除线（默认值为 false）
		void SetStrikethrough(bool enable);

		/// \~chinese
		/// @brief 更新文本布局
		void UpdateLayout();

		/// \~chinese
		/// @brief 设置默认文字样式
		static void SetDefaultStyle(TextStyle const& style);

		void OnRender(RenderTarget* rt) override;

	private:
		TextLayout	text_layout_;
	};

	/** @} */

	inline const String& TextActor::GetText() const
	{
		return text_layout_.GetText();
	}

	inline FontPtr TextActor::GetFont() const
	{
		return text_layout_.GetStyle().font;
	}

	inline const TextStyle& TextActor::GetStyle() const
	{
		return text_layout_.GetStyle();
	}

	inline const TextLayout& TextActor::GetLayout() const
	{
		return text_layout_;
	}

	inline void TextActor::SetText(String const& text)
	{
		text_layout_.SetText(text);
	}

	inline void TextActor::SetStyle(const TextStyle& style)
	{
		text_layout_.SetStyle(style);
	}

	inline void TextActor::SetFont(FontPtr font)
	{
		text_layout_.SetFont(font);
	}

	inline void TextActor::SetFontFamily(String const& family)
	{
		text_layout_.SetFontFamily(family);
	}

	inline void TextActor::SetFontSize(float size)
	{
		text_layout_.SetFontSize(size);
	}

	inline void TextActor::SetFontWeight(uint32_t weight)
	{
		text_layout_.SetFontWeight(weight);
	}

	inline void TextActor::SetItalic(bool italic)
	{
		text_layout_.SetItalic(italic);
	}

	inline void TextActor::SetWrapWidth(float wrap_width)
	{
		text_layout_.SetWrapWidth(wrap_width);
	}

	inline void TextActor::SetLineSpacing(float line_spacing)
	{
		text_layout_.SetLineSpacing(line_spacing);
	}

	inline void TextActor::SetAlignment(TextAlign align)
	{
		text_layout_.SetAlignment(align);
	}

	inline void TextActor::SetUnderline(bool enable)
	{
		text_layout_.SetUnderline(enable, 0, text_layout_.GetText().length());
	}

	inline void TextActor::SetStrikethrough(bool enable)
	{
		text_layout_.SetStrikethrough(enable, 0, text_layout_.GetText().length());
	}

	inline void TextActor::SetColor(Color const& color)
	{
		text_layout_.SetColor(color);
	}

	inline void TextActor::SetOutline(bool enable)
	{
		text_layout_.SetOutline(enable);
	}

	inline void TextActor::SetOutlineColor(Color const& outline_color)
	{
		text_layout_.SetOutlineColor(outline_color);
	}

	inline void TextActor::SetOutlineWidth(float outline_width)
	{
		text_layout_.SetOutlineWidth(outline_width);
	}

	inline void TextActor::SetOutlineStroke(StrokeStyle outline_stroke)
	{
		text_layout_.SetOutlineStroke(outline_stroke);
	}

}
