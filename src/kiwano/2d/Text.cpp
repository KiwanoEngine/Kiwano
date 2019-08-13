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

#include "Text.h"
#include "../base/logs.h"
#include "../renderer/render.h"

namespace kiwano
{
	namespace
	{
		Font text_default_font;
		TextStyle text_default_style;
	}

	void kiwano::Text::SetDefaultFont(Font const & font)
	{
		text_default_font = font;
	}

	void kiwano::Text::SetDefaultStyle(TextStyle const & style)
	{
		text_default_style = style;
	}

	Text::Text()
		: font_(text_default_font)
		, style_(text_default_style)
		, layout_dirty_(false)
	{
	}

	Text::Text(String const& text)
		: Text(text, text_default_font, text_default_style)
	{
	}

	Text::Text(String const& text, const Font & font)
		: Text(text, font, text_default_style)
	{
	}

	Text::Text(String const& text, const TextStyle & style)
		: Text(text, text_default_font, style)
	{
	}

	Text::Text(String const& text, const Font & font, const TextStyle & style)
		: font_(font)
		, style_(style)
		, text_(text)
		, layout_dirty_(true)
	{
	}

	Text::~Text()
	{
	}

	String const& Text::GetText() const
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

	String const& Text::GetFontFamily() const
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

	int Text::GetLineCount()
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

	Size const& Text::GetLayoutSize() const
	{
		UpdateLayout();
		return layout_size_;
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

	void Text::SetText(String const& text)
	{
		text_ = text;
		layout_dirty_ = true;
	}

	void Text::SetStyle(const TextStyle& style)
	{
		style_ = style;
		layout_dirty_ = true;
	}

	void Text::SetFont(const Font & font)
	{
		font_ = font;
		layout_dirty_ = true;
	}

	void Text::SetFontFamily(String const& family)
	{
		if (font_.family != family)
		{
			font_.family = family;
			layout_dirty_ = true;
		}
	}

	void Text::SetFontSize(float size)
	{
		if (font_.size != size)
		{
			font_.size = size;
			layout_dirty_ = true;
		}
	}

	void Text::SetFontWeight(unsigned int weight)
	{
		if (font_.weight != weight)
		{
			font_.weight = weight;
			layout_dirty_ = true;
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
			layout_dirty_ = true;
		}
	}

	void Text::SetWrapEnabled(bool wrap)
	{
		if (style_.wrap != wrap)
		{
			style_.wrap = wrap;
			layout_dirty_ = true;
		}
	}

	void Text::SetWrapWidth(float wrap_width)
	{
		if (style_.wrap_width != wrap_width)
		{
			style_.wrap_width = std::max(wrap_width, 0.f);
			layout_dirty_ = true;
		}
	}

	void Text::SetLineacingPtr(float line_spacing)
	{
		if (style_.line_spacing != line_spacing)
		{
			style_.line_spacing = line_spacing;
			layout_dirty_ = true;
		}
	}

	void Text::SetAlignment(TextAlign align)
	{
		if (style_.alignment != align)
		{
			style_.alignment = align;
			layout_dirty_ = true;
		}
	}

	void Text::SetUnderline(bool underline)
	{
		if (style_.underline != underline)
		{
			style_.underline = underline;
			layout_dirty_ = true;
		}
	}

	void Text::SetStrikethrough(bool strikethrough)
	{
		if (style_.strikethrough != strikethrough)
		{
			style_.strikethrough = strikethrough;
			layout_dirty_ = true;
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

	void Text::OnRender(Renderer* renderer)
	{
		UpdateLayout();

		if (text_layout_ && renderer->CheckVisibility(layout_size_, transform_matrix_))
		{
			PrepareRender(renderer);
			renderer->SetTextStyle(
				GetDisplayedOpacity(),
				style_.color,
				style_.outline,
				style_.outline_color,
				style_.outline_width,
				style_.outline_stroke
			);
            renderer->DrawTextLayout(text_layout_);
		}
	}

	void Text::UpdateLayout() const
	{
		if (!layout_dirty_)
			return;

		layout_dirty_ = false;
		text_format_ = nullptr;
		text_layout_ = nullptr;

		if (text_.empty())
			return;

        auto renderer = Renderer::GetInstance();

		ThrowIfFailed(
            renderer->GetD2DDeviceResources()->CreateTextFormat(
				text_format_,
				font_,
				style_
			)
		);

		ThrowIfFailed(
            renderer->GetD2DDeviceResources()->CreateTextLayout(
				text_layout_,
				layout_size_,
				text_,
				text_format_,
				style_
			)
		);
	}
}
