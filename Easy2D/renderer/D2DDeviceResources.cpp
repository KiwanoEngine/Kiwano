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

#include "D2DDeviceResources.h"
#include "../2d/Image.h"
#include "../base/logs.h"
#include "../platform/modules.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

namespace easy2d
{

	D2DDeviceResources::D2DDeviceResources()
		: ref_count_(0)
		, dpi_(96.f)
	{
		CreateDeviceIndependentResources();
	}

	D2DDeviceResources::~D2DDeviceResources()
	{
		DiscardResources();
	}

	STDMETHODIMP_(unsigned long) D2DDeviceResources::AddRef()
	{
		return InterlockedIncrement(&ref_count_);
	}

	STDMETHODIMP_(unsigned long) D2DDeviceResources::Release()
	{
		unsigned long newCount = InterlockedDecrement(&ref_count_);

		if (newCount == 0)
		{
			delete this;
			return 0;
		}

		return newCount;
	}

	STDMETHODIMP D2DDeviceResources::QueryInterface(
		IID const& riid,
		void** object)
	{
		if (__uuidof(IUnknown) == riid)
		{
			*object = this;
		}
		else
		{
			*object = nullptr;
			return E_FAIL;
		}

		AddRef();

		return S_OK;
	}

	void D2DDeviceResources::DiscardResources()
	{
		ClearImageCache();

		d2d_factory_.Reset();
		d2d_device_.Reset();
		d2d_device_context_.Reset();
		d2d_target_bitmap_.Reset();

		imaging_factory_.Reset();
		dwrite_factory_.Reset();

		d2d_miter_stroke_style_.Reset();
		d2d_bevel_stroke_style_.Reset();
		d2d_round_stroke_style_.Reset();
	}

	HRESULT D2DDeviceResources::CreateDeviceIndependentResources()
	{
		HRESULT hr = S_OK;

		ComPtr<ID2D1Factory1>			d2d_factory;
		ComPtr<IWICImagingFactory>		imaging_factory;
		ComPtr<IDWriteFactory>			dwrite_factory;

		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
#ifdef E2D_DEBUG
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&options,
			reinterpret_cast<void**>(&d2d_factory)
		);

