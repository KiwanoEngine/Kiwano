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
#include "base.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace easy2d
{
	namespace render
	{
		GraphicsDevice instance;

		GraphicsDevice::GraphicsDevice()
			: fps_text_format_(nullptr)
			, fps_text_layout_(nullptr)
			, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
		{
			ZeroMemory(&d2d, sizeof(D2DResources));
		}

		GraphicsDevice::~GraphicsDevice()
		{
			SafeRelease(fps_text_format_);
			SafeRelease(fps_text_layout_);
		}

		void GraphicsDevice::Initialize(HWND hwnd)
		{
			if (d2d.Factory)
				return;

			ThrowIfFailed(
				D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED,
					&d2d.Factory
				)
			);

			ThrowIfFailed(
				CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					reinterpret_cast<void**>(&d2d.WICImagingFactory)
				)
			);

			ThrowIfFailed(
				DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory),
					reinterpret_cast<IUnknown**>(&d2d.DWriteFactory)
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
				d2d.Factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d.MiterStrokeStyle
				)
			);

			stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;

			ThrowIfFailed(
				d2d.Factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d.BevelStrokeStyle
				)
			);

			stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;

			ThrowIfFailed(
				d2d.Factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&d2d.RoundStrokeStyle
				)
			);

			CreateDeviceResources(hwnd);
		}

		void GraphicsDevice::Uninitialize()
		{
			SafeRelease(d2d.TextRenderer);
			SafeRelease(d2d.SolidColorBrush);
			SafeRelease(d2d.HwndRenderTarget);

			SafeRelease(d2d.MiterStrokeStyle);
			SafeRelease(d2d.BevelStrokeStyle);
			SafeRelease(d2d.RoundStrokeStyle);

			SafeRelease(d2d.WICImagingFactory);
			SafeRelease(d2d.DWriteFactory);
			SafeRelease(d2d.Factory);
		}

		void GraphicsDevice::BeginDraw(HWND hwnd)
		{
			CreateDeviceResources(hwnd);

			d2d.HwndRenderTarget->BeginDraw();
			d2d.HwndRenderTarget->Clear(clear_color_);
		}

		void GraphicsDevice::EndDraw()
		{
			HRESULT hr = d2d.HwndRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
				// 并在下一次调用时重建资源
				hr = S_OK;

				SafeRelease(fps_text_format_);
				SafeRelease(fps_text_layout_);
				SafeRelease(d2d.TextRenderer);
				SafeRelease(d2d.SolidColorBrush);
				SafeRelease(d2d.HwndRenderTarget);
			}

			ThrowIfFailed(hr);
		}

		HRESULT GraphicsDevice::CreateRectGeometry(
			const math::Matrix& matrix,
			const Size& size,
			ID2D1Geometry** geometry
		) const
		{
			if (!d2d.Factory)
				return E_UNEXPECTED;

			HRESULT hr;

			ID2D1RectangleGeometry * rectangle = nullptr;
			ID2D1TransformedGeometry * transformed = nullptr;
			
			hr = d2d.Factory->CreateRectangleGeometry(
				D2D1::RectF(0, 0, size.width, size.height),
				&rectangle
			);

			if (SUCCEEDED(hr))
			{
				hr = d2d.Factory->CreateTransformedGeometry(
					rectangle,
					matrix,
					&transformed
				);
			}

			if (SUCCEEDED(hr))
			{
				*geometry = transformed;
			}

			SafeRelease(rectangle);
			return hr;
		}

		HRESULT GraphicsDevice::CreateTextFormat(IDWriteTextFormat ** text_format, const Font & font) const
		{
			if (!d2d.DWriteFactory)
				return E_UNEXPECTED;

			return d2d.DWriteFactory->CreateTextFormat(
				font.family.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT(font.weight),
				font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				font.size,
				L"",
				text_format
			);
		}

		HRESULT GraphicsDevice::CreateTextLayout(IDWriteTextLayout ** text_layout, const String & text, IDWriteTextFormat * text_format, float wrap_width) const
		{
			if (!d2d.DWriteFactory)
				return E_UNEXPECTED;

			UINT32 length = static_cast<UINT32>(text.length());
			return d2d.DWriteFactory->CreateTextLayout(
				text.c_str(),
				length,
				text_format,
				wrap_width,
				0,
				text_layout
			);
		}

		HRESULT GraphicsDevice::CreateLayer(ID2D1Layer ** layer)
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			return d2d.HwndRenderTarget->CreateLayer(layer);
		}

		HRESULT GraphicsDevice::DrawGeometry(
			ID2D1Geometry * geometry,
			const Color & border_color,
			float opacity,
			float stroke_width,
			StrokeStyle stroke
		)
		{
			if (!d2d.SolidColorBrush ||
				!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.SolidColorBrush->SetColor(border_color);
			d2d.SolidColorBrush->SetOpacity(opacity);
			d2d.HwndRenderTarget->DrawGeometry(
				geometry,
				d2d.SolidColorBrush,
				stroke_width,
				GetStrokeStyle(stroke)
			);
			return S_OK;
		}

		ID2D1StrokeStyle * GraphicsDevice::GetStrokeStyle(StrokeStyle stroke) const
		{
			ID2D1StrokeStyle * stroke_style = nullptr;
			switch (stroke)
			{
			case StrokeStyle::Miter:
				stroke_style = d2d.MiterStrokeStyle;
				break;
			case StrokeStyle::Bevel:
				stroke_style = d2d.BevelStrokeStyle;
				break;
			case StrokeStyle::Round:
				stroke_style = d2d.RoundStrokeStyle;
				break;
			}
			return stroke_style;
		}

		HRESULT GraphicsDevice::DrawImage(
			Image * image,
			float opacity,
			const Rect & dest_rect,
			const Rect & source_rect
		)
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->DrawBitmap(
				image->GetBitmap(),
				dest_rect,
				opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				source_rect
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawTextLayout(IDWriteTextLayout * text_layout)
		{
			if (!d2d.TextRenderer)
				return E_UNEXPECTED;

			return text_layout->Draw(nullptr, d2d.TextRenderer, 0, 0);
		}

		HRESULT GraphicsDevice::PushClip(const math::Matrix & clip_matrix, const Size & clip_size)
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->SetTransform(clip_matrix);
			d2d.HwndRenderTarget->PushAxisAlignedClip(
				D2D1::RectF(0, 0, clip_size.width, clip_size.height),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::PopClip()
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->PopAxisAlignedClip();
			return S_OK;
		}

		HRESULT GraphicsDevice::PushLayer(ID2D1Layer * layer, LayerProperties properties)
		{
			if (!d2d.HwndRenderTarget ||
				!d2d.SolidColorBrush)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->PushLayer(
				D2D1::LayerParameters(
					properties.area,
					nullptr,
					D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
					D2D1::Matrix3x2F::Identity(),
					properties.opacity,
					d2d.SolidColorBrush,
					D2D1_LAYER_OPTIONS_NONE
				),
				layer
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::PopLayer()
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->PopLayer();
			return S_OK;
		}

		HRESULT GraphicsDevice::CreateBitmapFromFile(const String & file_path, ID2D1Bitmap ** bitmap)
		{
			if (d2d.WICImagingFactory == nullptr ||
				d2d.HwndRenderTarget == nullptr)
			{
				return E_UNEXPECTED;
			}

			if (bitmap == nullptr)
			{
				return E_POINTER;
			}

			IWICBitmapDecoder*		decoder = nullptr;
			IWICBitmapFrameDecode*	source = nullptr;
			IWICStream*				stream = nullptr;
			IWICFormatConverter*	converter = nullptr;

			// 创建解码器
			HRESULT hr = d2d.WICImagingFactory->CreateDecoderFromFilename(
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
				hr = d2d.WICImagingFactory->CreateFormatConverter(&converter);
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
				hr = d2d.HwndRenderTarget->CreateBitmapFromWicBitmap(
					converter,
					nullptr,
					bitmap
				);
			}

			// 释放相关资源
			SafeRelease(decoder);
			SafeRelease(source);
			SafeRelease(stream);
			SafeRelease(converter);

			return hr;
		}

		HRESULT GraphicsDevice::CreateBitmapFromResource(Resource & res, ID2D1Bitmap ** bitmap)
		{
			if (d2d.WICImagingFactory == nullptr ||
				d2d.HwndRenderTarget == nullptr)
			{
				return E_UNEXPECTED;
			}

			if (bitmap == nullptr)
			{
				return E_POINTER;
			}

			HRESULT hr;

			HINSTANCE				hinstance = GetModuleHandle(nullptr);
			IWICBitmapDecoder*		decoder = nullptr;
			IWICBitmapFrameDecode*	source = nullptr;
			IWICStream*				stream = nullptr;
			IWICFormatConverter*	converter = nullptr;
			
			// 加载资源
			hr = res.Load() ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				// 创建 WIC 流
				hr = d2d.WICImagingFactory->CreateStream(&stream);
			}

			if (SUCCEEDED(hr))
			{
				// 初始化流
				hr = stream->InitializeFromMemory(
					static_cast<WICInProcPointer>(res.GetData()),
					res.GetDataSize()
				);
			}

			if (SUCCEEDED(hr))
			{
				// 创建流的解码器
				hr = d2d.WICImagingFactory->CreateDecoderFromStream(
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
				hr = d2d.WICImagingFactory->CreateFormatConverter(&converter);
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
				hr = d2d.HwndRenderTarget->CreateBitmapFromWicBitmap(
					converter,
					nullptr,
					bitmap
				);
			}

			// 释放相关资源
			SafeRelease(decoder);
			SafeRelease(source);
			SafeRelease(stream);
			SafeRelease(converter);

			return hr;
		}

		HRESULT GraphicsDevice::Resize(UINT32 width, UINT32 height)
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->Resize(D2D1::SizeU(width, height));
			return S_OK;
		}

		HRESULT GraphicsDevice::SetTransform(const math::Matrix & matrix)
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.HwndRenderTarget->SetTransform(matrix);
			return S_OK;
		}

		HRESULT GraphicsDevice::SetBrushOpacity(float opacity)
		{
			if (!d2d.HwndRenderTarget)
				return E_UNEXPECTED;

			d2d.SolidColorBrush->SetOpacity(opacity);
			return S_OK;
		}

		HRESULT GraphicsDevice::SetTextStyle(
			const Color & color,
			bool has_outline,
			const Color & outline_color,
			float outline_width,
			StrokeStyle outline_stroke
		)
		{
			if (!d2d.TextRenderer)
				return E_UNEXPECTED;

			d2d.TextRenderer->SetTextStyle(
				color,
				has_outline,
				outline_color,
				outline_width,
				static_cast<D2D1_LINE_JOIN>(outline_stroke)
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
					d2d.DWriteFactory->CreateTextFormat(
						L"",
						nullptr,
						DWRITE_FONT_WEIGHT_NORMAL,
						DWRITE_FONT_STYLE_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						20,
						L"",
						&fps_text_format_
					)
				);

				ThrowIfFailed(
					fps_text_format_->SetWordWrapping(
						DWRITE_WORD_WRAPPING_NO_WRAP
					)
				);
			}

			++render_times_;
			if (duration >= 100LL)
			{
				wchar_t fps_text[12] = {};
				int len = swprintf_s(fps_text, L"FPS: %.1f", 1000.f / static_cast<float>(duration) * render_times_);

				last_render_time_ = time::Now();
				render_times_ = 0;

				SafeRelease(fps_text_layout_);

				ThrowIfFailed(
					d2d.DWriteFactory->CreateTextLayout(
						fps_text,
						static_cast<UINT32>(len),
						fps_text_format_,
						0,
						0,
						&fps_text_layout_
					)
				);
			}

			if (fps_text_layout_)
			{
				d2d.HwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				d2d.SolidColorBrush->SetOpacity(1.0f);
				d2d.TextRenderer->SetTextStyle(
					D2D1::ColorF(D2D1::ColorF::White),
					TRUE,
					D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
					1.5f,
					D2D1_LINE_JOIN_ROUND
				);

				fps_text_layout_->Draw(nullptr, d2d.TextRenderer, 10, 0);
			}
		}

		void GraphicsDevice::CreateDeviceResources(HWND hwnd)
		{
			if (!d2d.HwndRenderTarget)
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
					d2d.Factory->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(
							hwnd,
							size,
							D2D1_PRESENT_OPTIONS_NONE),
						&d2d.HwndRenderTarget
					)
				);
			}

			if (!d2d.SolidColorBrush)
			{
				ThrowIfFailed(
					d2d.HwndRenderTarget->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::White),
						&d2d.SolidColorBrush
					)
				);
			}

			if (!d2d.TextRenderer)
			{
				ThrowIfFailed(
					ITextRenderer::Create(
						&d2d.TextRenderer,
						d2d.Factory,
						d2d.HwndRenderTarget,
						d2d.SolidColorBrush
					)
				);
			}
		}
	}
}
