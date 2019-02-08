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
	Factory::Factory()
	{
	}

	Factory::~Factory()
	{
	}

	HRESULT Factory::Init(bool debug)
	{
		E2D_LOG(L"Creating device-independent resources");

		D2D1_FACTORY_OPTIONS options;
		options.debugLevel = debug ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE;
		HRESULT hr = modules::DirectX::Get().D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory),
			&options,
			reinterpret_cast<void**>(&factory_)
		);

		if (SUCCEEDED(hr))
		{
			CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				__uuidof(IWICImagingFactory),
				reinterpret_cast<void**>(&imaging_factory_)
			);
		}

		if (SUCCEEDED(hr))
		{
			modules::DirectX::Get().DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&write_factory_)
			);
		}

		if (SUCCEEDED(hr))
		{
			auto stroke_style = D2D1::StrokeStyleProperties(
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
				&miter_stroke_style_
			);

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;
				hr = factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&bevel_stroke_style_
				);
			}

			if (SUCCEEDED(hr))
			{
				stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;
				hr = factory_->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&round_stroke_style_
				);
			}
		}
		return hr;
	}

	void Factory::Destroy()
	{
		E2D_LOG(L"Destroying device-independent resources");

		factory_.Reset();
		imaging_factory_.Reset();
		write_factory_.Reset();
		miter_stroke_style_.Reset();
		bevel_stroke_style_.Reset();
		round_stroke_style_.Reset();
	}

	HRESULT Factory::CreateHwndRenderTarget(D2DHwndRenderTargetPtr & hwnd_render_target, D2D1_RENDER_TARGET_PROPERTIES const & properties, D2D1_HWND_RENDER_TARGET_PROPERTIES const & hwnd_rt_properties) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		D2DHwndRenderTargetPtr hwnd_render_target_tmp;
		HRESULT hr = factory_->CreateHwndRenderTarget(
			properties,
			hwnd_rt_properties,
			&hwnd_render_target_tmp
		);

		if (SUCCEEDED(hr))
			hwnd_render_target = hwnd_render_target_tmp;
		return hr;
	}

	HRESULT Factory::CreateTextRenderer(
		D2DTextRendererPtr& text_renderer,
		D2DRenderTargetPtr const& render_target,
		D2DSolidColorBrushPtr const& brush
	)
	{
		if (!factory_)
			return E_UNEXPECTED;

		D2DTextRendererPtr text_renderer_tmp;
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

	HRESULT Factory::CreateBitmapFromFile(D2DBitmapPtr & bitmap, D2DRenderTargetPtr const & rt, String const & file_path)
	{
		if (imaging_factory_ == nullptr)
		{
			return E_UNEXPECTED;
		}

		IntrusivePtr<IWICBitmapDecoder>		decoder;
		IntrusivePtr<IWICBitmapFrameDecode>	source;
		IntrusivePtr<IWICStream>			stream;
		IntrusivePtr<IWICFormatConverter>	converter;
		IntrusivePtr<ID2D1Bitmap>			bitmap_tmp;

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

	HRESULT Factory::CreateBitmapFromResource(D2DBitmapPtr & bitmap, D2DRenderTargetPtr const & rt, Resource const & res)
	{
		if (imaging_factory_ == nullptr)
		{
			return E_UNEXPECTED;
		}

		IntrusivePtr<IWICBitmapDecoder>		decoder;
		IntrusivePtr<IWICBitmapFrameDecode>	source;
		IntrusivePtr<IWICStream>			stream;
		IntrusivePtr<IWICFormatConverter>	converter;
		IntrusivePtr<ID2D1Bitmap>			bitmap_tmp;
		
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

	HRESULT Factory::CreateRectangleGeometry(D2DRectangleGeometryPtr & geo, Rect const& rect) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		D2DRectangleGeometryPtr rectangle;
		HRESULT hr = factory_->CreateRectangleGeometry(
			ToD2dRectF(rect),
			&rectangle
		);

		if (SUCCEEDED(hr))
			geo = rectangle;
		return hr;
	}

	HRESULT Factory::CreateRoundedRectangleGeometry(D2DRoundedRectangleGeometryPtr & geo, Rect const & rect, float radius_x, float radius_y) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		D2DRoundedRectangleGeometryPtr rounded_rect;
		HRESULT hr = factory_->CreateRoundedRectangleGeometry(
			D2D1::RoundedRect(
				ToD2dRectF(rect),
				radius_x,
				radius_y
			),
			&rounded_rect
		);

		if (SUCCEEDED(hr))
			geo = rounded_rect;
		return hr;
	}

	HRESULT Factory::CreateEllipseGeometry(D2DEllipseGeometryPtr & geo, Point const & center, float radius_x, float radius_y) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		D2DEllipseGeometryPtr ellipse;
		HRESULT hr = factory_->CreateEllipseGeometry(
			D2D1::Ellipse(
				ToD2dPoint2F(center),
				radius_x,
				radius_y
			),
			&ellipse
		);

		if (SUCCEEDED(hr))
			geo = ellipse;
		return hr;
	}

	HRESULT Factory::CreateTransformedGeometry(
		D2DTransformedGeometryPtr& transformed,
		Matrix const& matrix,
		D2DGeometryPtr const& geo
	) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		D2DTransformedGeometryPtr transformed_tmp;
		HRESULT hr = factory_->CreateTransformedGeometry(
			geo.Get(),
			matrix,
			&transformed_tmp
		);

		if (SUCCEEDED(hr))
		{
			transformed = transformed_tmp;
		}
		return hr;
	}

	HRESULT Factory::CreatePathGeometry(D2DPathGeometryPtr & geometry) const
	{
		if (!factory_)
			return E_UNEXPECTED;

		return factory_->CreatePathGeometry(&geometry);
	}

	HRESULT Factory::CreateTextFormat(D2DTextFormatPtr & text_format, Font const & font, TextStyle const & text_style) const
	{
		if (!write_factory_)
			return E_UNEXPECTED;

		D2DTextFormatPtr text_format_tmp;
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

	HRESULT Factory::CreateTextLayout(D2DTextLayoutPtr & text_layout, Size& layout_size, String const & text, D2DTextFormatPtr const& text_format, TextStyle const & text_style) const
	{
		if (!write_factory_)
			return E_UNEXPECTED;

		text_layout = nullptr;

		HRESULT hr;
		D2DTextLayoutPtr text_layout_tmp;
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
			if (SUCCEEDED(hr))
			{
				hr = text_layout_tmp->GetMetrics(&metrics);
			}

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

	D2DStrokeStylePtr const& Factory::GetStrokeStyle(StrokeStyle stroke) const
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
