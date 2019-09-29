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
#include "Font.h"
#include "../2d/TextStyle.hpp"

namespace kiwano
{
	// 文本格式化
	class KGE_API TextFormat
	{
	public:
		TextFormat();

		TextFormat(Font const& font);

		void Update(Font const& font);

	public:
		inline ComPtr<IDWriteTextFormat> GetTextFormat() const		{ return text_format_; }

		inline void SetTextFormat(ComPtr<IDWriteTextFormat> format)	{ text_format_ = format; }

	protected:
		ComPtr<IDWriteTextFormat> text_format_;
	};


	// 文本布局
	class KGE_API TextLayout
	{
	public:
		TextLayout();

		TextLayout(String const& text, Font const& font, TextStyle const& style);

		void Update(Font const& font);

		void Update(String const& text, TextStyle const& style);

		std::uint32_t GetLineCount();

		Size GetLayoutSize() const;

		inline TextStyle const& GetTextStyle() const				{ return style_; }

	public:
		inline TextFormat GetTextFormat() const						{ return text_format_; }

		inline ComPtr<IDWriteTextLayout> GetTextLayout() const		{ return text_layout_; }

		inline void SetTextLayout(ComPtr<IDWriteTextLayout> layout)	{ text_layout_ = layout; }

		inline operator bool() const								{ return static_cast<bool>(text_layout_); }

	protected:
		TextStyle style_;
		TextFormat text_format_;
		ComPtr<IDWriteTextLayout> text_layout_;
	};
}