		if (SUCCEEDED(hr))
		{
			d2d_factory_ = d2d_factory;

			hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				__uuidof(IWICImagingFactory),
				reinterpret_cast<void**>(&imaging_factory)
			);
		}

		if (SUCCEEDED(hr))
		{
			imaging_factory_ = imaging_factory;

			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&dwrite_factory)
			);
		}

		if (SUCCEEDED(hr))
		{
			dwrite_factory_ = dwrite_factory;

			ComPtr<ID2D1StrokeStyle>		d2d_miter_stroke_style;
			ComPtr<ID2D1StrokeStyle>		d2d_bevel_stroke_style;
			ComPtr<ID2D1StrokeStyle>		d2d_round_stroke_style;

			D2D1_STROKE_STYLE_PROPERTIES stroke_style = D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f
			);

			hr = d2d_factory_->CreateStrokeStyle(
				stroke_style,
				nullptr,
				0,
				&d2d_miter_stroke_style
			);

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;
				hr = d2d_factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d_bevel_stroke_style
				);
			}

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;
				hr = d2d_factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d_round_stroke_style
				);
			}

			if (SUCCEEDED(hr))
			{
				d2d_miter_stroke_style_ = d2d_miter_stroke_style;
				d2d_bevel_stroke_style_ = d2d_bevel_stroke_style;
				d2d_round_stroke_style_ = d2d_round_stroke_style;
			}
		}

		return hr;
	}

	HRESULT D2DDeviceResources::SetD2DDevice(ComPtr<ID2D1Device> const& device)
	{
		ComPtr<ID2D1DeviceContext> d2d_device_ctx;

		HRESULT hr = device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&d2d_device_ctx
		);

		if (SUCCEEDED(hr))
		{
			d2d_device_ = device;
			d2d_device_context_ = d2d_device_ctx;
			d2d_device_context_->SetDpi(dpi_, dpi_);
		}

		return hr;
	}

	void D2DDeviceResources::SetTargetBitmap(ComPtr<ID2D1Bitmap1> const& target)
	{
		d2d_target_bitmap_ = target;
		if (d2d_device_context_)
			d2d_device_context_->SetTarget(d2d_target_bitmap_.Get());
	}

	HRESULT D2DDeviceResources::CreateBitmapFromFile(ComPtr<ID2D1Bitmap> & bitmap, String const & file_path)
	{
		if (!imaging_factory_ || !d2d_device_context_)
			return E_UNEXPECTED;

		size_t hash_code = std::hash<String>{}(file_path);
		if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
		{
			bitmap = bitmap_cache_[hash_code];
			return S_OK;
		}

		ComPtr<IWICBitmapDecoder>		decoder;
		ComPtr<IWICBitmapFrameDecode>	source;
		ComPtr<IWICStream>				stream;
		ComPtr<IWICFormatConverter>		converter;
		ComPtr<ID2D1Bitmap>				bitmap_tmp;

		HRESULT hr = imaging_factory_->CreateDecoderFromFilename(
			file_path.c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&decoder
		);

		if (SUCCEEDED(hr))
		{
			hr = decoder->GetFrame(0, &source);
		}

		if (SUCCEEDED(hr))
		{
			hr = imaging_factory_->CreateFormatConverter(&converter);
		}

		if (SUCCEEDED(hr))
		{
			// ͼƬ��ʽת���� 32bppPBGRA
			hr = converter->Initialize(
				source.Get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = d2d_device_context_->CreateBitmapFromWicBitmap(
				converter.Get(),
				nullptr,
				&bitmap_tmp
			);
		}

		if (SUCCEEDED(hr))
		{
			bitmap = bitmap_tmp;
			bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
		}

		return hr;
	}

	HRESULT D2DDeviceResources::CreateBitmapFromResource(ComPtr<ID2D1Bitmap> & bitmap, Resource const & res)
	{
		if (!imaging_factory_ || !d2d_device_context_)
			return E_UNEXPECTED;

		size_t hash_code = res.GetHashCode();
		if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
		{
			bitmap = bitmap_cache_[hash_code];
			return S_OK;
		}

		ComPtr<IWICBitmapDecoder>		decoder;
		ComPtr<IWICBitmapFrameDecode>	source;
		ComPtr<IWICStream>				stream;
		ComPtr<IWICFormatConverter>		converter;
		ComPtr<ID2D1Bitmap>				bitmap_tmp;

		// ������Դ
		LPVOID buffer;
		DWORD buffer_size;
		HRESULT hr = res.Load(buffer, buffer_size) ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = imaging_factory_->CreateStream(&stream);
		}

		if (SUCCEEDED(hr))
		{
			hr = stream->InitializeFromMemory(
				static_cast<WICInProcPointer>(buffer),
				buffer_size
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = imaging_factory_->CreateDecoderFromStream(
				stream.Get(),
				nullptr,
				WICDecodeMetadataCacheOnLoad,
				&decoder
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = decoder->GetFrame(0, &source);
		}

		if (SUCCEEDED(hr))
		{
			hr = imaging_factory_->CreateFormatConverter(&converter);
		}

		if (SUCCEEDED(hr))
		{
			// ͼƬ��ʽת���� 32bppPBGRA
			hr = converter->Initialize(
				source.Get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = d2d_device_context_->CreateBitmapFromWicBitmap(
				converter.Get(),
				nullptr,
				&bitmap_tmp
			);
		}

		if (SUCCEEDED(hr))
		{
			bitmap = bitmap_tmp;
			bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
		}

		return hr;
	}

	HRESULT D2DDeviceResources::CreateTextFormat(ComPtr<IDWriteTextFormat> & text_format, Font const & font, TextStyle const & text_style) const
	{
		if (!dwrite_factory_)
			return E_UNEXPECTED;

		ComPtr<IDWriteTextFormat> text_format_tmp;
		HRESULT hr = dwrite_factory_->CreateTextFormat(
			font.family.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT(font.weight),
			font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			font.size,
			L"",
			&text_format_tmp
		);

		if (SUCCEEDED(hr))
		{
			if (text_style.line_spacing == 0.f)
			{
				text_format_tmp->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
			}
			else
			{
				text_format_tmp->SetLineSpacing(
					DWRITE_LINE_SPACING_METHOD_UNIFORM,
					text_style.line_spacing,
					text_style.line_spacing * 0.8f
				);
			}
			text_format_tmp->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(text_style.alignment));
			text_format_tmp->SetWordWrapping(text_style.wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
			text_format = text_format_tmp;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateTextLayout(ComPtr<IDWriteTextLayout> & text_layout, Size& layout_size, String const & text, ComPtr<IDWriteTextFormat> const& text_format, TextStyle const & text_style) const
	{
		if (!dwrite_factory_)
			return E_UNEXPECTED;

		text_layout = nullptr;

		HRESULT hr;
		ComPtr<IDWriteTextLayout> text_layout_tmp;
		UINT32 length = static_cast<UINT32>(text.length());

		if (text_style.wrap)
		{
			hr = dwrite_factory_->CreateTextLayout(
				text.c_str(),
				length,
				text_format.Get(),
				text_style.wrap_width,
				0,
				&text_layout_tmp
			);
		}
		else
		{
			hr = dwrite_factory_->CreateTextLayout(
				text.c_str(),
				length,
				text_format.Get(),
				0,
				0,
				&text_layout_tmp
			);

			DWRITE_TEXT_METRICS metrics;
			if (SUCCEEDED(hr))
			{
				hr = text_layout_tmp->GetMetrics(&metrics);
			}

			if (SUCCEEDED(hr))
			{
				text_layout_tmp = nullptr;
				hr = dwrite_factory_->CreateTextLayout(
					text.c_str(),
					length,
					text_format.Get(),
					metrics.width,
					0,
					&text_layout_tmp
				);
			}
		}

		if (SUCCEEDED(hr))
		{
			DWRITE_TEXT_METRICS metrics;
			text_layout_tmp->GetMetrics(&metrics);

			if (text_style.wrap)
			{
				layout_size = Size(metrics.layoutWidth, metrics.height);
			}
			else
			{
				layout_size = Size(metrics.width, metrics.height);
			}

			DWRITE_TEXT_RANGE range = { 0, length };
			if (text_style.underline)
			{
				text_layout_tmp->SetUnderline(true, range);
			}
			if (text_style.strikethrough)
			{
				text_layout_tmp->SetStrikethrough(true, range);
			}
			text_layout = text_layout_tmp;
		}
		return hr;
	}

	void D2DDeviceResources::ClearImageCache()
	{
		bitmap_cache_.clear();
	}

	ID2D1StrokeStyle* D2DDeviceResources::GetStrokeStyle(StrokeStyle stroke) const
	{
		switch (stroke)
		{
		case StrokeStyle::Miter: return d2d_miter_stroke_style_.Get(); break;
		case StrokeStyle::Bevel: return d2d_bevel_stroke_style_.Get(); break;
		case StrokeStyle::Round: return d2d_round_stroke_style_.Get(); break;
		}
		return nullptr;
	}

}