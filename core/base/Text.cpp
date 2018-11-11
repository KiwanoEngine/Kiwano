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

#include "Text.h"
#include "render.h"
#include "base.h"

namespace easy2d
{
	//-------------------------------------------------------
	// Style
	//-------------------------------------------------------

	Text::Style::Style()
		: color(Color::White)
		, alignment(Align::Left)
		, wrap(false)
		, wrap_width(0.f)
		, line_spacing(0.f)
		, underline(false)
		, strikethrough(false)
		, outline(true)
		, outline_color(Color(Color::Black, 0.5))
		, outline_width(1.f)
		, outline_stroke(Stroke::Round)
	{}

	Text::Style::Style(
		Color color,
		Align alignment,
		bool wrap,
		float wrap_width,
		float line_spacing,
		bool underline,
		bool strikethrough,
		bool outline,
		Color outline_color,
		float outline_width,
		Stroke outline_stroke
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



	//-------------------------------------------------------
	// Text
	//-------------------------------------------------------

	Text::Text()
		: font_()
		, style_()
		, text_layout_(nullptr)
		, text_format_(nullptr)
	{
	}

	Text::Text(const String & text, const Font & font, const Style & style)
		: font_(font)
		, style_(style)
		, text_layout_(nullptr)
		, text_format_(nullptr)
		, text_(text)
	{
		Reset();
	}

	Text::~Text()
	{
		SafeRelease(text_format_);
		SafeRelease(text_layout_);
	}

	const String& Text::GetText() const
	{
		return text_;
	}

	const Font& Text::GetFont() const
	{
		return font_;
	}

	const Text::Style& Text::GetStyle() const
	{
		return style_;
	}

	const String& Text::GetFontFamily() const
	{
		return font_.family;
	}

	float Text::GetFontSize() const
	{
		return font_.size;
	}

	UINT Text::GetFontWeight() const
	{
		return font_.weight;
	}

	const Color& Text::GetColor() const
	{
		return style_.color;
	}

	const Color& Text::GetOutlineColor() const
	{
		return style_.outline_color;
	}

	float Text::GetOutlineWidth() const
	{
		return style_.outline_width;
	}

	Stroke Text::GetOutlineStroke() const
	{
		return style_.outline_stroke;
	}

	int Text::GetLineCount() const
	{
		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			text_layout_->GetMetrics(&metrics);
			return static_cast<int>(metrics.lineCount);
		}
		else
		{
			return 0;
		}
	}

	bool Text::IsItalic() const
	{
		return font_.italic;
	}

	bool Text::strikethrough() const
	{
		return style_.strikethrough;
	}

	bool Text::underline() const
	{
		return style_.underline;
	}

	bool Text::outline() const
	{
		return style_.outline;
	}

	void Text::SetText(const String& text)
	{
		text_ = text;
		Reset();
	}

	void Text::SetStyle(const Style& style)
	{
		style_ = style;
		Reset();
	}

	void Text::SetFont(const Font & font)
	{
		font_ = font;
		Reset();
	}

	void Text::SetFontFamily(const String& family)
	{
		font_.family = family;
		Reset();
	}

	void Text::SetFontSize(float size)
	{
		font_.size = size;
		Reset();
	}

	void Text::SetFontWeight(UINT weight)
	{
		font_.weight = weight;
		Reset();
	}

	void Text::SetColor(Color color)
	{
		style_.color = color;
	}

	void Text::SetItalic(bool val)
	{
		font_.italic = val;
		Reset();
	}

	void Text::SetWrapEnabled(bool wrap)
	{
		if (style_.wrap != wrap)
		{
			style_.wrap = wrap;
			Reset();
		}
	}

	void Text::SetWrapWidth(float wrap_width)
	{
		if (style_.wrap_width != wrap_width)
		{
			style_.wrap_width = std::max(wrap_width, 0.f);

			if (style_.wrap)
			{
				Reset();
			}
		}
	}

	void Text::SetLineSpacing(float line_spacing)
	{
		if (style_.line_spacing != line_spacing)
		{
			style_.line_spacing = line_spacing;
			Reset();
		}
	}

	void Text::SetAlignment(Align align)
	{
		if (style_.alignment != align)
		{
			style_.alignment = align;
			Reset();
		}
	}

