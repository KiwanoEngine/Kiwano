#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"


e2d::Renderer*		e2d::Renderer::instance_ = nullptr;
ID2D1Factory*		e2d::Renderer::factory_ = nullptr;
IWICImagingFactory*	e2d::Renderer::imaging_factory_ = nullptr;
IDWriteFactory*		e2d::Renderer::write_factory_ = nullptr;
ID2D1StrokeStyle*	e2d::Renderer::miter_stroke_style_ = nullptr;
ID2D1StrokeStyle*	e2d::Renderer::bevel_stroke_style_ = nullptr;
ID2D1StrokeStyle*	e2d::Renderer::round_stroke_style_ = nullptr;

e2d::Renderer * e2d::Renderer::GetInstance()
{
	if (!instance_)
	{
		instance_ = new (std::nothrow) Renderer;
	}
	return instance_;
}

void e2d::Renderer::DestroyInstance()
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;

		SafeRelease(miter_stroke_style_);
		SafeRelease(bevel_stroke_style_);
		SafeRelease(round_stroke_style_);
		SafeRelease(factory_);
		SafeRelease(imaging_factory_);
		SafeRelease(write_factory_);
	}
}

e2d::Renderer::Renderer()
	: show_fps_(false)
	, last_render_time_(Time::Now())
	, render_times_(0)
	, fps_text_format_(nullptr)
	, fps_text_layout_(nullptr)
	, render_target_(nullptr)
	, solid_brush_(nullptr)
	, text_renderer_(nullptr)
	, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
{
	::CoInitialize(nullptr);
}

e2d::Renderer::~Renderer()
{
	SafeRelease(fps_text_format_);
	SafeRelease(fps_text_layout_);
	SafeRelease(text_renderer_);
	SafeRelease(solid_brush_);
	SafeRelease(render_target_);

	::CoUninitialize();
}

void e2d::Renderer::BeginDraw()
{
	auto render_target = GetRenderTarget();
	render_target->BeginDraw();
	// 使用背景色清空屏幕
	render_target->Clear(clear_color_);
}

void e2d::Renderer::EndDraw()
{
	if (show_fps_)
	{
		int duration = (Time::Now() - last_render_time_).Milliseconds();

		++render_times_;
		if (duration >= 100)
		{
			String fpsText = String::Format(L"FPS: %.1f", (1000.f / duration * render_times_));
			last_render_time_ = Time::Now();
			render_times_ = 0;

			if (!fps_text_format_)
			{
				ThrowIfFailed(
					GetWriteFactory()->CreateTextFormat(
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
					fps_text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP)
				);
			}

			SafeRelease(fps_text_layout_);

			ThrowIfFailed(
				GetWriteFactory()->CreateTextLayout(
				(const wchar_t *)fpsText,
					(UINT32)fpsText.GetLength(),
					fps_text_format_,
					0,
					0,
					&fps_text_layout_
				)
			);
		}

		if (fps_text_layout_)
		{
			GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
			GetSolidBrush()->SetOpacity(1.0f);
			GetTextRenderer()->SetTextStyle(
				D2D1::ColorF(D2D1::ColorF::White),
				TRUE,
				D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
				1.5f,
				D2D1_LINE_JOIN_ROUND
			);

			ThrowIfFailed(
				fps_text_layout_->Draw(nullptr, text_renderer_, 10, 0)
			);
		}
	}

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

e2d::E2DTextRenderer * e2d::Renderer::GetTextRenderer()
{
	if (!text_renderer_)
	{
		// 创建自定义的文字渲染器
		ThrowIfFailed(
			E2DTextRenderer::Create(
				&text_renderer_,
				GetFactory(),
				GetRenderTarget(),
				GetSolidBrush()
			)
		);
	}
	return text_renderer_;
}

ID2D1HwndRenderTarget * e2d::Renderer::GetRenderTarget()
{
	if (!render_target_)
	{
		HWND hWnd = Window::GetInstance()->GetHWnd();

		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// 创建设备相关资源。这些资源应在 Direct2D 设备消失时重建
		// 创建一个 Direct2D 渲染目标
		ThrowIfFailed(
			GetFactory()->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(
					hWnd,
					size,
					D2D1_PRESENT_OPTIONS_NONE),
				&render_target_
			)
		);
	}
	return render_target_;
}

ID2D1SolidColorBrush * e2d::Renderer::GetSolidBrush()
{
	if (!solid_brush_)
	{
		ThrowIfFailed(
			GetRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&solid_brush_
			)
		);
	}
	return solid_brush_;
}

e2d::Color e2d::Renderer::GetBackgroundColor()
{
	return clear_color_;
}

void e2d::Renderer::SetBackgroundColor(const Color& color)
{
	clear_color_ = (D2D1_COLOR_F)color;
}

void e2d::Renderer::ShowFps(bool show)
{
	show_fps_ = show;
}

ID2D1Factory * e2d::Renderer::GetFactory()
{
	if (!factory_)
	{
		::CoInitialize(nullptr);

		ThrowIfFailed(
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				&factory_
			)
		);

		::CoUninitialize();
	}
	return factory_;
}

IWICImagingFactory * e2d::Renderer::GetImagingFactory()
{
	if (!imaging_factory_)
	{
		::CoInitialize(nullptr);

		ThrowIfFailed(
			CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				reinterpret_cast<void**>(&imaging_factory_)
			)
		);

		::CoUninitialize();
	}
	return imaging_factory_;
}

IDWriteFactory * e2d::Renderer::GetWriteFactory()
{
	if (!write_factory_)
	{
		::CoInitialize(nullptr);

		ThrowIfFailed(
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&write_factory_)
			)
		);

		::CoUninitialize();
	}
	return write_factory_;
}

ID2D1StrokeStyle * e2d::Renderer::GetMiterStrokeStyle()
{
	if (!miter_stroke_style_)
	{
		ThrowIfFailed(
			GetFactory()->CreateStrokeStyle(
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

ID2D1StrokeStyle * e2d::Renderer::GetBevelStrokeStyle()
{
	if (!bevel_stroke_style_)
	{
		ThrowIfFailed(
			GetFactory()->CreateStrokeStyle(
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

ID2D1StrokeStyle * e2d::Renderer::GetRoundStrokeStyle()
{
	if (!round_stroke_style_)
	{
		ThrowIfFailed(
			GetFactory()->CreateStrokeStyle(
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
