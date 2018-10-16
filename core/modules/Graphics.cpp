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

#include "..\e2dmodule.h"
#include "..\e2dobject.h"


easy2d::Graphics::Graphics(HWND hwnd)
	: factory_(nullptr)
	, imaging_factory_(nullptr)
	, write_factory_(nullptr)
	, miter_stroke_style_(nullptr)
	, bevel_stroke_style_(nullptr)
	, round_stroke_style_(nullptr)
	, fps_text_format_(nullptr)
	, fps_text_layout_(nullptr)
	, render_target_(nullptr)
	, solid_brush_(nullptr)
	, text_renderer_(nullptr)
	, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
{
	ThrowIfFailed(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&factory_
		)
	);

	ThrowIfFailed(
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&imaging_factory_)
		)
	);

	ThrowIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&write_factory_)
		)
	);

	RECT rc;
	::GetClientRect(hwnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	// 创建设备相关资源。这些资源应在 Direct2D 设备消失时重建
	// 创建一个 Direct2D 渲染目标
	ThrowIfFailed(
		factory_->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hwnd,
				size,
				D2D1_PRESENT_OPTIONS_NONE),
			&render_target_
		)
	);

	// 创建画刷
	ThrowIfFailed(
		render_target_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&solid_brush_
		)
	);

	// 创建自定义的文字渲染器
	ThrowIfFailed(
		E2DTextRenderer::Create(
			&text_renderer_,
			factory_,
			render_target_,
			solid_brush_
		)
	);
}

easy2d::Graphics::~Graphics()
{
	SafeRelease(fps_text_format_);
	SafeRelease(fps_text_layout_);
	SafeRelease(text_renderer_);
	SafeRelease(solid_brush_);
	SafeRelease(render_target_);

	SafeRelease(miter_stroke_style_);
	SafeRelease(bevel_stroke_style_);
	SafeRelease(round_stroke_style_);
	SafeRelease(factory_);
	SafeRelease(imaging_factory_);
	SafeRelease(write_factory_);
}

void easy2d::Graphics::BeginDraw()
{
	render_target_->BeginDraw();
	render_target_->Clear(clear_color_);
}

void easy2d::Graphics::EndDraw()
{
	HRESULT hr = render_target_->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
		// 并在下一次调用时重建资源
		hr = S_OK;

		SafeRelease(fps_text_format_);
		SafeRelease(fps_text_layout_);
		SafeRelease(text_renderer_);
		SafeRelease(solid_brush_);
		SafeRelease(render_target_);
	}

	ThrowIfFailed(hr);
}

void easy2d::Graphics::DrawDebugInfo()
{
	static int render_times_ = 0;
	static Time last_render_time_ = Time::Now();
	int duration = (Time::Now() - last_render_time_).Milliseconds();

	if (!fps_text_format_)
	{
		ThrowIfFailed(
			write_factory_->CreateTextFormat(
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
	if (duration >= 100)
	{
		String fps_text = String::Format(L"FPS: %.1f", (1000.f / duration * render_times_));
		last_render_time_ = Time::Now();
		render_times_ = 0;

		SafeRelease(fps_text_layout_);

		ThrowIfFailed(
			write_factory_->CreateTextLayout(
			(const wchar_t *)fps_text,
				(UINT32)fps_text.Length(),
				fps_text_format_,
				0,
				0,
				&fps_text_layout_
			)
		);
	}

	if (fps_text_layout_)
	{
		render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
		solid_brush_->SetOpacity(1.0f);
		text_renderer_->SetTextStyle(
			D2D1::ColorF(D2D1::ColorF::White),
			TRUE,
			D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
			1.5f,
			D2D1_LINE_JOIN_ROUND
		);

		fps_text_layout_->Draw(
			nullptr,
			text_renderer_,
			10,
			0
		);
	}
}

ID2D1HwndRenderTarget * easy2d::Graphics::GetRenderTarget() const
{
	return render_target_;
}

ID2D1SolidColorBrush * easy2d::Graphics::GetSolidBrush() const
{
	return solid_brush_;
}

easy2d::E2DTextRenderer * easy2d::Graphics::GetTextRender() const
{
	return text_renderer_;
}

ID2D1Factory * easy2d::Graphics::GetFactory() const
{
	return factory_;
}

IWICImagingFactory * easy2d::Graphics::GetImagingFactory() const
{
	return imaging_factory_;
}

IDWriteFactory * easy2d::Graphics::GetWriteFactory() const
{
	return write_factory_;
}

ID2D1StrokeStyle * easy2d::Graphics::GetMiterStrokeStyle()
{
	if (!miter_stroke_style_)
	{
		ThrowIfFailed(
			factory_->CreateStrokeStyle(
				D2D1::StrokeStyleProperties(
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_LINE_JOIN_MITER,
					2.0f,
					D2D1_DASH_STYLE_SOLID,
					0.0f),
				nullptr,
				0,
				&miter_stroke_style_
			)
		);
	}
	return miter_stroke_style_;
}

ID2D1StrokeStyle * easy2d::Graphics::GetBevelStrokeStyle()
{
	if (!bevel_stroke_style_)
	{
		ThrowIfFailed(
			factory_->CreateStrokeStyle(
				D2D1::StrokeStyleProperties(
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_LINE_JOIN_BEVEL,
					2.0f,
					D2D1_DASH_STYLE_SOLID,
					0.0f),
				nullptr,
				0,
				&bevel_stroke_style_
			)
		);
	}
	return bevel_stroke_style_;
}

ID2D1StrokeStyle * easy2d::Graphics::GetRoundStrokeStyle()
{
	if (!round_stroke_style_)
	{
		ThrowIfFailed(
			factory_->CreateStrokeStyle(
				D2D1::StrokeStyleProperties(
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_LINE_JOIN_ROUND,
					2.0f,
					D2D1_DASH_STYLE_SOLID,
					0.0f),
				nullptr,
				0,
				&round_stroke_style_
			)
		);
	}
	return round_stroke_style_;
}

float easy2d::Graphics::GetDpi()
{
	HDC hdc = ::GetDC(0);
	int dpi = ::GetDeviceCaps(hdc, LOGPIXELSX);
	::ReleaseDC(0, hdc);
	return static_cast<float>(dpi);
}
