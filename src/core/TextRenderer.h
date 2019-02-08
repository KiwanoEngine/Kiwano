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
#include "d2dhelper.hpp"

namespace easy2d
{
	interface ITextRenderer
		: public IDWriteTextRenderer
	{
	public:
		static E2D_API HRESULT Create(
			_Out_ ITextRenderer** ppTextRenderer,
			_In_ ID2D1Factory* pD2DFactory,
			_In_ ID2D1RenderTarget* pRT,
			_In_ ID2D1SolidColorBrush* pBrush
		);

		STDMETHOD_(void, SetTextStyle)(
			CONST D2D1_COLOR_F &fillColor,
			BOOL outline,
			CONST D2D1_COLOR_F &outlineColor,
			FLOAT outlineWidth,
			_In_ ID2D1StrokeStyle* outlineJoin
		) PURE;
	};
}

E2D_DECLARE_D2D_SMART_PTR(easy2d::ITextRenderer, D2DTextRendererPtr);
