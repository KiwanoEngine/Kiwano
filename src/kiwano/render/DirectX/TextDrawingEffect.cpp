// Copyright (c) 2023 Kiwano - Nomango
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

#include <tuple>
#include <kiwano/render/DirectX/TextDrawingEffect.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{
class TextDrawingEffect : public ITextDrawingEffect
{
public:
    TextDrawingEffect();

    STDMETHOD(CreateDeviceResources)(_In_ ID2D1Factory* pFactory);

    STDMETHOD(CreateOutlineGeomerty)
    (_Out_ ID2D1Geometry** ppOutlineGeo, _In_ DWRITE_GLYPH_RUN const* glyphRun, float fOriginX, float fOriginY);

    STDMETHOD(CreateStrikethroughGeomerty)
    (_Out_ ID2D1Geometry** ppStrikethroughGeo, _In_ DWRITE_STRIKETHROUGH const* strikethrough, float fOriginX,
     float fOriginY);

    STDMETHOD(CreateUnderlineGeomerty)
    (_Out_ ID2D1Geometry** ppUnderlineGeo, _In_ DWRITE_UNDERLINE const* underline, float fOriginX, float fOriginY);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    unsigned long        cRefCount_;
    ComPtr<ID2D1Factory> pFactory_;

    Map<std::tuple<const DWRITE_GLYPH_RUN*, float, float>, ComPtr<ID2D1Geometry>>     outlineCache_;
    Map<std::tuple<const DWRITE_STRIKETHROUGH*, float, float>, ComPtr<ID2D1Geometry>> strikethroughCache_;
    Map<std::tuple<const DWRITE_UNDERLINE*, float, float>, ComPtr<ID2D1Geometry>>     underlineCache_;
};

HRESULT ITextDrawingEffect::Create(_Out_ ITextDrawingEffect** ppTextDrawingEffect, _In_ ID2D1Factory* pFactory)
{
    HRESULT hr = E_FAIL;

    if (ppTextDrawingEffect)
    {
        TextDrawingEffect* pTextDrawingEffect = new (std::nothrow) TextDrawingEffect;
        if (pTextDrawingEffect)
        {
            hr = pTextDrawingEffect->CreateDeviceResources(pFactory);

            if (SUCCEEDED(hr))
            {
                pTextDrawingEffect->AddRef();

                DX::SafeRelease(*ppTextDrawingEffect);
                (*ppTextDrawingEffect) = pTextDrawingEffect;
                return S_OK;
            }
            else
            {
                delete pTextDrawingEffect;
                pTextDrawingEffect = NULL;
            }
        }
    }
    return hr;
}

TextDrawingEffect::TextDrawingEffect()
    : cRefCount_(0)
    , outlineCache_{}
{
}

STDMETHODIMP TextDrawingEffect::CreateDeviceResources(_In_ ID2D1Factory* pFactory)
{
    pFactory_ = pFactory;
    return S_OK;
}

STDMETHODIMP TextDrawingEffect::CreateOutlineGeomerty(_Out_ ID2D1Geometry**        ppOutlineGeo,
                                                      _In_ DWRITE_GLYPH_RUN const* glyphRun, float fOriginX,
                                                      float fOriginY)
{
    auto cache = outlineCache_.find(std::make_tuple(glyphRun, fOriginX, fOriginY));
    if (cache != outlineCache_.end())
    {
        auto& pOutlineGeo = cache->second;
        if (pOutlineGeo)
        {
            // Use cached geometry
            pOutlineGeo->AddRef();
            DX::SafeRelease(*ppOutlineGeo);
            (*ppOutlineGeo) = pOutlineGeo.Get();
            return S_OK;
        }
    }

    HRESULT hr = S_OK;

    ComPtr<ID2D1Geometry> pOutlineGeo;
    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1PathGeometry> pPathGeometry;
        hr = pFactory_->CreatePathGeometry(&pPathGeometry);

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1GeometrySink> pSink;
            hr = pPathGeometry->Open(&pSink);

            if (SUCCEEDED(hr))
            {
                hr = glyphRun->fontFace->GetGlyphRunOutline(
                    glyphRun->fontEmSize, glyphRun->glyphIndices, glyphRun->glyphAdvances, glyphRun->glyphOffsets,
                    glyphRun->glyphCount, glyphRun->isSideways, glyphRun->bidiLevel % 2, pSink.Get());
            }

            if (SUCCEEDED(hr))
            {
                hr = pSink->Close();
            }

            if (SUCCEEDED(hr))
            {
                const auto matrix = D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, fOriginX, fOriginY);

                ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
                hr = pFactory_->CreateTransformedGeometry(pPathGeometry.Get(), &matrix, &pTransformedGeometry);

                if (SUCCEEDED(hr))
                {
                    pOutlineGeo = pTransformedGeometry;
                }
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        pOutlineGeo->AddRef();
        DX::SafeRelease(*ppOutlineGeo);
        (*ppOutlineGeo) = pOutlineGeo.Get();

        outlineCache_.insert(std::make_pair(std::make_tuple(glyphRun, fOriginX, fOriginY), pOutlineGeo));
    }
    return hr;
}

