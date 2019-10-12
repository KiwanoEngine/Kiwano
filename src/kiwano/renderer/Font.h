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
#include <kiwano/renderer/FontCollection.h>

namespace kiwano
{
	// 字体粗细值
	struct FontWeight
	{
		enum Value : uint32_t
		{
			Thin		= 100U,
			ExtraLight	= 200U,
			Light		= 300U,
			Normal		= 400U,
			Medium		= 500U,
			Bold		= 700U,
			ExtraBold	= 800U,
			Black		= 900U,
			ExtraBlack	= 950U
		};
	};

	// 字体
	class Font
	{
	public:
		String			family;		// 字体族
		float			size;		// 字号
		uint32_t		weight;		// 粗细值
		bool			italic;		// 是否斜体
		FontCollection	collection;	// 字体集

	public:
		Font(
			String const&	family	= L"",
			float			size	= 18,
			uint32_t		weight	= FontWeight::Normal,
			bool			italic	= false
		);

		Font(
			FontCollection	collection,
			String const&	family	= L"",
			float			size	= 18,
			uint32_t		weight	= FontWeight::Normal,
			bool			italic	= false
		);
	};
}
