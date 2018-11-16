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
#include "base.hpp"
#include "logs.h"

namespace easy2d
{
	namespace
	{
		Font text_default_font;
		TextStyle text_default_style;
	}

	void easy2d::Text::SetDefaultFont(Font const & font)
	{
		text_default_font = font;
	}

	void easy2d::Text::SetDefaultStyle(TextStyle const & style)
	{
		text_default_style = style;
	}

	Text::Text()
		: font_(text_default_font)
		, style_(text_default_style)
		, text_layout_(nullptr)
		, text_format_(nullptr)
		, dirty_layout_(false)
	{
	}

	Text::Text(const String & text)
		: Text(text, text_default_font, text_default_style)
	{
	}

	Text::Text(const String & text, const Font & font)
		: Text(text, font, text_default_style)
	{
	}

	Text::Text(const String & text, const TextStyle & style)
		: Text(text, text_default_font, style)
	{
	}

	Text::Text(const String & text, const Font & font, const TextStyle & style)
		: font_(font)
		, style_(style)
		, text_(text)
		, text_layout_(nullptr)
		, text_format_(nullptr)
		, dirty_layout_(true)
	{
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

	const TextStyle& Text::GetStyle() const
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

	unsigned int Text::GetFontWeight() const
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

	StrokeStyle Text::GetOutlineStroke() const
	{
		return style_.outline_stroke;
	}

	int Text::GetLineCount() const
	{
		UpdateLayout();
		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(text_layout_->GetMetrics(&metrics)))
			{
				return static_cast<int>(metrics.lineCount);
			}
		}
		return 0;
	}

	Rect Text::GetContentBounds() const
	{
		UpdateLayout();
		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(text_layout_->GetMetrics(&metrics)))
			{
				return Rect(0.f, 0.f, metrics.layoutWidth, metrics.height);
			}
		}
		return Rect{};
	}

	bool Text::IsItalic() const
	{
		return font_.italic;
	}

	bool Text::HasStrikethrough() const
	{
		return style_.strikethrough;
	}

	bool Text::HasUnderline() const
	{
		return style_.underline;
	}

	bool Text::HasOutline() const
	{
		return style_.outline;
	}

	void Text::SetText(const String& text)
	{
		text_ = text;
		dirty_layout_ = true;
	}

	void Text::SetStyle(const TextStyle& style)
	{
		style_ = style;
		dirty_layout_ = true;
	}

	void Text::SetFont(const Font & font)
	{
		font_ = font;
		dirty_layout_ = true;
	}

	void Text::SetFontFamily(const String& family)
	{
		if (font_.family != family)
		{
			font_.family = family;
			dirty_layout_ = true;
		}
	}

	void Text::SetFontSize(float size)
	{
		if (font_.size != size)
		{
			font_.size = size;
			dirty_layout_ = true;
		}
	}

	void Text::SetFontWeight(unsigned int weight)
	{
		if (font_.weight != weight)
		{
			font_.weight = weight;
			dirty_layout_ = true;
		}
	}

	void Text::SetColor(Color const& color)
	{
		style_.color = color;
	}

	void Text::SetItalic(bool val)
	{
		if (font_.italic != val)
		{
			font_.italic = val;
			dirty_layout_ = true;
		}
	}

	void Text::SetWrapEnabled(bool wrap)
	{
		if (style_.wrap != wrap)
		{
			style_.wrap = wrap;
			dirty_layout_ = true;
		}
	}

	void Text::SetWrapWidth(float wrap_width)
	{
		if (style_.wrap_width != wrap_width)
		{
			style_.wrap_width = std::max(wrap_width, 0.f);
			dirty_layout_ = true;
		}
	}

	void Text::SetLineSpacing(float line_spacing)
	{
		if (style_.line_spacing != line_spacing)
		{
			style_.line_spacing = line_spacing;
			dirty_layout_ = true;
		}
	}

	void Text::SetAlignment(TextAlign align)
	{
		if (style_.alignment != align)
		{
			style_.alignment = align;
			dirty_layout_ = true;
		}
	}

	void Text::SetUnderline(bool underline)
	{
		if (style_.underline != underline)
		{
			style_.underline = underline;
			dirty_layout_ = true;
		}
	}

	void Text::SetStrikethrough(bool strikethrough)
	{
		if (style_.strikethrough != strikethrough)
		{
			style_.strikethrough = strikethrough;
			dirty_layout_ = true;
		}
	}

	void Text::SetOutline(bool outline)
	{
		style_.outline = outline;
	}

	void Text::SetOutlineColor(Color const&outline_color)
	{
		style_.outline_color = outline_color;
	}

	void Text::SetOutlineWidth(float outline_width)
	{
		style_.outline_width = outline_width;
	}

	void Text::SetOutlineStroke(StrokeStyle outline_stroke)
	{
		style_.outline_stroke = outline_stroke;
	}

	void Text::OnDraw() const
	{
		UpdateLayout();

		if (text_layout_)
		{
			auto graphics = devices::Graphics::Instance();
			graphics->SetBrushOpacity(GetDisplayOpacity());
			graphics->SetTextStyle(
				style_.color,
				style_.outline,
				style_.outline_color,
				style_.outline_width,
				style_.outline_stroke
			);
			graphics->DrawTextLayout(text_layout_);
		}
	}

	void Text::UpdateLayout() const
	{
		if (!dirty_layout_)
			return;

		SafeRelease(text_format_);
		SafeRelease(text_layout_);

		if (text_.empty())
			return;

		auto graphics = devices::Graphics::Instance();

		ThrowIfFailed(
			graphics->CreateTextFormat(
				&text_format_,
				font_
			)
		);

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
		text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(style_.alignment));
		text_format_->SetWordWrapping(style_.wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);

		if (style_.wrap)
		{
			ThrowIfFailed(
				graphics->CreateTextLayout(
					&text_layout_,
					text_,
					text_format_,
					style_.wrap_width
				)
			);
		}
		else
		{
			ThrowIfFailed(
				graphics->CreateTextLayout(
					&text_layout_,
					text_,
					text_format_,
					0
				)
			);

			DWRITE_TEXT_METRICS metrics;
			text_layout_->GetMetrics(&metrics);

			SafeRelease(text_layout_);
			ThrowIfFailed(
				graphics->CreateTextLayout(
					&text_layout_,
					text_,
					text_format_,
					metrics.width
				)
			);
		}

		DWRITE_TEXT_RANGE range = { 0, static_cast<UINT32>(text_.length()) };
		if (style_.underline)
		{
			text_layout_->SetUnderline(true, range);
		}
		if (style_.strikethrough)
		{
			text_layout_->SetStrikethrough(true, range);
		}

		dirty_layout_ = false;
	}
}