STDMETHODIMP TextDrawingEffect::CreateStrikethroughGeomerty(_Out_ ID2D1Geometry**            ppStrikethroughGeo,
                                                            _In_ DWRITE_STRIKETHROUGH const* strikethrough,
                                                            float fOriginX, float fOriginY)
{
    auto cache = strikethroughCache_.find(std::make_tuple(strikethrough, fOriginX, fOriginY));
    if (cache != strikethroughCache_.end())
    {
        auto& pStrikethroughGeo = cache->second;
        if (pStrikethroughGeo)
        {
            // Use cached geometry
            pStrikethroughGeo->AddRef();
            DX::SafeRelease(*ppStrikethroughGeo);
            (*ppStrikethroughGeo) = pStrikethroughGeo.Get();
            return S_OK;
        }
    }

    HRESULT hr = S_OK;

    ComPtr<ID2D1Geometry> pStrikethroughGeo;
    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1RectangleGeometry> pRectangleGeometry;

        D2D1_RECT_F rect = D2D1::RectF(0, strikethrough->offset, strikethrough->width,
                                       strikethrough->offset + strikethrough->thickness);
        hr               = pFactory_->CreateRectangleGeometry(&rect, &pRectangleGeometry);

        if (SUCCEEDED(hr))
        {
            const auto matrix = D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, fOriginX, fOriginY);

            ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
            hr = pFactory_->CreateTransformedGeometry(pRectangleGeometry.Get(), &matrix, &pTransformedGeometry);

            if (SUCCEEDED(hr))
            {
                pStrikethroughGeo = pTransformedGeometry;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        pStrikethroughGeo->AddRef();
        DX::SafeRelease(*ppStrikethroughGeo);
        (*ppStrikethroughGeo) = pStrikethroughGeo.Get();

        strikethroughCache_.insert(
            std::make_pair(std::make_tuple(strikethrough, fOriginX, fOriginY), pStrikethroughGeo));
    }
    return hr;
}

STDMETHODIMP TextDrawingEffect::CreateUnderlineGeomerty(_Out_ ID2D1Geometry**        ppUnderlineGeo,
                                                        _In_ DWRITE_UNDERLINE const* underline, float fOriginX,
                                                        float fOriginY)
{
    auto cache = underlineCache_.find(std::make_tuple(underline, fOriginX, fOriginY));
    if (cache != underlineCache_.end())
    {
        auto& pUnderlineGeo = cache->second;
        if (pUnderlineGeo)
        {
            // Use cached geometry
            pUnderlineGeo->AddRef();
            DX::SafeRelease(*ppUnderlineGeo);
            (*ppUnderlineGeo) = pUnderlineGeo.Get();
            return S_OK;
        }
    }

    HRESULT hr = S_OK;

    ComPtr<ID2D1Geometry> pUnderlineGeo;
    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1RectangleGeometry> pRectangleGeometry;

        D2D1_RECT_F rect =
            D2D1::RectF(0, underline->offset, underline->width, underline->offset + underline->thickness);
        hr = pFactory_->CreateRectangleGeometry(&rect, &pRectangleGeometry);

        if (SUCCEEDED(hr))
        {
            const auto matrix = D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, fOriginX, fOriginY);

            ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
            hr = pFactory_->CreateTransformedGeometry(pRectangleGeometry.Get(), &matrix, &pTransformedGeometry);

            if (SUCCEEDED(hr))
            {
                pUnderlineGeo = pTransformedGeometry;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        pUnderlineGeo->AddRef();
        DX::SafeRelease(*ppUnderlineGeo);
        (*ppUnderlineGeo) = pUnderlineGeo.Get();

        underlineCache_.insert(std::make_pair(std::make_tuple(underline, fOriginX, fOriginY), pUnderlineGeo));
    }
    return hr;
}

STDMETHODIMP_(unsigned long) TextDrawingEffect::AddRef()
{
    return InterlockedIncrement(&cRefCount_);
}

STDMETHODIMP_(unsigned long) TextDrawingEffect::Release()
{
    unsigned long newCount = InterlockedDecrement(&cRefCount_);

    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

STDMETHODIMP TextDrawingEffect::QueryInterface(REFIID riid, void** ppvObject)
{
    if (__uuidof(ITextDrawingEffect) == riid)
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
