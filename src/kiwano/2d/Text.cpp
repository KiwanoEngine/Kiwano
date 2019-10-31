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

#include <kiwano/2d/Text.h>
#include <kiwano/base/Logger.h>
#include <kiwano/renderer/Renderer.h>

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
		, layout_dirty_(false)
		, format_dirty_(false)
	{
		text_layout_.SetTextStyle(text_default_style);
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
		, text_(text)
		, layout_dirty_(true)
		, format_dirty_(true)
	{
		text_layout_.SetTextStyle(style);
	}

	Text::~Text()
	{
	}

	void Text::SetText(String const& text)
	{
		text_ = text;
		layout_dirty_ = true;
	}

	void Text::SetStyle(const TextStyle& style)
	{
		text_layout_.SetTextStyle(style);
		layout_dirty_ = true;
	}

	void Text::SetFont(const Font & font)
	{
		font_ = font;
		format_dirty_ = true;
	}

	void Text::SetFontFamily(String const& family)
	{
		if (font_.family != family)
		{
			font_.family = family;
			format_dirty_ = true;
		}
	}

	void Text::SetFontSize(float size)
	{
		if (font_.size != size)
		{
			font_.size = size;
			format_dirty_ = true;
		}
	}

	void Text::SetFontWeight(uint32_t weight)
	{
		if (font_.weight != weight)
		{
			font_.weight = weight;
			format_dirty_ = true;
		}
	}

	void Text::SetItalic(bool italic)
	{
		if (font_.italic != italic)
		{
			font_.italic = italic;
			format_dirty_ = true;
		}
	}

	void Text::SetWrapWidth(float wrap_width)
	{
		if (text_layout_.GetTextStyle().wrap_width != wrap_width)
		{
			text_layout_.GetTextStyle().wrap_width = wrap_width;
			layout_dirty_ = true;
		}
	}

	void Text::SetLineSpacing(float line_spacing)
	{
		if (text_layout_.GetTextStyle().line_spacing != line_spacing)
		{
			text_layout_.GetTextStyle().line_spacing = line_spacing;
			layout_dirty_ = true;
		}
	}

	void Text::SetAlignment(TextAlign align)
	{
		if (text_layout_.GetTextStyle().alignment != align)
		{
			text_layout_.GetTextStyle().alignment = align;
			layout_dirty_ = true;
		}
	}

	void Text::SetUnderline(bool underline)
	{
		if (text_layout_.GetTextStyle().underline != underline)
		{
			text_layout_.GetTextStyle().underline = underline;
			layout_dirty_ = true;
		}
	}

	void Text::SetStrikethrough(bool strikethrough)
	{
		if (text_layout_.GetTextStyle().strikethrough != strikethrough)
		{
			text_layout_.GetTextStyle().strikethrough = strikethrough;
			layout_dirty_ = true;
		}
	}

	void Text::SetColor(Color const& color)
	{
		text_layout_.GetTextStyle().color = color;
		text_layout_.GetTextStyle().color = color;
	}

	void Text::SetOutline(bool outline)
	{
		text_layout_.GetTextStyle().outline = outline;
	}

	void Text::SetOutlineColor(Color const&outline_color)
	{
		text_layout_.GetTextStyle().outline_color = outline_color;
	}

	void Text::SetOutlineWidth(float outline_width)
	{
		text_layout_.GetTextStyle().outline_width = outline_width;
	}

	void Text::SetOutlineStroke(StrokeStyle outline_stroke)
	{
		text_layout_.GetTextStyle().outline_stroke = outline_stroke;
	}

	void Text::OnRender(RenderTarget* rt)
	{
		UpdateLayout();

		if (text_layout_ && CheckVisibilty(rt))
		{
			PrepareRender(rt);
			rt->DrawTextLayout(text_layout_);
		}
	}

	void Text::UpdateLayout()
	{
		if (format_dirty_)
		{
			format_dirty_ = false;
			text_layout_.UpdateFont(font_);
		}

		if (layout_dirty_)
		{
			layout_dirty_ = false;
			text_layout_.UpdateLayout(text_);
			SetSize(text_layout_.GetLayoutSize());
		}
	}
}