	void Text::SetUnderline(bool underline)
	{
		if (style_.underline != underline)
		{
			style_.underline = underline;
			if (!text_format_)
				CreateFormat();
			CreateLayout();
		}
	}

	void Text::SetStrikethrough(bool strikethrough)
	{
		if (style_.strikethrough != strikethrough)
		{
			style_.strikethrough = strikethrough;
			if (!text_format_)
				CreateFormat();
			CreateLayout();
		}
	}

	void Text::SetOutline(bool outline)
	{
		style_.outline = outline;
	}

	void Text::SetOutlineColor(Color outline_color)
	{
		style_.outline_color = outline_color;
	}

	void Text::SetOutlineWidth(float outline_width)
	{
		style_.outline_width = outline_width;
	}

	void Text::SetOutlineStroke(Stroke outline_stroke)
	{
		style_.outline_stroke = outline_stroke;
	}

	void Text::OnDraw() const
	{
		if (text_layout_)
		{
			// 创建文本区域
			D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, GetTransform().size.width, GetTransform().size.height);
			// 设置画刷颜色和透明度
			render::D2D.SolidColorBrush->SetOpacity(GetDisplayOpacity());
			// 获取文本渲染器
			render::D2D.TextRenderer->SetTextStyle(
				style_.color,
				style_.outline,
				style_.outline_color,
				style_.outline_width,
				static_cast<D2D1_LINE_JOIN>(style_.outline_stroke)
			);
			text_layout_->Draw(nullptr, render::D2D.TextRenderer, 0, 0);
		}
	}

	void Text::Reset()
	{
		// 创建文字格式化
		CreateFormat();
		// 创建文字布局
		CreateLayout();
	}

	void Text::CreateFormat()
	{
		SafeRelease(text_format_);

		ThrowIfFailed(
			render::D2D.DWriteFactory->CreateTextFormat(
				font_.family.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT(font_.weight),
				font_.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				font_.size,
				L"",
				&text_format_
			)
		);

		// 设置文字对齐方式
		text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(style_.alignment));

		// 设置行间距
		if (style_.line_spacing == 0.f)
		{
			text_format_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			text_format_->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				style_.line_spacing,
				style_.line_spacing * 0.8f
			);
		}

		// 打开文本自动换行时，设置换行属性
		if (style_.wrap)
		{
			text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
		else
		{
			text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}

	void Text::CreateLayout()
	{
		SafeRelease(text_layout_);

		// 文本为空字符串时，重置属性
		if (text_.empty())
		{
			this->SetSize(0, 0);
			return;
		}

		if (text_format_ == nullptr)
		{
			E2D_WARNING("Text::CreateLayout failed! text_format_ NULL pointer exception.");
			return;
		}

		UINT32 length = static_cast<UINT32>(text_.size());

		// 对文本自动换行情况下进行处理
		if (style_.wrap)
		{
			ThrowIfFailed(
				render::D2D.DWriteFactory->CreateTextLayout(
					text_.c_str(),
					length,
					text_format_,
					style_.wrap_width,
					0,
					&text_layout_
				)
			);
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			text_layout_->GetMetrics(&metrics);
			// 重设文本宽高
			this->SetSize(metrics.layoutWidth, metrics.height);
		}
		else
		{
			// 为防止文本对齐问题，根据先创建 layout 以获取宽度
			ThrowIfFailed(
				render::D2D.DWriteFactory->CreateTextLayout(
					text_.c_str(),
					length,
					text_format_,
					0,
					0,
					&text_layout_
				)
			);

			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			text_layout_->GetMetrics(&metrics);
			// 重设文本宽高
			this->SetSize(metrics.width, metrics.height);

			// 重新创建 layout
			SafeRelease(text_layout_);
			ThrowIfFailed(
				render::D2D.DWriteFactory->CreateTextLayout(
					text_.c_str(),
					length,
					text_format_,
					GetTransform().size.width,
					0,
					&text_layout_
				)
			);
		}

		// 添加下划线和删除线
		DWRITE_TEXT_RANGE range = { 0, length };
		if (style_.underline)
		{
			text_layout_->SetUnderline(true, range);
		}
		if (style_.strikethrough)
		{
			text_layout_->SetStrikethrough(true, range);
		}
	}
}
