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
#include "IntrusivePtr.hpp"
#include "../math/vector.hpp"
#include "../math/Rect.hpp"
#include "Color.h"
#include <d2d1.h>
#include <dwrite.h>

#ifndef E2D_DECLARE_D2D_SMART_PTR
#define E2D_DECLARE_D2D_SMART_PTR(class_name, sp_name)\
	using sp_name = ::easy2d::IntrusivePtr< class_name >
#endif


namespace easy2d
{
	// "D2DPtr" is a shorthand for "COM Pointer"

	E2D_DECLARE_D2D_SMART_PTR(ID2D1Factory, D2DFactoryPtr);
	E2D_DECLARE_D2D_SMART_PTR(IWICImagingFactory, D2DImagingFactoryPtr);
	E2D_DECLARE_D2D_SMART_PTR(IDWriteFactory, D2DWriteFactoryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1SolidColorBrush, D2DSolidColorBrushPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1RenderTarget, D2DRenderTargetPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1HwndRenderTarget, D2DHwndRenderTargetPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1BitmapRenderTarget, D2DBitmapRenderTargetPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1StrokeStyle, D2DStrokeStylePtr);

	E2D_DECLARE_D2D_SMART_PTR(ID2D1Geometry, D2DGeometryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1RectangleGeometry, D2DRectangleGeometryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1RoundedRectangleGeometry, D2DRoundedRectangleGeometryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1EllipseGeometry, D2DEllipseGeometryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1GeometryGroup, D2DGeometryGroupPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1PathGeometry, D2DPathGeometryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1TransformedGeometry, D2DTransformedGeometryPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1GeometrySink, D2DGeometrySinkPtr);

	E2D_DECLARE_D2D_SMART_PTR(ID2D1Layer, D2DLayerPtr);
	E2D_DECLARE_D2D_SMART_PTR(ID2D1Bitmap, D2DBitmapPtr);
	E2D_DECLARE_D2D_SMART_PTR(IDWriteTextFormat, D2DTextFormatPtr);
	E2D_DECLARE_D2D_SMART_PTR(IDWriteTextLayout, D2DTextLayoutPtr);

	inline void IntrusivePtrAddRef(IUnknown* ptr)
	{
		if (ptr) { ptr->AddRef(); }
	}

	inline void IntrusivePtrRelease(IUnknown* ptr)
	{
		if (ptr) { ptr->Release(); }
	}

	template <typename T>
	inline void SafeRelease(T*& ptr)
	{
		if (ptr != nullptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	inline D2D1_POINT_2F const& ToD2dPoint2F(math::Vec2 const& point)
	{
		return reinterpret_cast<D2D1_POINT_2F const&>(point);
	}

	inline D2D1_SIZE_F const& ToD2dSizeF(math::Vec2 const& size)
	{
		return reinterpret_cast<D2D1_SIZE_F const&>(size);
	}

	inline D2D1_RECT_F ToD2dRectF(math::Rect const& rect)
	{
		return D2D1_RECT_F{ rect.origin.x, rect.origin.y, rect.origin.x + rect.size.x, rect.origin.y + rect.size.y };
	}

	inline D2D1_COLOR_F const& ToD2dColorF(Color const& color)
	{
		return reinterpret_cast<D2D1_COLOR_F const&>(color);
	}
}
