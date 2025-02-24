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

#include <kiwano/render/DirectX/Effect.h>
#include <kiwano/core/UUID.h>

#define XML(X) TEXT(#X)

namespace kiwano
{
namespace graphics
{
namespace directx
{
namespace
{
CLSID       g_currentShaderId       = { 0 };
const BYTE* g_currentShaderData     = nullptr;
UINT32      g_currentShaderDataSize = 0;
}  // namespace

CustomPixelEffect::CustomPixelEffect()
    : m_refCount(0)
    , m_shaderId{ 0 }
    , m_constants(nullptr)
    , m_constantsSize(0)
    , m_leftTopExpansion{ 0 }
    , m_rightBottomExpansion{ 0 }
    , m_inputRect{ 0 }
{
}

CustomPixelEffect::~CustomPixelEffect()
{
    m_drawInfo      = nullptr;
    m_effectContext = nullptr;
}

IFACEMETHODIMP_(HRESULT) CustomPixelEffect::Register(_In_ ID2D1Factory1* pFactory)
{
    PCWSTR pszXml =
        XML(
        <?xml version='1.0'?>
        <Effect>
            <Property name='DisplayName' type='string' value='CustomPixelEffect'/>
            <Property name='Author' type='string' value='Kiwano'/>
            <Property name='Category' type='string' value='Common'/>
            <Property name='Description' type='string' value='This is a common effect.'/>
            <Inputs>
                <Input name='Source'/>
            </Inputs>
            <Property name='Constants' type='blob'>
                <Property name='DisplayName' type='string' value='Constants'/>
            </Property>
            <Property name='LeftTopExpansion' type='vector2'>
                <Property name='DisplayName' type='string' value='LeftTopExpansion'/>
            </Property>
            <Property name='RightBottomExpansion' type='vector2'>
                <Property name='DisplayName' type='string' value='RightBottomExpansion'/>
            </Property>
        </Effect>
        );

    const D2D1_PROPERTY_BINDING bindings[] = {
        D2D1_BLOB_TYPE_BINDING(TEXT("Constants"), &SetConstants, &GetConstants),
        D2D1_VALUE_TYPE_BINDING(TEXT("LeftTopExpansion"), &SetLeftTopExpansion, &GetLeftTopExpansion),
        D2D1_VALUE_TYPE_BINDING(TEXT("RightBottomExpansion"), &SetRightBottomExpansion, &GetRightBottomExpansion),
    };

    return pFactory->RegisterEffectFromString(CLSID_CustomPixelEffect, pszXml, bindings, ARRAYSIZE(bindings),
                                              CustomPixelEffect::CreateEffect);
}

IFACEMETHODIMP_(HRESULT) CustomPixelEffect::CreateEffect(_Outptr_ IUnknown** ppEffectImpl)
{
    *ppEffectImpl = static_cast<ID2D1EffectImpl*>(new CustomPixelEffect());
    if (*ppEffectImpl == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    (*ppEffectImpl)->AddRef();
    return S_OK;
}

void CustomPixelEffect::RegisterShader(_In_ const CLSID& shaderId, _In_ const BYTE* data, _In_ UINT32 dataSize)
{
    g_currentShaderId       = shaderId;
    g_currentShaderData     = data;
    g_currentShaderDataSize = dataSize;
}

IFACEMETHODIMP CustomPixelEffect::Initialize(_In_ ID2D1EffectContext*  pEffectContext,
                                             _In_ ID2D1TransformGraph* pTransformGraph)
{
    m_effectContext = pEffectContext;
    m_shaderId      = g_currentShaderId;

    HRESULT hr = pEffectContext->LoadPixelShader(g_currentShaderId, g_currentShaderData, g_currentShaderDataSize);
    if (SUCCEEDED(hr))
    {
        // The graph consists of a single transform.
        hr = pTransformGraph->SetSingleTransformNode(this);
    }
    return hr;
}

IFACEMETHODIMP CustomPixelEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType)
{
    return m_drawInfo->SetPixelShaderConstantBuffer(m_constants, m_constantsSize);
}

// SetGraph is only called when the number of inputs changes. This never happens as we publish this effect
// as a single input effect.
IFACEMETHODIMP CustomPixelEffect::SetGraph(_In_ ID2D1TransformGraph* pGraph)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CustomPixelEffect::SetDrawInfo(_In_ ID2D1DrawInfo* pDrawInfo)
{
    m_drawInfo = pDrawInfo;
    return m_drawInfo->SetPixelShader(m_shaderId);
}

// Calculates the mapping between the output and input rects.
IFACEMETHODIMP CustomPixelEffect::MapOutputRectToInputRects(_In_ const D2D1_RECT_L*                   pOutputRect,
                                                            _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
                                                            UINT32 inputRectCount) const
{
    // This effect has exactly one input
    if (inputRectCount != 1)
    {
        return E_INVALIDARG;
    }

    pInputRects[0].left   = pOutputRect->left, -LONG(m_leftTopExpansion.x);
    pInputRects[0].top    = pOutputRect->top, -LONG(m_leftTopExpansion.y);
    pInputRects[0].right  = pOutputRect->right, LONG(m_rightBottomExpansion.x);
    pInputRects[0].bottom = pOutputRect->bottom, LONG(m_rightBottomExpansion.y);
    return S_OK;
}

IFACEMETHODIMP CustomPixelEffect::MapInputRectsToOutputRect(_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,
                                                            _In_reads_(inputRectCount)
                                                                CONST D2D1_RECT_L* pInputOpaqueSubRects,
                                                            UINT32 inputRectCount, _Out_ D2D1_RECT_L* pOutputRect,
                                                            _Out_ D2D1_RECT_L* pOutputOpaqueSubRect)
{
    // This effect has exactly one input
    if (inputRectCount != 1)
    {
        return E_INVALIDARG;
    }

    *pOutputRect = pInputRects[0];
    m_inputRect  = pInputRects[0];

    // Indicate that entire output might contain transparency.
    ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));
    return S_OK;
}

IFACEMETHODIMP CustomPixelEffect::MapInvalidRect(UINT32 inputIndex, D2D1_RECT_L invalidInputRect,
                                                 _Out_ D2D1_RECT_L* pInvalidOutputRect) const
{
    HRESULT hr = S_OK;

    // Indicate that the entire output may be invalid.
    *pInvalidOutputRect = m_inputRect;

    return hr;
}

IFACEMETHODIMP_(UINT32) CustomPixelEffect::GetInputCount() const
{
    return 1;
}

HRESULT CustomPixelEffect::SetConstants(_In_reads_(dataSize) const BYTE* data, UINT32 dataSize)
{
    m_constants     = data;
    m_constantsSize = dataSize;
    return S_OK;
}

HRESULT CustomPixelEffect::GetConstants(_Out_writes_opt_(dataSize) BYTE* data, UINT32 dataSize,
                                        _Out_opt_ UINT32* actualSize) const
{
    if (actualSize)
    {
        *actualSize = m_constantsSize;
    }
    if (data)
    {
        std::memcpy(data, m_constants, m_constantsSize);
    }
    return S_OK;
}

HRESULT CustomPixelEffect::SetLeftTopExpansion(D2D1_POINT_2F value)
{
    m_leftTopExpansion = value;
    return S_OK;
}

D2D1_POINT_2F CustomPixelEffect::GetLeftTopExpansion() const
{
    return m_leftTopExpansion;
}

HRESULT CustomPixelEffect::SetRightBottomExpansion(D2D1_POINT_2F value)
{
    m_rightBottomExpansion = value;
    return S_OK;
}

D2D1_POINT_2F CustomPixelEffect::GetRightBottomExpansion() const
{
    return m_rightBottomExpansion;
}

IFACEMETHODIMP_(unsigned long) CustomPixelEffect::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

IFACEMETHODIMP_(unsigned long) CustomPixelEffect::Release()
{
    unsigned long newCount = InterlockedDecrement(&m_refCount);

    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

IFACEMETHODIMP CustomPixelEffect::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppvObject)
{
    if (riid == __uuidof(ID2D1EffectImpl))
    {
        *ppvObject = reinterpret_cast<ID2D1EffectImpl*>(this);
    }
    else if (riid == __uuidof(ID2D1DrawTransform))
    {
        *ppvObject = static_cast<ID2D1DrawTransform*>(this);
    }
    else if (riid == __uuidof(ID2D1Transform))
    {
        *ppvObject = static_cast<ID2D1Transform*>(this);
    }
    else if (riid == __uuidof(ID2D1TransformNode))
    {
        *ppvObject = static_cast<ID2D1TransformNode*>(this);
    }
    else if (riid == __uuidof(IUnknown))
    {
        *ppvObject = this;
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
