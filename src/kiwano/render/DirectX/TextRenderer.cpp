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
namespace graphics
{
namespace directx
{
class TextRenderer : public ITextRenderer
{
public:
    TextRenderer();

    ~TextRenderer();

    STDMETHOD(CreateDeviceResources)(_In_ ID2D1DeviceContext* pContext);

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
    bool                       bOutlineRendering_;
    unsigned long              cRefCount_;
    uint32_t                   cPrimitivesCount_;
    float                      fDefaultOutlineWidth_;
    ComPtr<ID2D1Factory>       pFactory_;
    ComPtr<ID2D1DeviceContext> pContext_;
    ComPtr<ID2D1Brush>         pDefaultFillBrush_;
    ComPtr<ID2D1Brush>         pDefaultOutlineBrush_;
    ComPtr<ID2D1StrokeStyle>   pDefaultStrokeStyle_;
};

HRESULT ITextRenderer::Create(_Out_ ITextRenderer** ppTextRenderer, _In_ ID2D1DeviceContext* pContext)
{
    HRESULT hr = E_FAIL;

    if (ppTextRenderer)
    {
        TextRenderer* pTextRenderer = new (std::nothrow) TextRenderer;
        if (pTextRenderer)
        {
            hr = pTextRenderer->CreateDeviceResources(pContext);

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
    : bOutlineRendering_(false)
    , cRefCount_(0)
    , cPrimitivesCount_(0)
    , fDefaultOutlineWidth_(1)
{
}

TextRenderer::~TextRenderer() {}

STDMETHODIMP TextRenderer::CreateDeviceResources(_In_ ID2D1DeviceContext* pContext)
{
    HRESULT hr = E_FAIL;

    pFactory_.Reset();
    pContext_.Reset();

    if (pContext)
    {
        pContext_ = pContext;
        pContext_->GetFactory(&pFactory_);
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
    bOutlineRendering_    = pDefaultOutlineBrush_ != nullptr;

    HRESULT hr = pTextLayout->Draw(nullptr, this, fOriginX, fOriginY);
    if (SUCCEEDED(hr) && bOutlineRendering_)
    {
        bOutlineRendering_ = false;

        hr = pTextLayout->Draw(nullptr, this, fOriginX, fOriginY);
    }
    return hr;
}

STDMETHODIMP TextRenderer::DrawGlyphRun(__maybenull void* clientDrawingContext, float baselineOriginX,
                                        float baselineOriginY, DWRITE_MEASURING_MODE measuringMode,
                                        __in DWRITE_GLYPH_RUN const*             glyphRun,
                                        __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
                                        IUnknown*                                clientDrawingEffect)
{
    KGE_NOT_USED(clientDrawingContext);
    KGE_NOT_USED(measuringMode);
    KGE_NOT_USED(glyphRunDescription);

    HRESULT hr = S_OK;
    if (!bOutlineRendering_)
    {
        if (pDefaultFillBrush_)
        {
            if (SUCCEEDED(hr))
            {
                pContext_->DrawGlyphRun(D2D1::Point2F(baselineOriginX, baselineOriginY), glyphRun,
                                        pDefaultFillBrush_.Get());

                ++cPrimitivesCount_;
            }
        }
    }
    else
    {
        if (pDefaultOutlineBrush_)
        {
            ComPtr<ITextDrawingEffect> pTextDrawingEffect;
            hr = clientDrawingEffect->QueryInterface(&pTextDrawingEffect);

            ComPtr<ID2D1Geometry> pOutlineGeometry;
            if (SUCCEEDED(hr))
            {
                hr = pTextDrawingEffect->CreateOutlineGeomerty(&pOutlineGeometry, glyphRun, baselineOriginX,
                                                               baselineOriginY);
            }

            if (SUCCEEDED(hr))
            {
                pContext_->DrawGeometry(pOutlineGeometry.Get(), pDefaultOutlineBrush_.Get(), fDefaultOutlineWidth_,
                                        pDefaultStrokeStyle_.Get());

                ++cPrimitivesCount_;
            }
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

    ComPtr<ITextDrawingEffect> pTextDrawingEffect;
    hr = clientDrawingEffect->QueryInterface(&pTextDrawingEffect);

    ComPtr<ID2D1Geometry> pUnderlineGeometry;
    if (SUCCEEDED(hr))
    {
        hr = pTextDrawingEffect->CreateUnderlineGeomerty(&pUnderlineGeometry, underline, baselineOriginX,
                                                         baselineOriginY);
    }

    if (SUCCEEDED(hr))
    {
        if (!bOutlineRendering_)
        {
            if (pDefaultFillBrush_)
            {
                pContext_->FillGeometry(pUnderlineGeometry.Get(), pDefaultFillBrush_.Get());
                ++cPrimitivesCount_;
            }
        }
        else
        {
            if (pDefaultOutlineBrush_)
            {
                pContext_->DrawGeometry(pUnderlineGeometry.Get(), pDefaultOutlineBrush_.Get(),
                                        fDefaultOutlineWidth_, pDefaultStrokeStyle_.Get());

                ++cPrimitivesCount_;
            }
        }
    }
    return hr;
}

STDMETHODIMP TextRenderer::DrawStrikethrough(__maybenull void* clientDrawingContext, float baselineOriginX,
                                             float baselineOriginY, __in DWRITE_STRIKETHROUGH const* strikethrough,
                                             IUnknown* clientDrawingEffect)
{
    KGE_NOT_USED(clientDrawingContext);

    HRESULT hr = S_OK;

    ComPtr<ITextDrawingEffect> pTextDrawingEffect;
    hr = clientDrawingEffect->QueryInterface(&pTextDrawingEffect);

    ComPtr<ID2D1Geometry> pStrikethroughGeometry;
    if (SUCCEEDED(hr))
    {
        hr = pTextDrawingEffect->CreateStrikethroughGeomerty(&pStrikethroughGeometry, strikethrough, baselineOriginX,
                                                             baselineOriginY);
    }

    if (SUCCEEDED(hr))
    {
        if (!bOutlineRendering_)
        {
            if (pDefaultFillBrush_)
            {
                pContext_->FillGeometry(pStrikethroughGeometry.Get(), pDefaultFillBrush_.Get());
                ++cPrimitivesCount_;
            }
        }
        else
        {
            if (pDefaultOutlineBrush_)
            {
                pContext_->DrawGeometry(pStrikethroughGeometry.Get(), pDefaultOutlineBrush_.Get(),
                                        fDefaultOutlineWidth_, pDefaultStrokeStyle_.Get());

                ++cPrimitivesCount_;
            }
        }
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

    pContext_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
    return S_OK;
}

STDMETHODIMP TextRenderer::GetPixelsPerDip(__maybenull void* clientDrawingContext, __out float* pixelsPerDip)
{
    KGE_NOT_USED(clientDrawingContext);

    float x, yUnused;

    pContext_->GetDpi(&x, &yUnused);
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
}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
