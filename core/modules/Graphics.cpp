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


namespace easy2d
{
	// 文字渲染器
	class TextRenderer
		: public IDWriteTextRenderer
	{
	public:
		static HRESULT Create(
			IDWriteTextRenderer** ppTextRenderer,
			ID2D1Factory* pD2DFactory,
			ID2D1HwndRenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		STDMETHOD_(void, SetTextStyle)(
			CONST D2D1_COLOR_F &fillColor,
			BOOL outline,
			CONST D2D1_COLOR_F &outline_color,
			FLOAT outline_width,
			D2D1_LINE_JOIN outlineJoin
		);

		STDMETHOD(DrawGlyphRun)(
			__maybenull void* clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			DWRITE_MEASURING_MODE measuringMode,
			__in DWRITE_GLYPH_RUN const* glyphRun,
			__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(DrawUnderline)(
			__maybenull void* clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			__in DWRITE_UNDERLINE const* underline,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(DrawStrikethrough)(
			__maybenull void* clientDrawingContext,
			FLOAT baselineOriginX,
			FLOAT baselineOriginY,
			__in DWRITE_STRIKETHROUGH const* strikethrough,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(DrawInlineObject)(
			__maybenull void* clientDrawingContext,
			FLOAT originX,
			FLOAT originY,
			IDWriteInlineObject* inlineObject,
			BOOL IsSideways,
			BOOL IsRightToLeft,
			IUnknown* clientDrawingEffect
		);

		STDMETHOD(IsPixelSnappingDisabled)(
			__maybenull void* clientDrawingContext,
			__out BOOL* isDisabled
		);

		STDMETHOD(GetCurrentTransform)(
			__maybenull void* clientDrawingContext,
			__out DWRITE_MATRIX* transform
		);

		STDMETHOD(GetPixelsPerDip)(
			__maybenull void* clientDrawingContext,
			__out FLOAT* pixelsPerDip
		);

	public:
		unsigned long STDMETHODCALLTYPE AddRef();
		unsigned long STDMETHODCALLTYPE Release();
		HRESULT STDMETHODCALLTYPE QueryInterface(
			IID const& riid,
			void** ppvObject
		);

	private:
		TextRenderer(
			ID2D1Factory* pD2DFactory,
			ID2D1HwndRenderTarget* pRT,
			ID2D1SolidColorBrush* pBrush
		);

		~TextRenderer();

	private:
		unsigned long			cRefCount_;
		D2D1_COLOR_F			sFillColor_;
		D2D1_COLOR_F			sOutlineColor_;
		FLOAT					fOutlineWidth;
		BOOL					bShowOutline_;
		ID2D1Factory*			pD2DFactory_;
		ID2D1HwndRenderTarget*	pRT_;
		ID2D1SolidColorBrush*	pBrush_;
		ID2D1StrokeStyle*		pCurrStrokeStyle_;
	};

	TextRenderer::TextRenderer(ID2D1Factory* pD2DFactory, ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBrush)
		: cRefCount_(0)
		, pD2DFactory_(pD2DFactory)
		, pRT_(pRT)
		, pBrush_(pBrush)
		, sFillColor_()
		, sOutlineColor_()
		, fOutlineWidth(1)
		, bShowOutline_(TRUE)
		, pCurrStrokeStyle_(nullptr)
	{
		pD2DFactory->AddRef();
		pRT->AddRef();
		pBrush->AddRef();
	}

	TextRenderer::~TextRenderer()
	{
		SafeRelease(pD2DFactory_);
		SafeRelease(pRT_);
		SafeRelease(pBrush_);
	}

	HRESULT TextRenderer::Create(
		IDWriteTextRenderer** ppTextRenderer,
		ID2D1Factory* pD2DFactory,
		ID2D1HwndRenderTarget* pRT,
		ID2D1SolidColorBrush* pBrush
	)
	{
		*ppTextRenderer = new (std::nothrow) TextRenderer(pD2DFactory, pRT, pBrush);
		if (*ppTextRenderer)
		{
			(*ppTextRenderer)->AddRef();
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP_(void) TextRenderer::SetTextStyle(
		CONST D2D1_COLOR_F &fillColor,
		BOOL outline,
		CONST D2D1_COLOR_F &outline_color,
		FLOAT outline_width,
		D2D1_LINE_JOIN outlineJoin
	)
	{
		sFillColor_ = fillColor;
		bShowOutline_ = outline;
		sOutlineColor_ = outline_color;
		fOutlineWidth = 2 * outline_width;

		switch (outlineJoin)
		{
		case D2D1_LINE_JOIN_MITER:
			pCurrStrokeStyle_ = Device::GetGraphics()->GetMiterStrokeStyle();
			break;
		case D2D1_LINE_JOIN_BEVEL:
			pCurrStrokeStyle_ = Device::GetGraphics()->GetBevelStrokeStyle();
			break;
		case D2D1_LINE_JOIN_ROUND:
			pCurrStrokeStyle_ = Device::GetGraphics()->GetRoundStrokeStyle();
			break;
		default:
			pCurrStrokeStyle_ = nullptr;
			break;
		}
	}

	STDMETHODIMP TextRenderer::DrawGlyphRun(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		__in DWRITE_GLYPH_RUN const* glyphRun,
		__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
	)
	{
		HRESULT hr = S_OK;

		ID2D1PathGeometry* pPathGeometry = nullptr;
		hr = pD2DFactory_->CreatePathGeometry(
			&pPathGeometry
		);

		ID2D1GeometrySink* pSink = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = pPathGeometry->Open(
				&pSink
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = glyphRun->fontFace->GetGlyphRunOutline(
				glyphRun->fontEmSize,
				glyphRun->glyphIndices,
				glyphRun->glyphAdvances,
				glyphRun->glyphOffsets,
				glyphRun->glyphCount,
				glyphRun->isSideways,
				glyphRun->bidiLevel % 2,
				pSink
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = pSink->Close();
		}

		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = pD2DFactory_->CreateTransformedGeometry(
				pPathGeometry,
				&matrix,
				&pTransformedGeometry
			);
		}

		if (SUCCEEDED(hr) && bShowOutline_)
		{
			pBrush_->SetColor(sOutlineColor_);

			pRT_->DrawGeometry(
				pTransformedGeometry,
				pBrush_,
				fOutlineWidth,
				pCurrStrokeStyle_
			);
		}

		if (SUCCEEDED(hr))
		{
			pBrush_->SetColor(sFillColor_);

			pRT_->FillGeometry(
				pTransformedGeometry,
				pBrush_
			);
		}

		SafeRelease(pPathGeometry);
		SafeRelease(pSink);
		SafeRelease(pTransformedGeometry);

		return hr;
	}

	STDMETHODIMP TextRenderer::DrawUnderline(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
	)
	{
		HRESULT hr;

		D2D1_RECT_F rect = D2D1::RectF(
			0,
			underline->offset,
			underline->width,
			underline->offset + underline->thickness
		);

		ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
		hr = pD2DFactory_->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);

		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = pD2DFactory_->CreateTransformedGeometry(
				pRectangleGeometry,
				&matrix,
				&pTransformedGeometry
			);
		}

		if (SUCCEEDED(hr) && bShowOutline_)
		{
			pBrush_->SetColor(sOutlineColor_);

			pRT_->DrawGeometry(
				pTransformedGeometry,
				pBrush_,
				fOutlineWidth,
				pCurrStrokeStyle_
			);
		}

		if (SUCCEEDED(hr))
		{
			pBrush_->SetColor(sFillColor_);

			pRT_->FillGeometry(
				pTransformedGeometry,
				pBrush_
			);
		}

		SafeRelease(pRectangleGeometry);
		SafeRelease(pTransformedGeometry);

		return S_OK;
	}

	STDMETHODIMP TextRenderer::DrawStrikethrough(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
	)
	{
		HRESULT hr;

		D2D1_RECT_F rect = D2D1::RectF(
			0,
			strikethrough->offset,
			strikethrough->width,
			strikethrough->offset + strikethrough->thickness
		);

		ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
		hr = pD2DFactory_->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);

		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = pD2DFactory_->CreateTransformedGeometry(
				pRectangleGeometry,
				&matrix,
				&pTransformedGeometry
			);
		}

		if (SUCCEEDED(hr) && bShowOutline_)
		{
			pBrush_->SetColor(sOutlineColor_);

			pRT_->DrawGeometry(
				pTransformedGeometry,
				pBrush_,
				fOutlineWidth,
				pCurrStrokeStyle_
			);
		}

		if (SUCCEEDED(hr))
		{
			pBrush_->SetColor(sFillColor_);

			pRT_->FillGeometry(
				pTransformedGeometry,
				pBrush_
			);
		}

		SafeRelease(pRectangleGeometry);
		SafeRelease(pTransformedGeometry);

		return S_OK;
	}

	STDMETHODIMP TextRenderer::DrawInlineObject(
		__maybenull void* clientDrawingContext,
		FLOAT originX,
		FLOAT originY,
		IDWriteInlineObject* inlineObject,
		BOOL IsSideways,
		BOOL IsRightToLeft,
		IUnknown* clientDrawingEffect
	)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP_(unsigned long) TextRenderer::AddRef()
	{
		return InterlockedIncrement(&cRefCount_);
	}

	STDMETHODIMP_(unsigned long) TextRenderer::Release()
	{
		unsigned long newCount = InterlockedDecrement(&cRefCount_);

		if (newCount == 0)
		{
			delete this;
			return 0;
		}

		return newCount;
	}

	STDMETHODIMP TextRenderer::IsPixelSnappingDisabled(
		__maybenull void* clientDrawingContext,
		__out BOOL* isDisabled
	)
	{
		*isDisabled = FALSE;
		return S_OK;
	}

	STDMETHODIMP TextRenderer::GetCurrentTransform(
		__maybenull void* clientDrawingContext,
		__out DWRITE_MATRIX* transform
	)
	{
		pRT_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
		return S_OK;
	}

	STDMETHODIMP TextRenderer::GetPixelsPerDip(
		__maybenull void* clientDrawingContext,
		__out FLOAT* pixelsPerDip
	)
	{
		float x, yUnused;

		pRT_->GetDpi(&x, &yUnused);
		*pixelsPerDip = x / 96;

		return S_OK;
	}

	STDMETHODIMP TextRenderer::QueryInterface(
		IID const& riid,
		void** ppvObject
	)
	{
		if (__uuidof(IDWriteTextRenderer) == riid)
		{
			*ppvObject = this;
		}
		else if (__uuidof(IDWritePixelSnapping) == riid)
		{
			*ppvObject = this;
		}
		else if (__uuidof(IUnknown) == riid)
		{
			*ppvObject = this;
		}
		else
		{
			*ppvObject = nullptr;
			return E_FAIL;
		}

		AddRef();

		return S_OK;
	}
}


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
		TextRenderer::Create(
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
		wchar_t fps_text[12] = {};
		int len = swprintf_s(fps_text, L"FPS: %.1f", 1000.f / duration * render_times_);

		last_render_time_ = Time::Now();
		render_times_ = 0;

		SafeRelease(fps_text_layout_);

		ThrowIfFailed(
			write_factory_->CreateTextLayout(
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
		render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
		solid_brush_->SetOpacity(1.0f);
		static_cast<TextRenderer*>(text_renderer_)->SetTextStyle(
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

IDWriteTextRenderer* easy2d::Graphics::GetTextRender() const
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

void easy2d::Graphics::SetTextRendererStyle(const Color & fill_color, bool has_outline, const Color & outline_color, float outline_width, Stroke outline_stroke)
{
	static_cast<TextRenderer*>(text_renderer_)->SetTextStyle(
		D2D1_COLOR_F(fill_color),
		has_outline,
		D2D1_COLOR_F(outline_color),
		outline_width,
		D2D1_LINE_JOIN(outline_stroke)
	);
}

float easy2d::Graphics::GetDpi()
{
	static float dpi = -1;
	if (dpi < 0)
	{
		HDC hdc = ::GetDC(0);
		dpi = static_cast<float>(::GetDeviceCaps(hdc, LOGPIXELSX));
		::ReleaseDC(0, hdc);
	}
	return dpi;
}
