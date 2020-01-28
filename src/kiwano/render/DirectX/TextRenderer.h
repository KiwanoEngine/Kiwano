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

#pragma once
#include <kiwano/render/DirectX/D2DDeviceResources.h>

namespace kiwano
{
interface DWRITE_DECLARE_INTERFACE("b293e798-9916-4096-a3c1-e5d4039dfa64") ITextRenderer : public IDWriteTextRenderer
{
public:
    static KGE_API HRESULT Create(_Out_ ITextRenderer * *ppTextRenderer, _In_ ID2D1RenderTarget * pRT);

    STDMETHOD(DrawTextLayout)
    (_In_ IDWriteTextLayout * pTextLayout, float fOriginX, float fOriginY, _In_opt_ ID2D1Brush* pFillBrush,
     _In_opt_ ID2D1Brush* pOutlineBrush, float fOutlineWidth, _In_opt_ ID2D1StrokeStyle* pStrokeStyle) PURE;

    STDMETHOD_(uint32_t, GetLastPrimitivesCount)() PURE;
};
}  // namespace kiwano
