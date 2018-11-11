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
		_D2D_Resource D2D = { 0 };

		void easy2d::render::Initialize(HWND hwnd)
		{
			if (D2D.Factory)
				return;

			ThrowIfFailed(
				D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED,
					&D2D.Factory
				)
			);

			ThrowIfFailed(
				CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					reinterpret_cast<void**>(&D2D.WICImagingFactory)
				)
			);

			ThrowIfFailed(
				DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory),
					reinterpret_cast<IUnknown**>(&D2D.DWriteFactory)
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
				D2D.Factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&D2D.MiterStrokeStyle
				)
			);

			stroke_style.lineJoin = D2D1_LINE_JOIN_BEVEL;

			ThrowIfFailed(
				D2D.Factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&D2D.BevelStrokeStyle
				)
			);

			stroke_style.lineJoin = D2D1_LINE_JOIN_ROUND;

			ThrowIfFailed(
				D2D.Factory->CreateStrokeStyle(
					stroke_style,
					nullptr,
					0,
					&D2D.RoundStrokeStyle
				)
			);

			CreateDeviceResources(hwnd);
		}

		void easy2d::render::CreateDeviceResources(HWND hwnd)
		{
			if (!D2D.HwndRenderTarget)
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
					D2D.Factory->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(
							hwnd,
							size,
							D2D1_PRESENT_OPTIONS_NONE),
						&D2D.HwndRenderTarget
					)
				);
			}

			if (!D2D.SolidColorBrush)
			{
				ThrowIfFailed(
					D2D.HwndRenderTarget->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::White),
						&D2D.SolidColorBrush
					)
				);
			}

			if (!D2D.TextRenderer)
			{
				ThrowIfFailed(
					ITextRenderer::Create(
						&D2D.TextRenderer,
						D2D.Factory,
						D2D.HwndRenderTarget,
						D2D.SolidColorBrush
					)
				);
			}
		}

		void easy2d::render::Uninitialize()
		{
			SafeRelease(D2D.TextRenderer);
			SafeRelease(D2D.SolidColorBrush);
			SafeRelease(D2D.HwndRenderTarget);

			SafeRelease(D2D.MiterStrokeStyle);
			SafeRelease(D2D.BevelStrokeStyle);
			SafeRelease(D2D.RoundStrokeStyle);

			SafeRelease(D2D.WICImagingFactory);
			SafeRelease(D2D.DWriteFactory);
			SafeRelease(D2D.Factory);
		}

		//-------------------------------------------------------
		// GraphicsDevice
		//-------------------------------------------------------

		GraphicsDevice instance;

		GraphicsDevice::GraphicsDevice()
			: fps_text_format_(nullptr)
			, fps_text_layout_(nullptr)
			, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
		{
		}

		GraphicsDevice::~GraphicsDevice()
		{
			SafeRelease(fps_text_format_);
			SafeRelease(fps_text_layout_);
		}

		void GraphicsDevice::BeginDraw(HWND hwnd)
		{
			render::CreateDeviceResources(hwnd);
			render::D2D.HwndRenderTarget->BeginDraw();
			render::D2D.HwndRenderTarget->Clear(clear_color_);
		}

		void GraphicsDevice::EndDraw()
		{
			HRESULT hr = render::D2D.HwndRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
				// 并在下一次调用时重建资源
				hr = S_OK;

				SafeRelease(fps_text_format_);
				SafeRelease(fps_text_layout_);
				SafeRelease(render::D2D.TextRenderer);
				SafeRelease(render::D2D.SolidColorBrush);
				SafeRelease(render::D2D.HwndRenderTarget);
			}

			ThrowIfFailed(hr);
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
					render::D2D.DWriteFactory->CreateTextFormat(
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
					render::D2D.DWriteFactory->CreateTextLayout(
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
				render::D2D.HwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				render::D2D.SolidColorBrush->SetOpacity(1.0f);
				render::D2D.TextRenderer->SetTextStyle(
					D2D1::ColorF(D2D1::ColorF::White),
					TRUE,
					D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
					1.5f,
					D2D1_LINE_JOIN_ROUND
				);

				fps_text_layout_->Draw(nullptr, render::D2D.TextRenderer, 10, 0);
			}
		}
	}
}
