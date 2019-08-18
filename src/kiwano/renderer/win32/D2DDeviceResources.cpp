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
#include "../../utils/FileUtil.h"

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
		HRESULT CreateBitmapFromFile(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_ String const& file_path
		) override;

		HRESULT CreateBitmapFromResource(
			_Out_ ComPtr<ID2D1Bitmap>& bitmap,
			_In_ Resource const& res
		) override;

		HRESULT CreateTextFormat(
			_Out_ ComPtr<IDWriteTextFormat>& text_format,
			_In_ Font const& font
		) const override;

		HRESULT CreateTextLayout(
			_Out_ ComPtr<IDWriteTextLayout>& text_layout,
			_In_ String const& text,
			_In_ TextStyle const& text_style,
			_In_ ComPtr<IDWriteTextFormat> const& text_format
		) const override;

		HRESULT SetD2DDevice(
			_In_ ComPtr<ID2D1Device> const& device
		) override;

		void SetTargetBitmap(
			_In_ ComPtr<ID2D1Bitmap1> const& target
		) override;

		ID2D1StrokeStyle* GetStrokeStyle(StrokeStyle stroke) const override;

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
		float dpi_;

		ComPtr<ID2D1StrokeStyle>	d2d_miter_stroke_style_;
		ComPtr<ID2D1StrokeStyle>	d2d_bevel_stroke_style_;
		ComPtr<ID2D1StrokeStyle>	d2d_round_stroke_style_;
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

		d2d_miter_stroke_style_.reset();
		d2d_bevel_stroke_style_.reset();
		d2d_round_stroke_style_.reset();
	}

	HRESULT D2DDeviceResources::CreateDeviceIndependentResources()
	{
		HRESULT hr = S_OK;

		ComPtr<ID2D1Factory1>		d2d_factory;
		ComPtr<IWICImagingFactory>	imaging_factory;
		ComPtr<IDWriteFactory>		dwrite_factory;

		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
#ifdef KGE_DEBUG
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
			factory_ = d2d_factory;

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

			ComPtr<ID2D1StrokeStyle> d2d_miter_stroke_style;
			ComPtr<ID2D1StrokeStyle> d2d_bevel_stroke_style;
			ComPtr<ID2D1StrokeStyle> d2d_round_stroke_style;

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
				&d2d_miter_stroke_style
			);

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;
				hr = factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d_bevel_stroke_style
				);
			}

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;
				hr = factory_->CreateStrokeStyle(
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

	HRESULT D2DDeviceResources::SetD2DDevice(_In_ ComPtr<ID2D1Device> const& device)
	{
		ComPtr<ID2D1DeviceContext> d2d_device_ctx;

		HRESULT hr = device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&d2d_device_ctx
		);

		if (SUCCEEDED(hr))
		{
			device_ = device;
			device_context_ = d2d_device_ctx;
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

	HRESULT D2DDeviceResources::CreateBitmapFromFile(_Out_ ComPtr<ID2D1Bitmap> & bitmap, _In_ String const & file_path)
	{
		if (!imaging_factory_ || !device_context_)
			return E_UNEXPECTED;

		if (!FileUtil::ExistsFile(file_path))
		{
			KGE_WARNING_LOG(L"Image file '%s' not found!", file_path.c_str());
			return E_FAIL;
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
			// 图片格式转换成 32bppPBGRA
			hr = converter->Initialize(
				source.get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = device_context_->CreateBitmapFromWicBitmap(
				converter.get(),
				nullptr,
				&bitmap_tmp
			);
		}

		if (SUCCEEDED(hr))
		{
			bitmap = bitmap_tmp;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateBitmapFromResource(_Out_ ComPtr<ID2D1Bitmap> & bitmap, _In_ Resource const & res)
	{
		if (!imaging_factory_ || !device_context_)
			return E_UNEXPECTED;

		ComPtr<IWICBitmapDecoder>		decoder;
		ComPtr<IWICBitmapFrameDecode>	source;
		ComPtr<IWICStream>				stream;
		ComPtr<IWICFormatConverter>		converter;
		ComPtr<ID2D1Bitmap>				bitmap_tmp;

		// 加载资源
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
				stream.get(),
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
			// 图片格式转换成 32bppPBGRA
			hr = converter->Initialize(
				source.get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = device_context_->CreateBitmapFromWicBitmap(
				converter.get(),
				nullptr,
				&bitmap_tmp
			);
		}

		if (SUCCEEDED(hr))
		{
			bitmap = bitmap_tmp;
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
			L"en-us",
			&output
		);

		if (SUCCEEDED(hr))
		{
			text_format = output;
		}
		return hr;
	}

	HRESULT D2DDeviceResources::CreateTextLayout(_Out_ ComPtr<IDWriteTextLayout> & text_layout, _In_ String const & text,
		_In_ TextStyle const & text_style, _In_ ComPtr<IDWriteTextFormat> const& text_format) const
	{
		if (!dwrite_factory_)
			return E_UNEXPECTED;

		HRESULT hr;
		ComPtr<IDWriteTextLayout> output;
		UINT32 length = static_cast<UINT32>(text.length());

		if (text_style.wrap_width > 0)
		{
			hr = dwrite_factory_->CreateTextLayout(
				text.c_str(),
				length,
				text_format.get(),
				text_style.wrap_width,
				0,
				&output
			);
		}
		else
		{
			hr = dwrite_factory_->CreateTextLayout(
				text.c_str(),
				length,
				text_format.get(),
				0,
				0,
				&output
			);
		}

		if (SUCCEEDED(hr))
		{
			if (text_style.line_spacing == 0.f)
			{
				hr = output->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
			}
			else
			{
				hr = output->SetLineSpacing(
					DWRITE_LINE_SPACING_METHOD_UNIFORM,
					text_style.line_spacing,
					text_style.line_spacing * 0.8f
				);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = output->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(text_style.alignment));
		}

		if (SUCCEEDED(hr))
		{
			hr = output->SetWordWrapping((text_style.wrap_width > 0) ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
		}

		if (SUCCEEDED(hr))
		{
			if (text_style.underline)
			{
				hr = output->SetUnderline(true, { 0, length });
			}
		}

		if (SUCCEEDED(hr))
		{
			if (text_style.strikethrough)
			{
				output->SetStrikethrough(true, { 0, length });
			}
		}

		if (SUCCEEDED(hr))
		{
			// Fix the layout width when the text does not wrap
			if (!(text_style.wrap_width > 0))
			{
				DWRITE_TEXT_METRICS metrics;
				hr = output->GetMetrics(&metrics);
				
				if (SUCCEEDED(hr))
				{
					hr = output->SetMaxWidth(metrics.width);
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			text_layout = output;
		}
		return hr;
	}

	ID2D1StrokeStyle* D2DDeviceResources::GetStrokeStyle(StrokeStyle stroke) const
	{
		switch (stroke)
		{
		case StrokeStyle::Miter: return d2d_miter_stroke_style_.get(); break;
		case StrokeStyle::Bevel: return d2d_bevel_stroke_style_.get(); break;
		case StrokeStyle::Round: return d2d_round_stroke_style_.get(); break;
		}
		return nullptr;
	}

}
