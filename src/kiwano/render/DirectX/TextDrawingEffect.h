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

#pragma once
#include <kiwano/render/DirectX/helper.h>
#include <dwrite.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{
interface DWRITE_DECLARE_INTERFACE("7431F439-6E54-4707-A0DC-1AA035D6AFB8") ITextDrawingEffect : public IUnknown
{
public:
    static HRESULT Create(_Out_ ITextDrawingEffect * *ppTextDrawingEffect, _In_ ID2D1Factory * pFactory);

    STDMETHOD(CreateOutlineGeomerty)
    (_Out_ ID2D1Geometry * *ppOutlineGeo, _In_ DWRITE_GLYPH_RUN const* glyphRun, float fOriginX, float fOriginY) PURE;

    STDMETHOD(CreateStrikethroughGeomerty)
    (_Out_ ID2D1Geometry * *ppStrikethroughGeo, _In_ DWRITE_STRIKETHROUGH const* strikethrough, float fOriginX,
     float fOriginY) PURE;

    STDMETHOD(CreateUnderlineGeomerty)
    (_Out_ ID2D1Geometry * *ppUnderlineGeo, _In_ DWRITE_UNDERLINE const* underline, float fOriginX, float fOriginY)
        PURE;
};
}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
