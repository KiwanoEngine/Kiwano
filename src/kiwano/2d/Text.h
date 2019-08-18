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
#include "Actor.h"
#include "../renderer/TextLayout.h"

namespace kiwano
{
	// 文本
	class KGE_API Text
		: public Actor
	{
	public:
		Text();

		explicit Text(
			String const& text		/* 文字内容 */
		);

		explicit Text(
			String const& text,		/* 文字内容 */
			const Font& font		/* 字体 */
		);

		explicit Text(
			String const& text,		/* 文字内容 */
			const TextStyle& style	/* 文本样式 */
		);

		explicit Text(
			String const& text,		/* 文字内容 */
			const Font& font,		/* 字体 */
			const TextStyle& style	/* 文本样式 */
		);

		virtual ~Text();

		// 获取文本
		inline String const& GetText() const	{ return text_; }

		// 获取字体
		inline Font GetFont() const				{ return font_; }

		// 获取文本样式
		inline TextStyle GetStyle() const		{ return style_; }

		// 获取文本布局
		inline TextLayout GetLayout() const		{ return text_layout_; }

		// 设置文本
		void SetText(
			String const& text
		);

		// 设置文本样式
		void SetStyle(
			const TextStyle& style
		);

		// 设置字体
		void SetFont(
			const Font& font
		);

		// 设置字体族
		void SetFontFamily(
			String const& family
		);

		// 设置字号（默认值为 18）
		void SetFontSize(
			Float32 size
		);

		// 设置字体粗细值（默认值为 FontWeight::Normal）
		void SetFontWeight(
			UInt32 weight
		);

		// 设置文字颜色（默认值为 Color::White）
		void SetColor(
			Color const& color
		);

		// 设置文字斜体（默认值为 false）
		void SetItalic(
			bool val
		);

		// 设置文本自动换行的宽度（默认为 0）
		void SetWrapWidth(
			Float32 wrap_width
		);

		// 设置行间距（默认为 0）
		void SetLineSpacing(
			Float32 line_spacing
		);

		// 设置对齐方式（默认为 TextAlign::Left）
		void SetAlignment(
			TextAlign align
		);

		// 设置下划线（默认值为 false）
		void SetUnderline(
			bool underline
		);

		// 设置删除线（默认值为 false）
		void SetStrikethrough(
			bool strikethrough
		);

		// 设置是否显示描边
		void SetOutline(
			bool outline
		);

		// 设置描边颜色
		void SetOutlineColor(
			Color const& outline_color
		);

		// 设置描边线宽
		void SetOutlineWidth(
			Float32 outline_width
		);

		// 设置描边线相交样式
		void SetOutlineStroke(
			StrokeStyle outline_stroke
		);

		// 设置默认字体
		static void SetDefaultFont(
			Font const& font
		);

		// 设置默认文字样式
		static void SetDefaultStyle(
			TextStyle const& style
		);

		void OnRender(Renderer* renderer) override;

	protected:
		void UpdateLayout();

	protected:
		bool		format_dirty_;
		bool		layout_dirty_;
		TextLayout	text_layout_;
		String		text_;
		Font		font_;
		TextStyle	style_;
	};
}
