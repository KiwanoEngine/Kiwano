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

#include "D2DDeviceResources.h"
#include "../../base/Logger.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

namespace kiwano
{
	struct D2DDeviceResources
		: public ID2DDeviceResources
	{
	public:
		D2DDeviceResources();

		virtual ~D2DDeviceResources();

		HRESULT CreateDeviceIndependentResources();

	public:
		HRESULT CreateBitmapConverter(
			_Out_ ComPtr<IWICFormatConverter>& converter,
			_In_opt_ ComPtr<IWICBitmapSource> source,
			_In_ REFWICPixelFormatGUID format,
			WICBitmapDitherType dither,
			_In_opt_ ComPtr<IWICPalette> palette,
			double alpha_threshold_percent,
			WICBitmapPaletteType palette_translate
		) override;

		HRESULT CreateBitmapFromConverter(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_opt_ const D2D1_BITMAP_PROPERTIES* properties,
			_In_ ComPtr<IWICFormatConverter> converter
		) override;

		HRESULT CreateBitmapDecoderFromFile(
			_Out_ ComPtr<IWICBitmapDecoder>& decoder,
			const String& file_path
		) override;

		HRESULT CreateBitmapDecoderFromResource(
			_Out_ ComPtr<IWICBitmapDecoder>& decoder,
			const Resource& resource
		) override;

		HRESULT CreateTextFormat(
			_Out_ ComPtr<IDWriteTextFormat>& text_format,
			_In_ Font const& font
		) const override;

		HRESULT CreateTextLayout(
			_Out_ ComPtr<IDWriteTextLayout>& text_layout,
			_In_ String const& text,
			_In_ ComPtr<IDWriteTextFormat> const& text_format
		) const override;

		HRESULT SetD2DDevice(
			_In_ ComPtr<ID2D1Device> const& device
		) override;

		void SetTargetBitmap(
			_In_ ComPtr<ID2D1Bitmap1> const& target
		) override;

		void DiscardResources() override;

	public:
		unsigned long STDMETHODCALLTYPE AddRef();

		unsigned long STDMETHODCALLTYPE Release();

		HRESULT STDMETHODCALLTYPE QueryInterface(
			IID const& riid,
			void** ppvObject
		);

	protected:
		unsigned long ref_count_;
		Float32 dpi_;
	};


	HRESULT ID2DDeviceResources::Create(ID2DDeviceResources** device_resources)
	{
		HRESULT hr = E_FAIL;
		if (device_resources)
		{
			D2DDeviceResources* res = new (std::nothrow) D2DDeviceResources;
			if (res)
			{
				hr = res->CreateDeviceIndependentResources();
			}

			if (SUCCEEDED(hr))
			{
				res->AddRef();

				if (*device_resources)
				{
					(*device_resources)->Release();
				}
				(*device_resources) = res;
			}
			else
			{
				delete res;
				res = nullptr;
			}
		}
		return hr;
	}

