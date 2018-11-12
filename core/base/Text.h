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
#include "Font.h"

namespace easy2d
{
	// 文本
	class Text
		: public Node
	{
		E2D_DISABLE_COPY(Text);

	public:
		// 文本对齐方式
		enum class Align
		{
			Left,		/* 左对齐 */
			Right,		/* 右对齐 */
			Center		/* 居中对齐 */
		};

		// 文本样式
		class Style
		{
		public:
			Color	color;				// 颜色
			Align	alignment;			// 对齐方式
			bool	wrap;				// 打开自动换行
			float	wrap_width;			// 自动换行宽度
			float	line_spacing;		// 行间距
			bool	underline;			// 下划线
			bool	strikethrough;		// 删除线
			bool	outline;			// 显示描边
			Color	outline_color;		// 描边颜色
			float	outline_width;		// 描边线宽
			StrokeStyle	outline_stroke;		// 描边线相交样式

		public:
			Style();

			Style(
				Color color,
				Align alignment			= Align::Left,
				bool wrap				= false,
				float wrap_width		= 0.f,
				float line_spacing		= 0.f,
				bool underline			= false,
				bool strikethrough		= false,
				bool outline			= true,
				Color outline_color		= Color(Color::Black, 0.5),
				float outline_width		= 1.f,
				StrokeStyle outline_stroke = StrokeStyle::Round
			);
		};

	public:
		Text();

		explicit Text(
			const String& text,			/* 文字内容 */
			const Font& font	= Font(),		/* 字体 */
			const Style& style	= Style()		/* 文本样式 */
		);

		virtual ~Text();

		// 获取文本
		const String& GetText() const;

		// 获取字体
		const Font& GetFont() const;

		// 获取文本样式
		const Style& GetStyle() const;

		// 获取字体族
		const String& GetFontFamily() const;

		// 获取当前字号
		float GetFontSize() const;

		// 获取当前字体粗细值
		UINT GetFontWeight() const;

		// 获取文字颜色
		const Color& GetColor() const;

		// 获取描边颜色
		const Color& GetOutlineColor() const;

		// 获取描边线宽
		float GetOutlineWidth() const;

		// 获取描边线相交样式
		StrokeStyle GetOutlineStroke() const;

		// 获取文本显示行数
		int GetLineCount() const;

		// 是否是斜体
		bool IsItalic() const;

		// 是否显示删除线
		bool strikethrough() const;

		// 是否显示下划线
		bool underline() const;

		// 是否显示描边
		bool outline() const;

		// 设置文本
		void SetText(
			const String& text
		);

		// 设置文本样式
		void SetStyle(
			const Style& style
		);

		// 设置字体
		void SetFont(
			const Font& font
		);

		// 设置字体族
		void SetFontFamily(
			const String& family
		);

		// 设置字号（默认值为 22）
		void SetFontSize(
			float size
		);

		// 设置字体粗细值（默认值为 Text::Font::Weight::Normal）
		void SetFontWeight(
			UINT weight
		);

		// 设置文字颜色（默认值为 Color::WHITE）
		void SetColor(
			Color color
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

		// 设置对齐方式（默认为 Align::Left）
		void SetAlignment(
			Align align
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
			Color outline_color
		);

		// 设置描边线宽
		void SetOutlineWidth(
			float outline_width
		);

		// 设置描边线相交样式
		void SetOutlineStroke(
			StrokeStyle outline_stroke
		);

		// 渲染文字
		virtual void OnDraw() const override;

	private:
		// 重新排版文字
		void Reset();

		// 创建文字格式化
		void CreateFormat();

		// 创建文字布局
		void CreateLayout();

	private:
		String		text_;
		Font				font_;
		Style				style_;
		IDWriteTextFormat*	text_format_;
		IDWriteTextLayout*	text_layout_;
	};
}