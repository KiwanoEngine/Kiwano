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

#include "TextLayout.h"
#include "Renderer.h"
#include "../base/Logger.h"

namespace kiwano
{
	//
	// TextFormat
	//

	TextFormat::TextFormat()
	{
	}

	TextFormat::TextFormat(Font const& font)
	{
		Update(font);
	}

	void TextFormat::Update(Font const& font)
	{
		Renderer::GetInstance()->CreateTextFormat(*this, font);
	}


	//
	// TextLayout
	//

	TextLayout::TextLayout()
	{
	}

	TextLayout::TextLayout(String const& text, Font const& font, TextStyle const& style)
	{
		Update(font);
		Update(text, style);
	}

	void TextLayout::Update(Font const& font)
	{
		text_format_.Update(font);
	}

	void TextLayout::Update(String const& text, TextStyle const& style)
	{
		style_ = style;

		if (text.empty())
		{
			text_format_.SetTextFormat(nullptr);
			SetTextLayout(nullptr);
			return;
		}

		Renderer::GetInstance()->CreateTextLayout(
			*this,
			text,
			style,
			text_format_
		);
	}

	UINT32 TextLayout::GetLineCount()
	{
		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(text_layout_->GetMetrics(&metrics)))
			{
				return metrics.lineCount;
			}
		}
		return 0;
	}

	Size TextLayout::GetLayoutSize() const
	{
		if (text_layout_)
		{
			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(text_layout_->GetMetrics(&metrics)))
			{
				return (metrics.layoutWidth > 0) ? Size(metrics.layoutWidth, metrics.height) : Size(metrics.width, metrics.height);
			}
		}
		return Size();
	}

}
