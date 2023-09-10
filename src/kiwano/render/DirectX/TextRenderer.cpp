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

#include <kiwano/render/DirectX/TextRenderer.h>
#include <kiwano/render/DirectX/TextDrawingEffect.h>

namespace kiwano
{
class TextRenderer : public ITextRenderer
{
public:
    TextRenderer();

    ~TextRenderer();

    STDMETHOD(CreateDeviceResources)(_In_ ID2D1RenderTarget* pRT);

    STDMETHOD(DrawTextLayout)
    (_In_ IDWriteTextLayout* pTextLayout, float fOriginX, float fOriginY, _In_opt_ ID2D1Brush* pDefaultFillBrush,
     _In_opt_ ID2D1Brush* pDefaultOutlineBrush, float fDefaultOutlineWidth, _In_opt_ ID2D1StrokeStyle* pStrokeStyle);

    STDMETHOD(DrawGlyphRun)
    (__maybenull void* clientDrawingContext, float baselineOriginX, float baselineOriginY,
     DWRITE_MEASURING_MODE measuringMode, __in DWRITE_GLYPH_RUN const* glyphRun,
     __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, IUnknown* clientDrawingEffect);

    STDMETHOD(DrawUnderline)
    (__maybenull void* clientDrawingContext, float baselineOriginX, float baselineOriginY,
     __in DWRITE_UNDERLINE const* underline, IUnknown* clientDrawingEffect);

    STDMETHOD(DrawStrikethrough)
    (__maybenull void* clientDrawingContext, float baselineOriginX, float baselineOriginY,
     __in DWRITE_STRIKETHROUGH const* strikethrough, IUnknown* clientDrawingEffect);

    STDMETHOD(DrawInlineObject)
    (__maybenull void* clientDrawingContext, float originX, float originY, IDWriteInlineObject* inlineObject,
     BOOL IsSideways, BOOL IsRightToLeft, IUnknown* clientDrawingEffect);

    STDMETHOD(IsPixelSnappingDisabled)(__maybenull void* clientDrawingContext, __out BOOL* isDisabled);

    STDMETHOD(GetCurrentTransform)(__maybenull void* clientDrawingContext, __out DWRITE_MATRIX* transform);

    STDMETHOD(GetPixelsPerDip)(__maybenull void* clientDrawingContext, __out float* pixelsPerDip);

