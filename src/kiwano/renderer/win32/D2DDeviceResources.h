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
#include "../Font.h"
#include "../Color.h"
#include "../../math/math.h"
#include "../../base/Resource.h"
#include "../../2d/TextStyle.hpp"
#include <dwrite.h>
#include <d2d1.h>
#include <d2d1_1.h>

namespace kiwano
{
	namespace DX
	{
		template <typename T>
		inline void SafeRelease(T*& ptr)
		{
			if (ptr != nullptr)
			{
				ptr->Release();
				ptr = nullptr;
			}
		}

		template <typename T>
		inline T* SafeAcquire(T* ptr)
		{
			if (ptr != nullptr)
			{
				ptr->AddRef();
			}
			return ptr;
		}

		//
		// Point2F
		//

		inline D2D1_POINT_2F const& ConvertToPoint2F(Vec2 const& vec2)
		{
			return reinterpret_cast<D2D1_POINT_2F const&>(vec2);
		}

		inline D2D1_POINT_2F& ConvertToPoint2F(Vec2& vec2)
		{
			return reinterpret_cast<D2D1_POINT_2F&>(vec2);
		}

		inline const D2D1_POINT_2F* ConvertToPoint2F(const Vec2* vec2)
		{
			return reinterpret_cast<const D2D1_POINT_2F*>(vec2);
		}

		inline D2D1_POINT_2F* ConvertToPoint2F(Vec2* vec2)
		{
			return reinterpret_cast<D2D1_POINT_2F*>(vec2);
		}

		//
		// SizeF
		//

		inline D2D1_SIZE_F const& ConvertToSizeF(Vec2 const& vec2)
		{
			return reinterpret_cast<D2D1_SIZE_F const&>(vec2);
		}

		inline D2D1_SIZE_F& ConvertToSizeF(Vec2& vec2)
		{
			return reinterpret_cast<D2D1_SIZE_F&>(vec2);
		}

		inline const D2D1_SIZE_F* ConvertToSizeF(const Vec2* vec2)
		{
			return reinterpret_cast<const D2D1_SIZE_F*>(vec2);
		}

		inline D2D1_SIZE_F* ConvertToSizeF(Vec2* vec2)
		{
			return reinterpret_cast<D2D1_SIZE_F*>(vec2);
		}

		//
		// RectF
		//

		inline D2D1_RECT_F const& ConvertToRectF(Rect const& rect)
		{
			return reinterpret_cast<D2D1_RECT_F const&>(rect);
		}

		inline D2D1_RECT_F& ConvertToRectF(Rect& rect)
		{
			return reinterpret_cast<D2D1_RECT_F&>(rect);
		}

		inline const D2D1_RECT_F* ConvertToRectF(const Rect* rect)
		{
			return reinterpret_cast<const D2D1_RECT_F*>(rect);
		}

		inline D2D1_RECT_F* ConvertToRectF(Rect* rect)
		{
			return reinterpret_cast<D2D1_RECT_F*>(rect);
		}

		//
		// ColorF
		//
		inline D2D1_COLOR_F const& ConvertToColorF(Color const& color)
		{
			return reinterpret_cast<D2D1_COLOR_F const&>(color);
		}

		inline D2D1_COLOR_F& ConvertToColorF(Color& color)
		{
			return reinterpret_cast<D2D1_COLOR_F&>(color);
		}

		inline const D2D1_COLOR_F* ConvertToColorF(const Color* color)
		{
			return reinterpret_cast<const D2D1_COLOR_F*>(color);
		}

		inline D2D1_COLOR_F* ConvertToColorF(Color* color)
		{
			return reinterpret_cast<D2D1_COLOR_F*>(color);
		}

		//
		// MatrixF
		//

		inline D2D1_MATRIX_3X2_F const& ConvertToMatrix3x2F(Matrix3x2 const& matrix)
		{
			return reinterpret_cast<D2D1_MATRIX_3X2_F const&>(matrix);
		}

		inline D2D1_MATRIX_3X2_F& ConvertToMatrix3x2F(Matrix3x2& matrix)
		{
			return reinterpret_cast<D2D1_MATRIX_3X2_F&>(matrix);
		}

		inline const D2D1_MATRIX_3X2_F* ConvertToMatrix3x2F(const Matrix3x2* matrix)
		{
			return reinterpret_cast<const D2D1_MATRIX_3X2_F*>(matrix);
		}

		inline D2D1_MATRIX_3X2_F* ConvertToMatrix3x2F(Matrix3x2* matrix)
		{
			return reinterpret_cast<D2D1_MATRIX_3X2_F*>(matrix);
		}

		// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
		inline Float32 ConvertDipsToPixels(Float32 dips, Float32 dpi)
		{
			static const Float32 dips_per_inch = 96.0f;
			return math::Floor(dips * dpi / dips_per_inch + 0.5f); // Round to nearest integer.
		}
	}
}

namespace kiwano
{
	MIDL_INTERFACE("5706684a-bf6d-4b03-b627-094758a33032")
	KGE_API ID2DDeviceResources
		: public IUnknown
	{
	public:
		static HRESULT Create(ID2DDeviceResources** device_resources);

		virtual HRESULT CreateBitmapFromFile(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_ String const& file_path
		) = 0;

		virtual HRESULT CreateBitmapFromResource(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_ Resource const& res
		) = 0;

		virtual HRESULT CreateTextFormat(
			_Out_ ComPtr<IDWriteTextFormat>& text_format,
			_In_ Font const& font
		) const = 0;

		virtual HRESULT CreateTextLayout(
			_Out_ ComPtr<IDWriteTextLayout>& text_layout,
			_In_ String const& text,
			_In_ TextStyle const& text_style,
			_In_ ComPtr<IDWriteTextFormat> const& text_format
		) const = 0;

		virtual ID2D1StrokeStyle* GetStrokeStyle(StrokeStyle stroke) const = 0;

		virtual HRESULT SetD2DDevice(
			_In_ ComPtr<ID2D1Device> const& device
		) = 0;

		virtual void SetTargetBitmap(
			_In_ ComPtr<ID2D1Bitmap1> const& target
		) = 0;

		virtual void DiscardResources() = 0;

		inline ID2D1Factory1*			GetFactory() const				{ KGE_ASSERT(factory_); return factory_.get(); }
		inline IWICImagingFactory*		GetWICImagingFactory() const	{ KGE_ASSERT(imaging_factory_); return imaging_factory_.get(); }
		inline IDWriteFactory*			GetDWriteFactory() const		{ KGE_ASSERT(dwrite_factory_); return dwrite_factory_.get(); }
		inline ID2D1Device*				GetDevice() const				{ KGE_ASSERT(device_); return device_.get(); }
		inline ID2D1DeviceContext*		GetDeviceContext() const		{ KGE_ASSERT(device_context_); return device_context_.get(); }
		inline ID2D1Bitmap1*			GetTargetBitmap() const			{ KGE_ASSERT(target_bitmap_); return target_bitmap_.get(); }

	protected:
		ComPtr<ID2D1Factory1>		factory_;
		ComPtr<ID2D1Device>			device_;
		ComPtr<ID2D1DeviceContext>	device_context_;
		ComPtr<ID2D1Bitmap1>		target_bitmap_;

		ComPtr<IWICImagingFactory>	imaging_factory_;
		ComPtr<IDWriteFactory>		dwrite_factory_;
	};

}
