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

#include "render.h"
#include "time.h"
#include "base.hpp"
#include "logs.h"
#include "modules.h"
#include "Image.h"
#include "Transform.hpp"

namespace easy2d
{
	namespace devices
	{
		GraphicsDevice::GraphicsDevice()
			: fps_text_format_(nullptr)
			, fps_text_layout_(nullptr)
			, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
			, opacity_(1.f)
			, window_occluded(false)
			, initialized(false)
		{
			ZeroMemory(&d2d, sizeof(D2DResources));
		}

		GraphicsDevice::~GraphicsDevice()
		{
			E2D_LOG("Destroying graphics device");

			ClearImageCache();
		}

		void GraphicsDevice::Init(HWND hwnd, bool debug)
		{
			if (initialized)
				return;

			E2D_LOG("Initing graphics device");

			D2D1_FACTORY_OPTIONS options{ debug ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE };
			ThrowIfFailed(
				modules::DirectX().D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED,
					__uuidof(ID2D1Factory),
					&options,
					reinterpret_cast<void**>(&d2d.factory)
				)
			);

			ThrowIfFailed(
				CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					__uuidof(IWICImagingFactory),
					reinterpret_cast<void**>(&d2d.imaging_factory)
				)
			);

			ThrowIfFailed(
				modules::DirectX().DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory),
					reinterpret_cast<IUnknown**>(&d2d.write_factory)
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
				d2d.factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d.miter_stroke_style
				)
			);

			stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;
			ThrowIfFailed(
				d2d.factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d.bevel_stroke_style
				)
			);

			stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;
			ThrowIfFailed(
				d2d.factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d.round_stroke_style
				)
			);

			CreateDeviceResources(hwnd);

			initialized = true;
		}

		void GraphicsDevice::BeginDraw(HWND hwnd)
		{
			CreateDeviceResources(hwnd);

			window_occluded = !!(d2d.render_target->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED);

			if (!window_occluded)
			{
				d2d.render_target->BeginDraw();
				d2d.render_target->Clear(clear_color_);
			}
		}

		void GraphicsDevice::EndDraw()
		{
			if (!window_occluded)
			{
				HRESULT hr = d2d.render_target->EndDraw();

				if (hr == D2DERR_RECREATE_TARGET)
				{
					// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
					// 并在下一次调用时重建资源
					hr = S_OK;

					fps_text_format_ = nullptr;
					fps_text_layout_ = nullptr;
					d2d.text_renderer = nullptr;
					d2d.solid_brush = nullptr;
					d2d.render_target = nullptr;
				}

				ThrowIfFailed(hr);
			}
		}

		void GraphicsDevice::ClearImageCache()
		{
			bitmap_cache_.clear();
		}

		HRESULT GraphicsDevice::CreateRectangleGeometry(cpRectangleGeometry & geo, Rect const& rect) const
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			cpRectangleGeometry rectangle;
			HRESULT hr = d2d.factory->CreateRectangleGeometry(
				rect,
				&rectangle
			);

			if (SUCCEEDED(hr))
				geo = rectangle;
			return hr;
		}

		HRESULT GraphicsDevice::CreateRoundedRectangleGeometry(cpRoundedRectangleGeometry & geo, Rect const & rect, float radius_x, float radius_y) const
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			cpRoundedRectangleGeometry rounded_rect;
			HRESULT hr = d2d.factory->CreateRoundedRectangleGeometry(
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

		HRESULT GraphicsDevice::CreateEllipseGeometry(cpEllipseGeometry & geo, Point const & center, float radius_x, float radius_y) const
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			cpEllipseGeometry ellipse;
			HRESULT hr = d2d.factory->CreateEllipseGeometry(
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

		HRESULT GraphicsDevice::CreateTransformedGeometry(
			cpTransformedGeometry& transformed,
			math::Matrix const& matrix,
			cpGeometry const& geo
		) const
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			cpTransformedGeometry transformed_tmp;
			HRESULT hr = d2d.factory->CreateTransformedGeometry(
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

		HRESULT GraphicsDevice::CreatePathGeometry(cpPathGeometry & geometry) const
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			return d2d.factory->CreatePathGeometry(&geometry);
		}

		HRESULT GraphicsDevice::CreateTextFormat(cpTextFormat & text_format, Font const & font, TextStyle const & text_style) const
		{
			if (!d2d.write_factory)
				return E_UNEXPECTED;

			cpTextFormat text_format_tmp;
			HRESULT hr = d2d.write_factory->CreateTextFormat(
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

		HRESULT GraphicsDevice::CreateTextLayout(cpTextLayout & text_layout, Size& layout_size, String const & text, cpTextFormat const& text_format, TextStyle const & text_style) const
		{
			if (!d2d.write_factory)
				return E_UNEXPECTED;

			text_layout = nullptr;

			HRESULT hr;
			cpTextLayout text_layout_tmp;
			UINT32 length = static_cast<UINT32>(text.length());

			if (text_style.wrap)
			{
				hr = d2d.write_factory->CreateTextLayout(
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
				hr = d2d.write_factory->CreateTextLayout(
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
					hr = d2d.write_factory->CreateTextLayout(
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

		HRESULT GraphicsDevice::CreateTextRenderer(
			cpTextRenderer& text_renderer,
			cpRenderTarget const& render_target,
			cpSolidColorBrush const& brush
		)
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			cpTextRenderer text_renderer_tmp;
			HRESULT hr = ITextRenderer::Create(
				&text_renderer_tmp,
				d2d.factory.Get(),
				render_target.Get(),
				brush.Get()
			);

			if (SUCCEEDED(hr))
				text_renderer = text_renderer_tmp;
			return hr;
		}

		HRESULT GraphicsDevice::CreateLayer(cpLayer& layer)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			layer = nullptr;
			return d2d.render_target->CreateLayer(&layer);
		}

		HRESULT GraphicsDevice::CreateSolidColorBrush(cpSolidColorBrush & brush) const
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			brush = nullptr;
			return d2d.render_target->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&brush
			);
		}

		HRESULT GraphicsDevice::DrawGeometry(
			cpGeometry const& geometry,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke
		)
		{
			if (!d2d.solid_brush ||
				!d2d.render_target)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			d2d.solid_brush->SetColor(stroke_color);
			d2d.render_target->DrawGeometry(
				geometry.Get(),
				d2d.solid_brush.Get(),
				stroke_width,
				GetStrokeStyle(stroke).Get()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::FillGeometry(cpGeometry const & geometry, const Color & fill_color)
		{
			if (!d2d.solid_brush ||
				!d2d.render_target)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			d2d.solid_brush->SetColor(fill_color);
			d2d.render_target->FillGeometry(
				geometry.Get(),
				d2d.solid_brush.Get()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawImage(spImage const & image)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			if (!image->GetBitmap())
				return S_OK;

			if (window_occluded)
				return S_OK;

			d2d.render_target->DrawBitmap(
				image->GetBitmap().Get(),
				D2D1::RectF(0.f, 0.f, image->GetWidth(), image->GetHeight()),
				opacity_,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				image->GetCropRect()
			);
			return S_OK;
		}

		cpStrokeStyle const& GraphicsDevice::GetStrokeStyle(StrokeStyle stroke) const
		{
			switch (stroke)
			{
			case StrokeStyle::Miter:
				return d2d.miter_stroke_style;
				break;
			case StrokeStyle::Bevel:
				return d2d.bevel_stroke_style;
				break;
			case StrokeStyle::Round:
				return d2d.round_stroke_style;
				break;
			}
			return d2d.miter_stroke_style;
		}

		HRESULT GraphicsDevice::DrawBitmap(
			cpBitmap const& bitmap
		)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			// Do not crop bitmap 
			auto rect = D2D1::RectF(0.f, 0.f, bitmap->GetSize().width, bitmap->GetSize().height);
			d2d.render_target->DrawBitmap(
				bitmap.Get(),
				rect,
				opacity_,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				rect
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawTextLayout(cpTextLayout const& text_layout)
		{
			if (!d2d.text_renderer)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			return text_layout->Draw(nullptr, d2d.text_renderer.Get(), 0, 0);
		}

		HRESULT GraphicsDevice::PushClip(const math::Matrix & clip_matrix, const Size & clip_size)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			d2d.render_target->SetTransform(ConvertToD2DMatrix(clip_matrix));
			d2d.render_target->PushAxisAlignedClip(
				D2D1::RectF(0, 0, clip_size.width, clip_size.height),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::PopClip()
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			d2d.render_target->PopAxisAlignedClip();
			return S_OK;
		}

		HRESULT GraphicsDevice::PushLayer(cpLayer const& layer, LayerProperties const& properties)
		{
			if (!d2d.render_target ||
				!d2d.solid_brush)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			d2d.render_target->PushLayer(
				D2D1::LayerParameters(
					properties.area,
					nullptr,
					D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
					D2D1::Matrix3x2F::Identity(),
					properties.opacity,
					d2d.solid_brush.Get(),
					D2D1_LAYER_OPTIONS_NONE
				),
				layer.Get()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::PopLayer()
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			if (window_occluded)
				return S_OK;

			d2d.render_target->PopLayer();
			return S_OK;
		}

		HRESULT GraphicsDevice::GetSize(Size & size)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			auto rtsize = d2d.render_target->GetSize();
			size.width = rtsize.width;
			size.height = rtsize.height;
			return S_OK;
		}

		HRESULT GraphicsDevice::CreateBitmapFromFile(cpBitmap& bitmap, String const& file_path)
		{
			if (d2d.imaging_factory == nullptr ||
				d2d.render_target == nullptr)
			{
				return E_UNEXPECTED;
			}

			size_t hash_code = std::hash<String>{}(file_path);
			if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
			{
				bitmap = bitmap_cache_[hash_code];
				return S_OK;
			}

			IWICBitmapDecoder*		decoder = nullptr;
			IWICBitmapFrameDecode*	source = nullptr;
			IWICStream*				stream = nullptr;
			IWICFormatConverter*	converter = nullptr;

			// 创建解码器
			HRESULT hr = d2d.imaging_factory->CreateDecoderFromFilename(
				file_path.c_str(),
				nullptr,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&decoder
			);

			if (SUCCEEDED(hr))
			{
				// 创建初始化框架
				hr = decoder->GetFrame(0, &source);
			}

			if (SUCCEEDED(hr))
			{
				// 创建图片格式转换器
				hr = d2d.imaging_factory->CreateFormatConverter(&converter);
			}

			if (SUCCEEDED(hr))
			{
				// 图片格式转换成 32bppPBGRA
				hr = converter->Initialize(
					source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					nullptr,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}

			if (SUCCEEDED(hr))
			{
				// 从 WIC 位图创建一个 Direct2D 位图
				bitmap = nullptr;
				hr = d2d.render_target->CreateBitmapFromWicBitmap(
					converter,
					nullptr,
					&bitmap
				);
			}

			if (SUCCEEDED(hr))
			{
				bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
			}

			SafeRelease(decoder);
			SafeRelease(source);
			SafeRelease(stream);
			SafeRelease(converter);

			return hr;
		}

		HRESULT GraphicsDevice::CreateBitmapFromResource(cpBitmap& bitmap, Resource const& res)
		{
			if (d2d.imaging_factory == nullptr ||
				d2d.render_target == nullptr)
			{
				return E_UNEXPECTED;
			}

			size_t hash_code = res.GetHashCode();
			if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
			{
				bitmap = bitmap_cache_[hash_code];
				return S_OK;
			}

			HRESULT hr;

			HINSTANCE				hinstance = GetModuleHandle(nullptr);
			IWICBitmapDecoder*		decoder = nullptr;
			IWICBitmapFrameDecode*	source = nullptr;
			IWICStream*				stream = nullptr;
			IWICFormatConverter*	converter = nullptr;
			ResourceData			buffer;

			// 加载资源
			hr = res.Load(&buffer) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				// 创建 WIC 流
				hr = d2d.imaging_factory->CreateStream(&stream);
			}

			if (SUCCEEDED(hr))
			{
				// 初始化流
				hr = stream->InitializeFromMemory(
					static_cast<WICInProcPointer>(buffer.buffer),
					buffer.buffer_size
				);
			}

			if (SUCCEEDED(hr))
			{
				// 创建流的解码器
				hr = d2d.imaging_factory->CreateDecoderFromStream(
					stream,
					nullptr,
					WICDecodeMetadataCacheOnLoad,
					&decoder
				);
			}

			if (SUCCEEDED(hr))
			{
				// 创建初始化框架
				hr = decoder->GetFrame(0, &source);
			}

			if (SUCCEEDED(hr))
			{
				// 创建图片格式转换器
				hr = d2d.imaging_factory->CreateFormatConverter(&converter);
			}

			if (SUCCEEDED(hr))
			{
				// 图片格式转换成 32bppPBGRA
				hr = converter->Initialize(
					source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					nullptr,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}

			if (SUCCEEDED(hr))
			{
				// 从 WIC 位图创建一个 Direct2D 位图
				hr = d2d.render_target->CreateBitmapFromWicBitmap(
					converter,
					nullptr,
					&bitmap
				);
			}

			if (SUCCEEDED(hr))
			{
				bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
			}

			SafeRelease(decoder);
			SafeRelease(source);
			SafeRelease(stream);
			SafeRelease(converter);

			return hr;
		}

		HRESULT GraphicsDevice::CreateBitmapRenderTarget(cpBitmapRenderTarget & brt)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			brt = nullptr;
			return d2d.render_target->CreateCompatibleRenderTarget(&brt);
		}

		HRESULT GraphicsDevice::Resize(UINT32 width, UINT32 height)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			d2d.render_target->Resize(D2D1::SizeU(width, height));
			return S_OK;
		}

		HRESULT GraphicsDevice::SetTransform(const math::Matrix & matrix)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			d2d.render_target->SetTransform(ConvertToD2DMatrix(matrix));
			return S_OK;
		}

		HRESULT GraphicsDevice::SetOpacity(float opacity)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			opacity_ = opacity;
			d2d.solid_brush->SetOpacity(opacity);
			return S_OK;
		}

		HRESULT GraphicsDevice::SetTextStyle(
			Color const& color,
			bool has_outline,
			Color const& outline_color,
			float outline_width,
			StrokeStyle outline_stroke
		)
		{
			if (!d2d.text_renderer)
				return E_UNEXPECTED;

			d2d.text_renderer->SetTextStyle(
				color,
				has_outline,
				outline_color,
				outline_width,
				GetStrokeStyle(outline_stroke).Get()
			);
			return S_OK;
		}

		void GraphicsDevice::SetBackgroundColor(const Color& color)
		{
			clear_color_ = color;
		}

		void GraphicsDevice::CreateDeviceResources(HWND hwnd)
		{
			if (!d2d.render_target)
			{
				RECT rc;
				::GetClientRect(hwnd, &rc);

				D2D1_SIZE_U size = D2D1::SizeU(
					rc.right - rc.left,
					rc.bottom - rc.top
				);

				// 创建设备相关资源。这些资源应在 Direct2D 设备消失时重建
				// 创建一个 Direct2D 渲染目标
				ThrowIfFailed(
					d2d.factory->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(
							hwnd,
							size,
							D2D1_PRESENT_OPTIONS_NONE),
						&d2d.render_target
					)
				);
			}

			if (!d2d.solid_brush)
			{
				ThrowIfFailed(
					d2d.render_target->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::White),
						&d2d.solid_brush
					)
				);
			}

			if (!d2d.text_renderer)
			{
				ThrowIfFailed(
					ITextRenderer::Create(
						&d2d.text_renderer,
						d2d.factory.Get(),
						d2d.render_target.Get(),
						d2d.solid_brush.Get()
					)
				);
			}
		}

	}
}
