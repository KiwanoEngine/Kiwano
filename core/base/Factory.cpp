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

#include "Factory.h"
#include "logs.h"
#include "modules.h"
#include "Transform.hpp"

namespace easy2d
{
	FactoryImpl::FactoryImpl()
		: initialized_(false)
	{
	}

	FactoryImpl::~FactoryImpl()
	{
		E2D_LOG("Destroying device independent resources");
	}

	void FactoryImpl::Init(bool debug)
	{
		if (initialized_)
			return;

		E2D_LOG("Creating device independent resources");

		D2D1_FACTORY_OPTIONS fact_options;
		fact_options.debugLevel = debug ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE;
		ThrowIfFailed(
			modules::DirectX().D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				__uuidof(ID2D1Factory),
				&fact_options,
				reinterpret_cast<void**>(&factory_)
			)
		);

		ThrowIfFailed(
			CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				__uuidof(IWICImagingFactory),
				reinterpret_cast<void**>(&imaging_factory_)
			)
		);

		ThrowIfFailed(
			modules::DirectX().DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&write_factory_)
			)
		);

		auto stroke_style = D2D1::StrokeStyleProperties(
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_LINE_JOIN_MITER,
			2.0f,
			D2D1_DASH_STYLE_SOLID,
			0.0f
		);

		ThrowIfFailed(
			factory_->CreateStrokeStyle(
				stroke_style,
				nullptr,
				0,
				&miter_stroke_style_
			)
		);

		stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;
		ThrowIfFailed(
			factory_->CreateStrokeStyle(
				stroke_style,
				nullptr,
				0,
				&bevel_stroke_style_
			)
		);

		stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;
		ThrowIfFailed(
			factory_->CreateStrokeStyle(
				stroke_style,
				nullptr,
				0,
				&round_stroke_style_
			)
		);

		initialized_ = true;
	}

	HRESULT FactoryImpl::CreateHwndRenderTarget(cpHwndRenderTarget & hwnd_render_target, D2D1_RENDER_TARGET_PROPERTIES const & properties, D2D1_HWND_RENDER_TARGET_PROPERTIES const & hwnd_rt_properties) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		cpHwndRenderTarget hwnd_render_target_tmp;
		HRESULT hr = factory_->CreateHwndRenderTarget(
			properties,
			hwnd_rt_properties,
			&hwnd_render_target_tmp
		);

		if (SUCCEEDED(hr))
			hwnd_render_target = hwnd_render_target_tmp;
		return hr;
	}

	HRESULT FactoryImpl::CreateTextRenderer(
		cpTextRenderer& text_renderer,
		cpRenderTarget const& render_target,
		cpSolidColorBrush const& brush
	)
	{
		if (!factory_)
			return E_UNEXPECTED;

		cpTextRenderer text_renderer_tmp;
		HRESULT hr = ITextRenderer::Create(
			&text_renderer_tmp,
			factory_.Get(),
			render_target.Get(),
			brush.Get()
		);

		if (SUCCEEDED(hr))
			text_renderer = text_renderer_tmp;
		return hr;
	}

	HRESULT FactoryImpl::CreateBitmapFromFile(cpBitmap & bitmap, cpRenderTarget const & rt, String const & file_path)
	{
		if (imaging_factory_ == nullptr)
		{
			return E_UNEXPECTED;
		}

		using namespace intrusive;

		SmartPointer<IWICBitmapDecoder>		decoder;
		SmartPointer<IWICBitmapFrameDecode>	source;
		SmartPointer<IWICStream>			stream;
		SmartPointer<IWICFormatConverter>	converter;
		SmartPointer<ID2D1Bitmap>			bitmap_tmp;

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
			hr = rt->CreateBitmapFromWicBitmap(
				converter.Get(),
				nullptr,
				&bitmap_tmp
			);
		}

		if (SUCCEEDED(hr))
			bitmap = bitmap_tmp;

		return hr;
	}

	HRESULT FactoryImpl::CreateBitmapFromResource(cpBitmap & bitmap, cpRenderTarget const & rt, Resource const & res)
	{
		if (imaging_factory_ == nullptr)
		{
			return E_UNEXPECTED;
		}

		using namespace intrusive;

		SmartPointer<IWICBitmapDecoder>		decoder;
		SmartPointer<IWICBitmapFrameDecode>	source;
		SmartPointer<IWICStream>			stream;
		SmartPointer<IWICFormatConverter>	converter;
		SmartPointer<ID2D1Bitmap>			bitmap_tmp;
		
		// 加载资源
		ResourceData buffer;
		HRESULT hr = res.Load(&buffer) ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = imaging_factory_->CreateStream(&stream);
		}

		if (SUCCEEDED(hr))
		{
			hr = stream->InitializeFromMemory(
				static_cast<WICInProcPointer>(buffer.buffer),
				buffer.buffer_size
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
			// 图片格式转换成 32bppPBGRA
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
			hr = rt->CreateBitmapFromWicBitmap(
				converter.Get(),
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

	HRESULT FactoryImpl::CreateRectangleGeometry(cpRectangleGeometry & geo, Rect const& rect) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		cpRectangleGeometry rectangle;
		HRESULT hr = factory_->CreateRectangleGeometry(
			rect,
			&rectangle
		);

		if (SUCCEEDED(hr))
			geo = rectangle;
		return hr;
	}

	HRESULT FactoryImpl::CreateRoundedRectangleGeometry(cpRoundedRectangleGeometry & geo, Rect const & rect, float radius_x, float radius_y) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		cpRoundedRectangleGeometry rounded_rect;
		HRESULT hr = factory_->CreateRoundedRectangleGeometry(
			D2D1::RoundedRect(
				rect,
				radius_x,
				radius_y
			),
			&rounded_rect
		);

		if (SUCCEEDED(hr))
			geo = rounded_rect;
		return hr;
	}

	HRESULT FactoryImpl::CreateEllipseGeometry(cpEllipseGeometry & geo, Point const & center, float radius_x, float radius_y) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		cpEllipseGeometry ellipse;
		HRESULT hr = factory_->CreateEllipseGeometry(
			D2D1::Ellipse(
				center,
				radius_x,
				radius_y
			),
			&ellipse
		);

		if (SUCCEEDED(hr))
			geo = ellipse;
		return hr;
	}

	HRESULT FactoryImpl::CreateTransformedGeometry(
		cpTransformedGeometry& transformed,
		math::Matrix const& matrix,
		cpGeometry const& geo
	) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		cpTransformedGeometry transformed_tmp;
		HRESULT hr = factory_->CreateTransformedGeometry(
			geo.Get(),
			ConvertToD2DMatrix(matrix),
			&transformed_tmp
		);

		if (SUCCEEDED(hr))
		{
			transformed = transformed_tmp;
		}
		return hr;
	}

	HRESULT FactoryImpl::CreatePathGeometry(cpPathGeometry & geometry) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		return factory_->CreatePathGeometry(&geometry);
	}

	HRESULT FactoryImpl::CreateTextFormat(cpTextFormat & text_format, Font const & font, TextStyle const & text_style) const
	{
		if (!write_factory_)
			return E_UNEXPECTED;

		cpTextFormat text_format_tmp;
		HRESULT hr = write_factory_->CreateTextFormat(
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

	HRESULT FactoryImpl::CreateTextLayout(cpTextLayout & text_layout, Size& layout_size, String const & text, cpTextFormat const& text_format, TextStyle const & text_style) const
	{
		if (!write_factory_)
			return E_UNEXPECTED;

		text_layout = nullptr;

		HRESULT hr;
		cpTextLayout text_layout_tmp;
		UINT32 length = static_cast<UINT32>(text.length());

		if (text_style.wrap)
		{
			hr = write_factory_->CreateTextLayout(
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
			hr = write_factory_->CreateTextLayout(
				text.c_str(),
				length,
				text_format.Get(),
				0,
				0,
				&text_layout_tmp
			);

			DWRITE_TEXT_METRICS metrics;
			text_layout_tmp->GetMetrics(&metrics);

			if (SUCCEEDED(hr))
			{
				text_layout_tmp = nullptr;
				hr = write_factory_->CreateTextLayout(
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

	cpStrokeStyle const& FactoryImpl::GetStrokeStyle(StrokeStyle stroke) const
	{
		switch (stroke)
		{
		case StrokeStyle::Miter:
			return miter_stroke_style_;
			break;
		case StrokeStyle::Bevel:
			return bevel_stroke_style_;
			break;
		case StrokeStyle::Round:
			return round_stroke_style_;
			break;
		}
		return miter_stroke_style_;
	}

}
