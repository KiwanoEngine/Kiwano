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

#include <kiwano/renderer/win32/TextRenderer.h>

namespace kiwano
{
	class TextRenderer
		: public ITextRenderer
	{
	public:
		TextRenderer(
			ID2D1RenderTarget* pRT
		);

		~TextRenderer();

		STDMETHOD(CreateDeviceResources)();

		STDMETHOD_(void, SetTextStyle)(
			_In_ float opacity,
			_In_ CONST D2D1_COLOR_F &fillColor,
			_In_ BOOL outline,
			_In_ CONST D2D1_COLOR_F &outlineColor,
			_In_ float outlineWidth,
			_In_ ID2D1StrokeStyle* outlineJoin
			);

		STDMETHOD(DrawGlyphRun)(
			__maybenull void* clientDrawingContext,
			float baselineOriginX,
			float baselineOriginY,
			DWRITE_MEASURING_MODE measuringMode,
			__in DWRITE_GLYPH_RUN const* glyphRun,
			__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
			IUnknown* clientDrawingEffect
			);

		STDMETHOD(DrawUnderline)(
			__maybenull void* clientDrawingContext,
			float baselineOriginX,
			float baselineOriginY,
			__in DWRITE_UNDERLINE const* underline,
			IUnknown* clientDrawingEffect
			);

		STDMETHOD(DrawStrikethrough)(
			__maybenull void* clientDrawingContext,
			float baselineOriginX,
			float baselineOriginY,
			__in DWRITE_STRIKETHROUGH const* strikethrough,
			IUnknown* clientDrawingEffect
			);

		STDMETHOD(DrawInlineObject)(
			__maybenull void* clientDrawingContext,
			float originX,
			float originY,
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
			__out float* pixelsPerDip
			);

	public:
		unsigned long STDMETHODCALLTYPE AddRef();
		unsigned long STDMETHODCALLTYPE Release();
		HRESULT STDMETHODCALLTYPE QueryInterface(
			REFIID riid,
			void** ppvObject
		);

	private:
		unsigned long			cRefCount_;
		D2D1_COLOR_F			sFillColor_;
		D2D1_COLOR_F			sOutlineColor_;
		float					fOutlineWidth;
		BOOL					bShowOutline_;
		ID2D1Factory*			pFactory_;
		ID2D1RenderTarget*		pRT_;
		ID2D1SolidColorBrush*	pBrush_;
		ID2D1StrokeStyle*		pCurrStrokeStyle_;
	};

	HRESULT ITextRenderer::Create(
        _Out_ ITextRenderer** ppTextRenderer,
		_In_ ID2D1RenderTarget* pRT)
	{
		HRESULT hr = E_FAIL;

		if (ppTextRenderer)
		{
			TextRenderer* pTextRenderer = new (std::nothrow) TextRenderer(pRT);
			if (pTextRenderer)
			{
				hr = pTextRenderer->CreateDeviceResources();

				if (SUCCEEDED(hr))
				{
					pTextRenderer->AddRef();

					DX::SafeRelease(*ppTextRenderer);
					(*ppTextRenderer) = pTextRenderer;
					return S_OK;
				}
				else
				{
					delete pTextRenderer;
					pTextRenderer = NULL;
				}
			}
		}
		return hr;
	}

	TextRenderer::TextRenderer(ID2D1RenderTarget* pRT)
		: cRefCount_(0)
		, pFactory_(NULL)
		, pRT_(pRT)
		, pBrush_(NULL)
		, sFillColor_()
		, sOutlineColor_()
		, fOutlineWidth(1)
		, bShowOutline_(TRUE)
		, pCurrStrokeStyle_(NULL)
	{
		if (pRT_)
		{
			pRT_->AddRef();
			pRT_->GetFactory(&pFactory_);
		}
	}

	TextRenderer::~TextRenderer()
	{
		DX::SafeRelease(pFactory_);
		DX::SafeRelease(pRT_);
		DX::SafeRelease(pBrush_);
	}

	STDMETHODIMP TextRenderer::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		DX::SafeRelease(pBrush_);

