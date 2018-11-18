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
#include "macros.h"
#include "intrusive/SmartPointer.hpp"
#include <d2d1.h>
#include <dwrite.h>

#ifndef E2D_DECLARE_D2D_SMART_PTR
#define E2D_DECLARE_D2D_SMART_PTR(class_name, sp_name)\
	using sp_name = ::easy2d::intrusive::SmartPointer< class_name >
#endif


namespace easy2d
{
	E2D_DECLARE_D2D_SMART_PTR(ID2D1Factory, cpFactory);
	E2D_DECLARE_D2D_SMART_PTR(IWICImagingFactory, cpImagingFactory);
	E2D_DECLARE_D2D_SMART_PTR(IDWriteFactory, cpWriteFactory);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1SolidColorBrush, cpSolidColorBrush);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1RenderTarget, cpRenderTarget);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1HwndRenderTarget, cpHwndRenderTarget);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1BitmapRenderTarget, cpBitmapRenderTarget);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1StrokeStyle, cpStrokeStyle);

	E2D_DECLARE_D2D_SMART_PTR(ID2D1Layer, cpLayer);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1Bitmap, cpBitmap);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1Geometry, cpGeometry);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1PathGeometry, cpPathGeometry);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1GeometrySink, cpGeometrySink);
	E2D_DECLARE_D2D_SMART_PTR(IDWriteTextFormat, cpTextFormat);
	E2D_DECLARE_D2D_SMART_PTR(IDWriteTextLayout, cpTextLayout);

	inline void IntrusivePtrAddRef(IUnknown* ptr)
	{
		if (ptr) { ptr->AddRef(); }
	}

	inline void IntrusivePtrRelease(IUnknown* ptr)
	{
		if (ptr) { ptr->Release(); }
	}

	inline void SafeRelease(IUnknown* ptr)
	{
		if (ptr != nullptr)
		{
			ptr->Release();
		}
	}
}
