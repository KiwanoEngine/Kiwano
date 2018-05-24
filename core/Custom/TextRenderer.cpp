#include "..\e2dcustom.h"
#include "..\e2dbase.h"

using namespace e2d;

TextRenderer::TextRenderer()
	: cRefCount_(0)
	, pD2DFactory_(nullptr)
	, pRT_(nullptr)
	, pBrush_(nullptr)
	, sFillColor_()
	, sOutlineColor_()
	, fOutlineWidth(1)
	, bShowOutline_(TRUE)
	, pCurrStrokeStyle_(nullptr)
{
}

TextRenderer::~TextRenderer()
{
	SafeRelease(pD2DFactory_);
	SafeRelease(pRT_);
	SafeRelease(pBrush_);
}

TextRenderer * TextRenderer::Create(
	ID2D1Factory* pD2DFactory,
	ID2D1HwndRenderTarget* pRT,
	ID2D1SolidColorBrush* pBrush
)
{
	TextRenderer * pTextRenderer = new (std::nothrow) TextRenderer();
	if (pTextRenderer)
	{
		pD2DFactory->AddRef();
		pRT->AddRef();
		pBrush->AddRef();

		pTextRenderer->pD2DFactory_ = pD2DFactory;
		pTextRenderer->pRT_ = pRT;
		pTextRenderer->pBrush_ = pBrush;
	}
	return pTextRenderer;
}

STDMETHODIMP_(void) TextRenderer::SetTextStyle(
	CONST D2D1_COLOR_F &fillColor,
	BOOL hasOutline,
	CONST D2D1_COLOR_F &outlineColor,
	FLOAT outlineWidth,
	D2D1_LINE_JOIN outlineJoin
)
{
	sFillColor_ = fillColor;
	bShowOutline_ = hasOutline;
	sOutlineColor_ = outlineColor;
	fOutlineWidth = 2 * outlineWidth;

	switch (outlineJoin)
	{
	case D2D1_LINE_JOIN_MITER:
		pCurrStrokeStyle_ = Renderer::getMiterID2D1StrokeStyle();
		break;
	case D2D1_LINE_JOIN_BEVEL:
		pCurrStrokeStyle_ = Renderer::getBevelID2D1StrokeStyle();
		break;
	case D2D1_LINE_JOIN_ROUND:
		pCurrStrokeStyle_ = Renderer::getRoundID2D1StrokeStyle();
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
	BOOL isSideways,
	BOOL isRightToLeft,
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