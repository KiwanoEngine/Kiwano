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

namespace easy2d
{
	namespace devices
	{
		namespace
		{
			inline D2D1_MATRIX_3X2_F ConvertToD2DMatrix(math::Matrix const& matrix)
			{
				return D2D1_MATRIX_3X2_F{
						matrix.m[0], matrix.m[1],
						matrix.m[2], matrix.m[3],
						matrix.m[4], matrix.m[5]
				};
			}
		}

		GraphicsDevice::GraphicsDevice()
			: fps_text_format_(nullptr)
			, fps_text_layout_(nullptr)
			, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
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

			d2d.render_target->BeginDraw();
			d2d.render_target->Clear(clear_color_);
		}

		void GraphicsDevice::EndDraw()
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

		void GraphicsDevice::ClearImageCache()
		{
			bitmap_cache_.clear();
		}

		HRESULT GraphicsDevice::CreateRectGeometry(
			cpGeometry& geometry,
			const math::Matrix& matrix,
			const Size& size
		) const
		{
			if (!d2d.factory)
				return E_UNEXPECTED;

			HRESULT hr;

			ID2D1RectangleGeometry * rectangle = nullptr;
			ID2D1TransformedGeometry * transformed = nullptr;

			hr = d2d.factory->CreateRectangleGeometry(
				D2D1::RectF(0, 0, size.width, size.height),
				&rectangle
			);

			if (SUCCEEDED(hr))
			{
				hr = d2d.factory->CreateTransformedGeometry(
					rectangle,
					ConvertToD2DMatrix(matrix),
					&transformed
				);
			}

			if (SUCCEEDED(hr))
			{
				geometry = transformed;
			}

			SafeRelease(rectangle);
			SafeRelease(transformed);
			return hr;
		}

		HRESULT GraphicsDevice::CreateTextFormat(cpTextFormat& text_format, Font const& font) const
		{
			if (!d2d.write_factory)
				return E_UNEXPECTED;

			text_format = nullptr;
			return d2d.write_factory->CreateTextFormat(
				font.family.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT(font.weight),
				font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				font.size,
				L"",
				&text_format
			);;
		}

		HRESULT GraphicsDevice::CreateTextLayout(cpTextLayout& text_layout, String const& text, cpTextFormat const& text_format, float wrap_width) const
		{
			if (!d2d.write_factory)
				return E_UNEXPECTED;

			text_layout = nullptr;

			UINT32 length = static_cast<UINT32>(text.length());
			return d2d.write_factory->CreateTextLayout(
				text.c_str(),
				length,
				text_format.Get(),
				wrap_width,
				0,
				&text_layout
			);
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
			Color const& border_color,
			float opacity,
			float stroke_width,
			StrokeStyle stroke
		)
		{
			if (!d2d.solid_brush ||
				!d2d.render_target)
				return E_UNEXPECTED;

			d2d.solid_brush->SetColor(border_color);
			d2d.solid_brush->SetOpacity(opacity);
			d2d.render_target->DrawGeometry(
				geometry.Get(),
				d2d.solid_brush.Get(),
				stroke_width,
				GetStrokeStyle(stroke).Get()
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

		cpRenderTarget const & GraphicsDevice::GetRenderTarget() const
		{
			return d2d.render_target;
		}

		HRESULT GraphicsDevice::DrawImage(
			spImage const& image,
			float opacity,
			const Rect & dest_rect,
			const Rect & source_rect
		)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

			d2d.render_target->DrawBitmap(
				image->GetBitmap().Get(),
				dest_rect,
				opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				source_rect
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawTextLayout(cpTextLayout const& text_layout)
		{
			if (!d2d.text_renderer)
				return E_UNEXPECTED;

			return text_layout->Draw(nullptr, d2d.text_renderer.Get(), 0, 0);
		}

		HRESULT GraphicsDevice::PushClip(const math::Matrix & clip_matrix, const Size & clip_size)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

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

			d2d.render_target->PopAxisAlignedClip();
			return S_OK;
		}

		HRESULT GraphicsDevice::PushLayer(cpLayer const& layer, LayerProperties const& properties)
		{
			if (!d2d.render_target ||
				!d2d.solid_brush)
				return E_UNEXPECTED;

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

			d2d.render_target->PopLayer();
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

		HRESULT GraphicsDevice::SetBrushOpacity(float opacity)
		{
			if (!d2d.render_target)
				return E_UNEXPECTED;

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

		void GraphicsDevice::DrawDebugInfo()
		{
			static int render_times_ = 0;
			static auto last_render_time_ = time::Now();

			int64_t duration = (time::Now() - last_render_time_).Milliseconds();

			if (!fps_text_format_)
			{
				ThrowIfFailed(
					CreateTextFormat(fps_text_format_, Font{ L"", 20 })
				);

				fps_text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
			}

			++render_times_;
			if (duration >= 100LL)
			{
				wchar_t fps_text[12] = {};
				int len = swprintf_s(fps_text, L"FPS: %.1f", 1000.f / static_cast<float>(duration) * render_times_);

				last_render_time_ = time::Now();
				render_times_ = 0;

				ThrowIfFailed(
					CreateTextLayout(fps_text_layout_, fps_text, fps_text_format_, 0)
				);
			}

			if (fps_text_layout_)
			{
				d2d.render_target->SetTransform(D2D1::Matrix3x2F::Identity());
				d2d.solid_brush->SetOpacity(1.0f);
				d2d.text_renderer->SetTextStyle(
					D2D1::ColorF(D2D1::ColorF::White),
					TRUE,
					D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
					1.5f,
					d2d.round_stroke_style.Get()
				);

				fps_text_layout_->Draw(nullptr, d2d.text_renderer.Get(), 10, 0);
			}
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