    STDMETHOD_(uint32_t, GetLastPrimitivesCount)();

public:
    unsigned long STDMETHODCALLTYPE AddRef();
    unsigned long STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE       QueryInterface(REFIID riid, void** ppvObject);

private:
    unsigned long             cRefCount_;
    uint32_t                  cPrimitivesCount_;
    float                     fDefaultOutlineWidth_;
    ComPtr<ID2D1Factory>      pFactory_;
    ComPtr<ID2D1RenderTarget> pRT_;
    ComPtr<ID2D1Brush>        pDefaultFillBrush_;
    ComPtr<ID2D1Brush>        pDefaultOutlineBrush_;
    ComPtr<ID2D1StrokeStyle>  pDefaultStrokeStyle_;
};

HRESULT ITextRenderer::Create(_Out_ ITextRenderer** ppTextRenderer, _In_ ID2D1RenderTarget* pRT)
{
    HRESULT hr = E_FAIL;

    if (ppTextRenderer)
    {
        TextRenderer* pTextRenderer = new (std::nothrow) TextRenderer;
        if (pTextRenderer)
        {
            hr = pTextRenderer->CreateDeviceResources(pRT);

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

TextRenderer::TextRenderer()
    : cRefCount_(0)
    , cPrimitivesCount_(0)
    , fDefaultOutlineWidth_(1)
{
}

TextRenderer::~TextRenderer() {}

STDMETHODIMP TextRenderer::CreateDeviceResources(_In_ ID2D1RenderTarget* pRT)
{
    HRESULT hr = E_FAIL;

    pFactory_.Reset();
    pRT_.Reset();

    if (pRT)
    {
        pRT_ = pRT;
        pRT_->GetFactory(&pFactory_);
        hr = S_OK;
    }
    return hr;
}

STDMETHODIMP TextRenderer::DrawTextLayout(_In_ IDWriteTextLayout* pTextLayout, float fOriginX, float fOriginY,
                                          _In_opt_ ID2D1Brush* pDefaultFillBrush,
                                          _In_opt_ ID2D1Brush* pDefaultOutlineBrush, float fDefaultOutlineWidth,
                                          _In_opt_ ID2D1StrokeStyle* pDefaultStrokeStyle)
{
    if (!pTextLayout)
    {
        return E_INVALIDARG;
    }

    cPrimitivesCount_     = 0;
    pDefaultFillBrush_    = pDefaultFillBrush;
    pDefaultOutlineBrush_ = pDefaultOutlineBrush;
    fDefaultOutlineWidth_ = fDefaultOutlineWidth;
    pDefaultStrokeStyle_  = pDefaultStrokeStyle;

    return pTextLayout->Draw(nullptr, this, fOriginX, fOriginY);
}

STDMETHODIMP TextRenderer::DrawGlyphRun(__maybenull void* clientDrawingContext, float baselineOriginX,
                                        float baselineOriginY, DWRITE_MEASURING_MODE measuringMode,
                                        __in DWRITE_GLYPH_RUN const* glyphRun,
                                        __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
                                        IUnknown*                                clientDrawingEffect)
{
    KGE_NOT_USED(clientDrawingContext);
    KGE_NOT_USED(measuringMode);
    KGE_NOT_USED(glyphRunDescription);

    ComPtr<ITextDrawingEffect> pTextDrawingEffect;

    HRESULT hr = clientDrawingEffect->QueryInterface(&pTextDrawingEffect);

    if (pDefaultOutlineBrush_)
    {
        ComPtr<ID2D1Geometry> pOutlineGeometry;

        if (SUCCEEDED(hr))
        {
            hr = pTextDrawingEffect->CreateOutlineGeomerty(&pOutlineGeometry, glyphRun, baselineOriginX,
                                                           baselineOriginY);
        }

        if (SUCCEEDED(hr))
        {
            pRT_->DrawGeometry(pOutlineGeometry.Get(), pDefaultOutlineBrush_.Get(), fDefaultOutlineWidth_,
                               pDefaultStrokeStyle_.Get());

            ++cPrimitivesCount_;
        }
    }

    if (pDefaultFillBrush_)
    {
        if (SUCCEEDED(hr))
        {
            pRT_->DrawGlyphRun(D2D1::Point2F(baselineOriginX, baselineOriginY), glyphRun, pDefaultFillBrush_.Get());

            ++cPrimitivesCount_;
        }
    }
    return hr;
}

STDMETHODIMP TextRenderer::DrawUnderline(__maybenull void* clientDrawingContext, float baselineOriginX,
                                         float baselineOriginY, __in DWRITE_UNDERLINE const* underline,
                                         IUnknown* clientDrawingEffect)
{
    KGE_NOT_USED(clientDrawingContext);

    HRESULT hr = S_OK;

    ComPtr<ID2D1RectangleGeometry>   pRectangleGeometry;
    ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
    ComPtr<ID2D1Brush>               pCurrentFillBrush;

    if (clientDrawingEffect)
    {
        hr = clientDrawingEffect->QueryInterface<ID2D1Brush>(&pCurrentFillBrush);
    }
    else
    {
        pCurrentFillBrush = pDefaultFillBrush_;
    }

    if (pCurrentFillBrush || pDefaultOutlineBrush_)
    {
        if (SUCCEEDED(hr))
        {
            D2D1_RECT_F rect =
                D2D1::RectF(0, underline->offset, underline->width, underline->offset + underline->thickness);

            hr = pFactory_->CreateRectangleGeometry(&rect, &pRectangleGeometry);
        }

        if (SUCCEEDED(hr))
        {
            D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, baselineOriginX, baselineOriginY);

            hr = pFactory_->CreateTransformedGeometry(pRectangleGeometry.Get(), &matrix, &pTransformedGeometry);
        }
    }

    if (SUCCEEDED(hr) && pDefaultOutlineBrush_)
    {
        pRT_->DrawGeometry(pTransformedGeometry.Get(), pDefaultOutlineBrush_.Get(), fDefaultOutlineWidth_,
                           pDefaultStrokeStyle_.Get());

        ++cPrimitivesCount_;
    }

    if (SUCCEEDED(hr) && pCurrentFillBrush)
    {
        pRT_->FillGeometry(pTransformedGeometry.Get(), pCurrentFillBrush.Get());

        ++cPrimitivesCount_;
    }
    return hr;
}

STDMETHODIMP TextRenderer::DrawStrikethrough(__maybenull void* clientDrawingContext, float baselineOriginX,
                                             float baselineOriginY, __in DWRITE_STRIKETHROUGH const* strikethrough,
                                             IUnknown* clientDrawingEffect)
{
    KGE_NOT_USED(clientDrawingContext);

    HRESULT hr = S_OK;

    ComPtr<ID2D1RectangleGeometry>   pRectangleGeometry;
    ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
    ComPtr<ID2D1Brush>               pCurrentFillBrush;

    if (clientDrawingEffect)
    {
        hr = clientDrawingEffect->QueryInterface<ID2D1Brush>(&pCurrentFillBrush);
    }
    else
    {
        pCurrentFillBrush = pDefaultFillBrush_;
    }

    if (pCurrentFillBrush || pDefaultOutlineBrush_)
    {
        if (SUCCEEDED(hr))
        {
            D2D1_RECT_F rect = D2D1::RectF(0, strikethrough->offset, strikethrough->width,
                                           strikethrough->offset + strikethrough->thickness);

            hr = pFactory_->CreateRectangleGeometry(&rect, &pRectangleGeometry);
        }

        if (SUCCEEDED(hr))
        {
            D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, baselineOriginX, baselineOriginY);

            hr = pFactory_->CreateTransformedGeometry(pRectangleGeometry.Get(), &matrix, &pTransformedGeometry);
        }
    }

    if (SUCCEEDED(hr) && pDefaultOutlineBrush_)
    {
        pRT_->DrawGeometry(pTransformedGeometry.Get(), pDefaultOutlineBrush_.Get(), fDefaultOutlineWidth_,
                           pDefaultStrokeStyle_.Get());

        ++cPrimitivesCount_;
    }

    if (SUCCEEDED(hr) && pCurrentFillBrush)
    {
        pRT_->FillGeometry(pTransformedGeometry.Get(), pCurrentFillBrush.Get());

        ++cPrimitivesCount_;
    }
    return hr;
}

STDMETHODIMP TextRenderer::DrawInlineObject(__maybenull void* clientDrawingContext, float originX, float originY,
                                            IDWriteInlineObject* inlineObject, BOOL IsSideways, BOOL IsRightToLeft,
                                            IUnknown* clientDrawingEffect)
{
    KGE_NOT_USED(clientDrawingContext);
    KGE_NOT_USED(originX);
    KGE_NOT_USED(originY);
    KGE_NOT_USED(inlineObject);
    KGE_NOT_USED(IsSideways);
    KGE_NOT_USED(IsRightToLeft);
    KGE_NOT_USED(clientDrawingEffect);
    return E_NOTIMPL;
}

STDMETHODIMP TextRenderer::IsPixelSnappingDisabled(__maybenull void* clientDrawingContext, __out BOOL* isDisabled)
{
    KGE_NOT_USED(clientDrawingContext);

    *isDisabled = FALSE;
    return S_OK;
}

STDMETHODIMP TextRenderer::GetCurrentTransform(__maybenull void* clientDrawingContext, __out DWRITE_MATRIX* transform)
{
    KGE_NOT_USED(clientDrawingContext);

    pRT_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
    return S_OK;
}

STDMETHODIMP TextRenderer::GetPixelsPerDip(__maybenull void* clientDrawingContext, __out float* pixelsPerDip)
{
    KGE_NOT_USED(clientDrawingContext);

    float x, yUnused;

    pRT_->GetDpi(&x, &yUnused);
    *pixelsPerDip = x / 96;

    return S_OK;
}

STDMETHODIMP_(uint32_t) TextRenderer::GetLastPrimitivesCount()
{
    return cPrimitivesCount_;
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

STDMETHODIMP TextRenderer::QueryInterface(REFIID riid, void** ppvObject)
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
}  // namespace kiwano
