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
#include "helper.hpp"
#include "../base/Resource.h"
#include "../2d/Font.hpp"
#include "../2d/TextStyle.hpp"
#include <dwrite.h>
#include <d2d1.h>
#include <d2d1_1.h>

namespace kiwano
{
	class KGE_API D2DDeviceResources
		: public IUnknown
	{
	public:
		HRESULT CreateBitmapFromFile(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_ String const& file_path
		);

		HRESULT CreateBitmapFromResource(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_ Resource const& res
		);

		HRESULT CreateTextFormat(
			_Out_ ComPtr<IDWriteTextFormat>& text_format,
			_In_ Font const& font,
			_In_ TextStyle const& text_style
		) const;

		HRESULT CreateTextLayout(
			_Out_ ComPtr<IDWriteTextLayout>& text_layout,
			_Out_ Size& layout_size,
			_In_ String const& text,
			_In_ ComPtr<IDWriteTextFormat> const& text_format,
			_In_ TextStyle const& text_style
		) const;

		void ClearImageCache();

		void DiscardResources();

		HRESULT SetD2DDevice(
			_In_ ComPtr<ID2D1Device> const& device
		);

		void SetTargetBitmap(
			_In_ ComPtr<ID2D1Bitmap1> const& target
		);

		inline ID2D1Factory1*			GetD2DFactory() const			{ KGE_ASSERT(d2d_factory_); return d2d_factory_.Get(); }
		inline IWICImagingFactory*		GetWICImagingFactory() const	{ KGE_ASSERT(imaging_factory_); return imaging_factory_.Get(); }
		inline IDWriteFactory*			GetDWriteFactory() const		{ KGE_ASSERT(dwrite_factory_); return dwrite_factory_.Get(); }
		inline ID2D1Device*				GetD2DDevice() const			{ KGE_ASSERT(d2d_device_); return d2d_device_.Get(); }
		inline ID2D1DeviceContext*		GetD2DDeviceContext() const		{ KGE_ASSERT(d2d_device_context_); return d2d_device_context_.Get(); }
		inline ID2D1Bitmap1*			GetD2DTargetBitmap() const		{ KGE_ASSERT(d2d_target_bitmap_); return d2d_target_bitmap_.Get(); }

		ID2D1StrokeStyle*				GetStrokeStyle(StrokeStyle stroke) const;

	public:
		unsigned long STDMETHODCALLTYPE AddRef();

		unsigned long STDMETHODCALLTYPE Release();

		HRESULT STDMETHODCALLTYPE QueryInterface(
			IID const& riid,
			void** ppvObject
		);

	protected:
		D2DDeviceResources();

		virtual ~D2DDeviceResources();

		HRESULT CreateDeviceIndependentResources();

	private:
		unsigned long ref_count_;
		float dpi_;

		using BitmapMap = UnorderedMap<size_t, ComPtr<ID2D1Bitmap>>;
		BitmapMap bitmap_cache_;

		ComPtr<ID2D1Factory1>		d2d_factory_;
		ComPtr<ID2D1Device>			d2d_device_;
		ComPtr<ID2D1DeviceContext>	d2d_device_context_;
		ComPtr<ID2D1Bitmap1>		d2d_target_bitmap_;

		ComPtr<IWICImagingFactory>	imaging_factory_;
		ComPtr<IDWriteFactory>		dwrite_factory_;

		ComPtr<ID2D1StrokeStyle>	d2d_miter_stroke_style_;
		ComPtr<ID2D1StrokeStyle>	d2d_bevel_stroke_style_;
		ComPtr<ID2D1StrokeStyle>	d2d_round_stroke_style_;
	};

}
