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
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <initguid.h>

DEFINE_GUID(CLSID_CustomPixelEffect, 0x6b0e1068, 0x186e, 0x43f7, 0xbb, 0x5, 0x64, 0x91, 0x9, 0xa5, 0x6a, 0x79);

namespace kiwano
{
namespace graphics
{
namespace directx
{

class CustomPixelEffect
    : public ID2D1EffectImpl
    , public ID2D1DrawTransform
{
public:
    enum InputIndex : UINT32
    {
        Constants = 0,
        LeftTopExpansion,
        RightBottomExpansion,
    };

    static HRESULT Register(_In_ ID2D1Factory1* pFactory);
    static HRESULT CreateEffect(_Outptr_ IUnknown** ppEffectImpl);

    static void RegisterShader(_In_ const CLSID& shaderId, _In_ const BYTE* data, _In_ UINT32 dataSize);

    IFACEMETHODIMP Initialize(_In_ ID2D1EffectContext* pContextInternal, _In_ ID2D1TransformGraph* pTransformGraph);

    IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);
    IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);

    // Declare ID2D1DrawTransform implementation methods.
    IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo);

    // Declare ID2D1Transform implementation methods.
    IFACEMETHODIMP MapOutputRectToInputRects(_In_ const D2D1_RECT_L*                   pOutputRect,
                                             _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
                                             UINT32                                    inputRectCount) const;

    IFACEMETHODIMP MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,
                                             _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects,
                                             UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect,
                                             _Out_ D2D1_RECT_L* pOutputOpaqueSubRect);

    IFACEMETHODIMP MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect,
                                  _Out_ D2D1_RECT_L* pInvalidOutputRect) const;

    // Declare ID2D1TransformNode implementation methods.
    IFACEMETHODIMP_(UINT32) GetInputCount() const;

    HRESULT SetConstants(_In_reads_(dataSize) const BYTE* data, UINT32 dataSize);
    HRESULT GetConstants(_Out_writes_opt_(dataSize) BYTE* data, UINT32 dataSize, _Out_opt_ UINT32* actualSize) const;

    HRESULT       SetLeftTopExpansion(D2D1_POINT_2F value);
    D2D1_POINT_2F GetLeftTopExpansion() const;

    HRESULT       SetRightBottomExpansion(D2D1_POINT_2F value);
    D2D1_POINT_2F GetRightBottomExpansion() const;

    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
    IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

private:
    CustomPixelEffect();

    ~CustomPixelEffect();

    LONG                       m_refCount;
    ComPtr<ID2D1DrawInfo>      m_drawInfo;
    ComPtr<ID2D1EffectContext> m_effectContext;
    D2D1_RECT_L                m_inputRect;
    CLSID                      m_shaderId;
    const BYTE*                m_constants;
    UINT32                     m_constantsSize;
    D2D1_POINT_2F              m_leftTopExpansion;
    D2D1_POINT_2F              m_rightBottomExpansion;
};

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
