#include "..\e2dcustom.h"
#include "..\e2dbase.h"

using namespace e2d;

CustomTextRenderer::CustomTextRenderer(
	ID2D1Factory* pD2DFactory,
	ID2D1HwndRenderTarget* pRT,
	ID2D1SolidColorBrush* pBrush
)
	: cRefCount_(0)
	, pD2DFactory_(pD2DFactory)
	, pRT_(pRT)
	, pBrush_(pBrush)
	, sFillColor_()
	, sOutlineColor_()
	, fOutlineWidth(1)
	, bShowOutline_(TRUE)
	, nOutlineJoin_(D2D1_LINE_JOIN_MITER)
{
	pD2DFactory_->AddRef();
	pRT_->AddRef();
	pBrush_->AddRef();
}

CustomTextRenderer::~CustomTextRenderer()
{
	SafeReleaseInterface(pD2DFactory_);
	SafeReleaseInterface(pRT_);
	SafeReleaseInterface(pBrush_);
}

STDMETHODIMP_(void) CustomTextRenderer::SetTextStyle(
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
	nOutlineJoin_ = outlineJoin;
}

STDMETHODIMP CustomTextRenderer::DrawGlyphRun(
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

	ID2D1PathGeometry* pPathGeometry = NULL;
	hr = pD2DFactory_->CreatePathGeometry(
		&pPathGeometry
	);

	ID2D1GeometrySink* pSink = NULL;
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

	ID2D1TransformedGeometry* pTransformedGeometry = NULL;
	if (SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pPathGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	ID2D1StrokeStyle * pStrokeStyle = NULL;

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getID2D1Factory()->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				nOutlineJoin_,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			NULL,
			0,
			&pStrokeStyle
		);
	}

	if (SUCCEEDED(hr))
	{
		if (bShowOutline_)
		{
			pBrush_->SetColor(sOutlineColor_);

			pRT_->DrawGeometry(
				pTransformedGeometry,
				pBrush_,
				fOutlineWidth,
				pStrokeStyle
			);
		}

		pBrush_->SetColor(sFillColor_);

		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush_
		);
	}

	SafeReleaseInterface(pPathGeometry);
	SafeReleaseInterface(pSink);
	SafeReleaseInterface(pTransformedGeometry);

	return hr;
}

STDMETHODIMP CustomTextRenderer::DrawUnderline(
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

	ID2D1RectangleGeometry* pRectangleGeometry = NULL;
	hr = pD2DFactory_->CreateRectangleGeometry(
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
		hr = pD2DFactory_->CreateTransformedGeometry(
			pRectangleGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	ID2D1StrokeStyle * pStrokeStyle = NULL;

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getID2D1Factory()->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				nOutlineJoin_,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			NULL,
			0,
			&pStrokeStyle
		);
	}

	if (SUCCEEDED(hr))
	{
		if (bShowOutline_)
		{
			pBrush_->SetColor(sOutlineColor_);

			pRT_->DrawGeometry(
				pTransformedGeometry,
				pBrush_,
				fOutlineWidth,
				pStrokeStyle
			);
		}

		pBrush_->SetColor(sFillColor_);

		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush_
		);
	}

	SafeReleaseInterface(pRectangleGeometry);
	SafeReleaseInterface(pTransformedGeometry);

	return S_OK;
}

STDMETHODIMP CustomTextRenderer::DrawStrikethrough(
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

	ID2D1RectangleGeometry* pRectangleGeometry = NULL;
	hr = pD2DFactory_->CreateRectangleGeometry(
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
		hr = pD2DFactory_->CreateTransformedGeometry(
			pRectangleGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	ID2D1StrokeStyle * pStrokeStyle = NULL;

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getID2D1Factory()->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				nOutlineJoin_,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			NULL,
			0,
			&pStrokeStyle
		);
	}

	if (SUCCEEDED(hr))
	{
		if (bShowOutline_)
		{
			pBrush_->SetColor(sOutlineColor_);

			pRT_->DrawGeometry(
				pTransformedGeometry,
				pBrush_,
				fOutlineWidth,
				pStrokeStyle
			);
		}

		pBrush_->SetColor(sFillColor_);

		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush_
		);
	}

	SafeReleaseInterface(pRectangleGeometry);
	SafeReleaseInterface(pTransformedGeometry);

	return S_OK;
}

STDMETHODIMP CustomTextRenderer::DrawInlineObject(
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

STDMETHODIMP_(unsigned long) CustomTextRenderer::AddRef()
{
	return InterlockedIncrement(&cRefCount_);
}

STDMETHODIMP_(unsigned long) CustomTextRenderer::Release()
{
	unsigned long newCount = InterlockedDecrement(&cRefCount_);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

STDMETHODIMP CustomTextRenderer::IsPixelSnappingDisabled(
	__maybenull void* clientDrawingContext,
	__out BOOL* isDisabled
)
{
	*isDisabled = FALSE;
	return S_OK;
}

STDMETHODIMP CustomTextRenderer::GetCurrentTransform(
	__maybenull void* clientDrawingContext,
	__out DWRITE_MATRIX* transform
)
{
	pRT_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}

STDMETHODIMP CustomTextRenderer::GetPixelsPerDip(
	__maybenull void* clientDrawingContext,
	__out FLOAT* pixelsPerDip
)
{
	float x, yUnused;

	pRT_->GetDpi(&x, &yUnused);
	*pixelsPerDip = x / 96;

	return S_OK;
}

STDMETHODIMP CustomTextRenderer::QueryInterface(
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
		*ppvObject = NULL;
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}