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

#include <kiwano/renderer/TextLayout.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	TextLayout::TextLayout()
		: dirty_flag_(DirtyFlag::Clean)
	{
	}

	void TextLayout::Update()
	{
		if (!IsDirty())
			return;

		if (text_.empty())
		{
			text_format_.reset();
			text_layout_.reset();
			return;
		}

		if (!text_format_ || (dirty_flag_ & DirtyFlag::DirtyFormat))
		{
			Renderer::Instance().CreateTextFormat(*this);
		}

		if (dirty_flag_ & DirtyFlag::DirtyLayout)
		{
			Renderer::Instance().CreateTextLayout(*this);

			if (text_layout_)
			{
				SetAlignment(style_.alignment);
				SetWrapWidth(style_.wrap_width);
				SetLineSpacing(style_.line_spacing);
			}
		}

		dirty_flag_ = DirtyFlag::Updated;
	}

	void TextLayout::SetText(const String& text)
	{
		text_ = text;
		dirty_flag_ |= DirtyFlag::DirtyLayout;
	}

	void TextLayout::SetStyle(const TextStyle& style)
	{
		style_ = style;
		dirty_flag_ |= DirtyFlag::DirtyLayout;
	}

	void TextLayout::SetFont(FontPtr font)
	{
		if (style_.font != font)
		{
			style_.font = font;
			dirty_flag_ |= DirtyFlag::DirtyFormat;
		}
	}

	void TextLayout::SetFontFamily(String const& family)
	{
		if (style_.font_family != family)
		{
			style_.font_family = family;
			dirty_flag_ |= DirtyFlag::DirtyFormat;
		}
	}

	void TextLayout::SetFontSize(float size)
	{
		if (style_.font_size != size)
		{
			style_.font_size = size;
			dirty_flag_ |= DirtyFlag::DirtyFormat;
		}
	}

	void TextLayout::SetFontWeight(uint32_t weight)
	{
		if (style_.font_weight != weight)
		{
			style_.font_weight = weight;
			dirty_flag_ |= DirtyFlag::DirtyFormat;
		}
	}

	void TextLayout::SetItalic(bool italic)
	{
		if (style_.italic != italic)
		{
			style_.italic = italic;
			dirty_flag_ |= DirtyFlag::DirtyFormat;
		}
	}

	uint32_t TextLayout::GetLineCount() const
	{
		// Force to update layout
		const_cast<TextLayout*>(this)->Update();

		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(GetTextLayout()->GetMetrics(&metrics)))
			{
				return metrics.lineCount;
			}
		}
		return 0;
	}

	Size TextLayout::GetLayoutSize() const
	{
		// Force to update layout
		const_cast<TextLayout*>(this)->Update();

		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(GetTextLayout()->GetMetrics(&metrics)))
			{
				return (metrics.layoutWidth > 0) ? Size(metrics.layoutWidth, metrics.height) : Size(metrics.width, metrics.height);
			}
		}
		return Size();
	}

	void TextLayout::SetWrapWidth(float wrap_width)
	{
		style_.wrap_width = wrap_width;

		if (text_layout_)
		{
			HRESULT hr = S_OK;
			bool enable_wrapping = (wrap_width > 0);

			if (SUCCEEDED(hr))
			{
				hr = text_layout_->SetWordWrapping(enable_wrapping ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
			}

			if (SUCCEEDED(hr))
			{
				if (enable_wrapping)
				{
					hr = text_layout_->SetMaxWidth(wrap_width);
				}
				else
				{
					// Fix the layout width when the text does not wrap
					DWRITE_TEXT_METRICS metrics;
					hr = text_layout_->GetMetrics(&metrics);

					if (SUCCEEDED(hr))
					{
						hr = text_layout_->SetMaxWidth(metrics.width);
					}
				}
			}
			win32::ThrowIfFailed(hr);
		}
	}

	void TextLayout::SetLineSpacing(float line_spacing)
	{
		style_.line_spacing = line_spacing;

		if (text_layout_)
		{
			HRESULT hr = S_OK;
			if (line_spacing == 0.f)
			{
				hr = text_layout_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
			}
			else
			{
				hr = text_layout_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, line_spacing, line_spacing * 0.8f);
			}
			win32::ThrowIfFailed(hr);
		}
	}

	void TextLayout::SetAlignment(TextAlign align)
	{
		style_.alignment = align;

		if (text_layout_)
		{
			HRESULT hr = text_layout_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(align));
			win32::ThrowIfFailed(hr);
		}
	}

	void TextLayout::SetUnderline(bool enable, uint32_t start, uint32_t length)
	{
		// Force to update layout
		Update();

		HRESULT hr = text_layout_ ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = text_layout_->SetUnderline(enable, { start, length });
		}
		win32::ThrowIfFailed(hr);
	}

	void TextLayout::SetStrikethrough(bool enable, uint32_t start, uint32_t length)
	{
		// Force to update layout
		Update();

		HRESULT hr = text_layout_ ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = text_layout_->SetStrikethrough(enable, { start, length });
		}
		win32::ThrowIfFailed(hr);
	}

}