		if (pRT_)
		{
			hr = pRT_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&pBrush_
			);
		}

		return hr;
	}

	STDMETHODIMP_(void) TextRenderer::SetTextStyle(
        _In_ float opacity,
        _In_ CONST D2D1_COLOR_F &fillColor,
        _In_ BOOL outline,
        _In_ CONST D2D1_COLOR_F &outlineColor,
        _In_ float outlineWidth,
        _In_ ID2D1StrokeStyle* outlineJoin)
	{
		sFillColor_ = fillColor;
		bShowOutline_ = outline;
		sOutlineColor_ = outlineColor;
		fOutlineWidth = outlineWidth;
		pCurrStrokeStyle_ = outlineJoin;

		if (pBrush_) pBrush_->SetOpacity(opacity);
	}

	STDMETHODIMP TextRenderer::DrawGlyphRun(
		__maybenull void* clientDrawingContext,
		float baselineOriginX,
		float baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		__in DWRITE_GLYPH_RUN const* glyphRun,
		__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect)
	{
		KGE_UNUSED(clientDrawingContext);
		KGE_UNUSED(measuringMode);
		KGE_UNUSED(glyphRunDescription);
		KGE_UNUSED(clientDrawingEffect);

		HRESULT hr = S_OK;

		if (bShowOutline_)
		{
			ID2D1GeometrySink* pSink = NULL;
			ID2D1PathGeometry* pPathGeometry = NULL;
			ID2D1TransformedGeometry* pTransformedGeometry = NULL;

			hr = pFactory_->CreatePathGeometry(
				&pPathGeometry
			);

			if (SUCCEEDED(hr))
			{
				hr = pPathGeometry->Open(
					&pSink
				);

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

				if (SUCCEEDED(hr))
				{
					D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
						1.0f, 0.0f,
						0.0f, 1.0f,
						baselineOriginX, baselineOriginY
					);

					if (SUCCEEDED(hr))
					{
						hr = pFactory_->CreateTransformedGeometry(
							pPathGeometry,
							&matrix,
							&pTransformedGeometry
						);
					}

					if (SUCCEEDED(hr))
					{
						pBrush_->SetColor(sOutlineColor_);

						pRT_->DrawGeometry(
							pTransformedGeometry,
							pBrush_,
							fOutlineWidth * 2,  // twice width for widening
							pCurrStrokeStyle_
						);
					}
				}
			}

			DX::SafeRelease(pPathGeometry);
			DX::SafeRelease(pSink);
			DX::SafeRelease(pTransformedGeometry);
		}

		if (SUCCEEDED(hr))
		{
			pBrush_->SetColor(sFillColor_);

			pRT_->DrawGlyphRun(
				D2D1::Point2F(baselineOriginX, baselineOriginY),
				glyphRun,
				pBrush_
			);
		}
		return hr;
	}

	STDMETHODIMP TextRenderer::DrawUnderline(
		__maybenull void* clientDrawingContext,
		float baselineOriginX,
		float baselineOriginY,
		__in DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect)
	{
		KGE_UNUSED(clientDrawingContext);
		KGE_UNUSED(clientDrawingEffect);

		HRESULT hr;

		D2D1_RECT_F rect = D2D1::RectF(
			0,
			underline->offset,
			underline->width,
			underline->offset + underline->thickness
		);

		ID2D1RectangleGeometry* pRectangleGeometry = NULL;
		hr = pFactory_->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);

		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = NULL;
		if (SUCCEEDED(hr))
		{
			hr = pFactory_->CreateTransformedGeometry(
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
				fOutlineWidth * 2,
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

		DX::SafeRelease(pRectangleGeometry);
		DX::SafeRelease(pTransformedGeometry);

		return S_OK;
	}

	STDMETHODIMP TextRenderer::DrawStrikethrough(
		__maybenull void* clientDrawingContext,
		float baselineOriginX,
		float baselineOriginY,
		__in DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect)
	{
		KGE_UNUSED(clientDrawingContext);
		KGE_UNUSED(clientDrawingEffect);

		HRESULT hr;

		D2D1_RECT_F rect = D2D1::RectF(
			0,
			strikethrough->offset,
			strikethrough->width,
			strikethrough->offset + strikethrough->thickness
		);

		ID2D1RectangleGeometry* pRectangleGeometry = NULL;
		hr = pFactory_->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);

		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = NULL;
		if (SUCCEEDED(hr))
		{
			hr = pFactory_->CreateTransformedGeometry(
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
				fOutlineWidth * 2,
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

		DX::SafeRelease(pRectangleGeometry);
		DX::SafeRelease(pTransformedGeometry);

		return S_OK;
	}

	STDMETHODIMP TextRenderer::DrawInlineObject(
		__maybenull void* clientDrawingContext,
		float originX,
		float originY,
		IDWriteInlineObject* inlineObject,
		BOOL IsSideways,
		BOOL IsRightToLeft,
		IUnknown* clientDrawingEffect)
	{
		KGE_UNUSED(clientDrawingContext);
		KGE_UNUSED(originX);
		KGE_UNUSED(originY);
		KGE_UNUSED(inlineObject);
		KGE_UNUSED(IsSideways);
		KGE_UNUSED(IsRightToLeft);
		KGE_UNUSED(clientDrawingEffect);
		return E_NOTIMPL;
	}

	STDMETHODIMP TextRenderer::IsPixelSnappingDisabled(
		__maybenull void* clientDrawingContext,
		__out BOOL* isDisabled)
	{
		KGE_UNUSED(clientDrawingContext);

		*isDisabled = FALSE;
		return S_OK;
	}

	STDMETHODIMP TextRenderer::GetCurrentTransform(
		__maybenull void* clientDrawingContext,
		__out DWRITE_MATRIX* transform)
	{
		KGE_UNUSED(clientDrawingContext);

		pRT_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
		return S_OK;
	}

	STDMETHODIMP TextRenderer::GetPixelsPerDip(
		__maybenull void* clientDrawingContext,
		__out float* pixelsPerDip)
	{
		KGE_UNUSED(clientDrawingContext);

		float x, yUnused;

		pRT_->GetDpi(&x, &yUnused);
		*pixelsPerDip = x / 96;

		return S_OK;
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

	STDMETHODIMP TextRenderer::QueryInterface(
		REFIID riid,
		void** ppvObject)
	{
		if (__uuidof(ITextRenderer) == riid)
		{
			*ppvObject = this;
		}
		else if (__uuidof(IDWriteTextRenderer) == riid)
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
			*ppvObject = NULL;
			return E_FAIL;
		}

		AddRef();

		return S_OK;
	}
}
