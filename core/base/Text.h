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
#include "Node.h"
#include "Font.hpp"
#include "TextStyle.hpp"

namespace easy2d
{
	// 文本
	class Text
		: public Node
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
		String const& GetText() const;

		// 获取字体
		const Font& GetFont() const;

		// 获取文本样式
		const TextStyle& GetStyle() const;

		// 获取字体族
		String const& GetFontFamily() const;

		// 获取当前字号
		float GetFontSize() const;

		// 获取当前字体粗细值
		unsigned int GetFontWeight() const;

		// 获取文字颜色
		const Color& GetColor() const;

		// 获取描边颜色
		const Color& GetOutlineColor() const;

		// 获取描边线宽
		float GetOutlineWidth() const;

		// 获取描边线相交样式
		StrokeStyle GetOutlineStroke() const;

		// 获取文本显示行数
		int GetLineCount();

		// 是否是斜体
		bool IsItalic() const;

		// 是否显示删除线
		bool HasStrikethrough() const;

		// 是否显示下划线
		bool HasUnderline() const;

		// 是否显示描边
		bool HasOutline() const;

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

		// 设置字号（默认值为 22）
		void SetFontSize(
			float size
		);

		// 设置字体粗细值（默认值为 FontWeight::Normal）
		void SetFontWeight(
			unsigned int weight
		);

		// 设置文字颜色（默认值为 Color::White）
		void SetColor(
			Color const& color
		);

		// 设置文字斜体（默认值为 false）
		void SetItalic(
			bool val
		);

		// 打开或关闭文本自动换行（默认为关闭）
		void SetWrapEnabled(
			bool wrap
		);

		// 设置文本自动换行的宽度（默认为 0）
		void SetWrapWidth(
			float wrap_width
		);

		// 设置行间距（默认为 0）
		void SetLineSpacing(
			float line_spacing
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
			float outline_width
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

		virtual void OnRender() override;

	protected:
		void UpdateLayout();

	protected:
		String			text_;
		Font			font_;
		TextStyle		style_;
		cpTextFormat	text_format_;
		cpTextLayout	text_layout_;
	};
}