	D2DDeviceResources::D2DDeviceResources()
		: ref_count_(0)
		, dpi_(96.f)
	{
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

	STDMETHODIMP D2DDeviceResources::QueryInterface(IID const& riid, void** object)
	{
		if (__uuidof(ID2DDeviceResources) == riid)
		{
			*object = this;
		}
		else if (__uuidof(IUnknown) == riid)
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
		factory_.reset();
		device_.reset();
		device_context_.reset();
		target_bitmap_.reset();

		imaging_factory_.reset();
		dwrite_factory_.reset();

		miter_stroke_style_.reset();
		bevel_stroke_style_.reset();
		round_stroke_style_.reset();
	}

	HRESULT D2DDeviceResources::CreateDeviceIndependentResources()
	{
		HRESULT hr = S_OK;

		ComPtr<ID2D1Factory1>		factory;
		ComPtr<IWICImagingFactory>	imaging_factory;
		ComPtr<IDWriteFactory>		dwrite_factory;

		D2D1_FACTORY_OPTIONS config;
		ZeroMemory(&config, sizeof(D2D1_FACTORY_OPTIONS));
#if defined(KGE_DEBUG) && defined(KGE_ENABLE_DX_DEBUG)
		config.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&config,
			reinterpret_cast<void**>(&factory)
		);

		if (SUCCEEDED(hr))
		{
			factory_ = factory;

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

			ComPtr<ID2D1StrokeStyle> miter_stroke_style;
			ComPtr<ID2D1StrokeStyle> bevel_stroke_style;
			ComPtr<ID2D1StrokeStyle> round_stroke_style;

			D2D1_STROKE_STYLE_PROPERTIES stroke_style = D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f
			);

			hr = factory_->CreateStrokeStyle(
				stroke_style,
				nullptr,
				0,
				&miter_stroke_style
			);

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;
				hr = factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&bevel_stroke_style
				);
			}

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;
				hr = factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&round_stroke_style
				);
			}

			if (SUCCEEDED(hr))
			{
				miter_stroke_style_ = miter_stroke_style;
				bevel_stroke_style_ = bevel_stroke_style;
				round_stroke_style_ = round_stroke_style;
			}
		}

		return hr;
	}

	HRESULT D2DDeviceResources::SetD2DDevice(_In_ ComPtr<ID2D1Device> const& device)
	{
		ComPtr<ID2D1DeviceContext> device_ctx;

		HRESULT hr = device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&device_ctx
		);

		if (SUCCEEDED(hr))
		{
			device_ = device;
			device_context_ = device_ctx;
			device_context_->SetDpi(dpi_, dpi_);
		}

		return hr;
	}

	void D2DDeviceResources::SetTargetBitmap(_In_ ComPtr<ID2D1Bitmap1> const& target)
	{
		target_bitmap_ = target;
		if (device_context_)
			device_context_->SetTarget(target_bitmap_.get());
	}

	HRESULT D2DDeviceResources::CreateBitmapConverter(_Out_ ComPtr<IWICFormatConverter>& converter, _In_opt_ ComPtr<IWICBitmapSource> source,
		_In_ REFWICPixelFormatGUID format, WICBitmapDitherType dither, _In_opt_ ComPtr<IWICPalette> palette, double alpha_threshold_percent,
		WICBitmapPaletteType palette_translate
	)
	{
		if (!imaging_factory_)
			return E_UNEXPECTED;

		ComPtr<IWICFormatConverter> output;
		HRESULT hr = imaging_factory_->CreateFormatConverter(&output);

		if (SUCCEEDED(hr))
		{
			hr = output->Initialize(
				source.get(),
				format,
				dither,
				palette.get(),
				alpha_threshold_percent,
				palette_translate
			);
		}

		if (SUCCEEDED(hr))
		{
			converter = output;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateBitmapFromConverter(_Out_ ComPtr<ID2D1Bitmap>& bitmap, _In_opt_ const D2D1_BITMAP_PROPERTIES* properties,
		_In_ ComPtr<IWICFormatConverter> converter)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		ComPtr<ID2D1Bitmap> output;
		HRESULT hr = device_context_->CreateBitmapFromWicBitmap(
			converter.get(),
			properties,
			&output
		);

		if (SUCCEEDED(hr))
		{
			bitmap = output;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateBitmapDecoderFromFile(_Out_ ComPtr<IWICBitmapDecoder>& decoder, const String& file_path)
	{
		if (!imaging_factory_)
			return E_UNEXPECTED;

		ComPtr<IWICBitmapDecoder> decoder_output;
		HRESULT hr = imaging_factory_->CreateDecoderFromFilename(
			file_path.c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&decoder_output
		);

		if (SUCCEEDED(hr))
		{
			decoder = decoder_output;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateBitmapDecoderFromResource(_Out_ ComPtr<IWICBitmapDecoder>& decoder, const Resource& resource)
	{
		if (!imaging_factory_)
			return E_UNEXPECTED;

		Resource::Data res_data = resource.GetData();
		HRESULT hr = res_data ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICStream> stream;
			hr = imaging_factory_->CreateStream(&stream);

			if (SUCCEEDED(hr))
			{
				hr = stream->InitializeFromMemory(
					static_cast<WICInProcPointer>(res_data.buffer),
					res_data.size
				);
			}

			if (SUCCEEDED(hr))
			{
				ComPtr<IWICBitmapDecoder> decoder_output;
				hr = imaging_factory_->CreateDecoderFromStream(
					stream.get(),
					nullptr,
					WICDecodeMetadataCacheOnLoad,
					&decoder_output
				);

				if (SUCCEEDED(hr))
				{
					decoder = decoder_output;
				}
			}
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateTextFormat(_Out_ ComPtr<IDWriteTextFormat> & text_format, _In_ Font const & font) const
	{
		if (!dwrite_factory_)
			return E_UNEXPECTED;

		ComPtr<IDWriteTextFormat> output;
		HRESULT hr = dwrite_factory_->CreateTextFormat(
			font.family.c_str(),
			font.collection.GetFontCollection().get(),
			DWRITE_FONT_WEIGHT(font.weight),
			font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			font.size,
			L"",
			&output
		);

		if (SUCCEEDED(hr))
		{
			text_format = output;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateTextLayout(_Out_ ComPtr<IDWriteTextLayout>& text_layout, _In_ String const& text,
		_In_ ComPtr<IDWriteTextFormat> const& text_format) const
	{
		if (!dwrite_factory_)
			return E_UNEXPECTED;

		ComPtr<IDWriteTextLayout> output;
		HRESULT hr = dwrite_factory_->CreateTextLayout(
			text.c_str(),
			static_cast<UINT32>(text.length()),
			text_format.get(),
			0,
			0,
			&output
		);

		if (SUCCEEDED(hr))
		{
			text_layout = output;
		}
		return hr;
	}

}
