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
// THE SOFTWARE IS PROVIDED "AS IS", WITH_Out_ WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// _Out_ OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <kiwano/core/BinaryData.h>
#include <kiwano/render/DirectX/helper.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{

// Loaders for font files

interface DWRITE_DECLARE_INTERFACE("7EC7A55A-1964-4098-83E0-EFA7C12C6EF7") IFontFileCollectionLoader
    : public IDWriteFontCollectionLoader
{
public:
    static HRESULT Create(_Out_ IFontFileCollectionLoader * *ppCollectionLoader);

    STDMETHOD(AddFilePaths)
    (const Vector<String>& filePaths, _Out_ LPVOID* pCollectionKey, _Out_ uint32_t* pCollectionKeySize) PURE;
};

interface DWRITE_DECLARE_INTERFACE("0A1A3F2A-85F2-41BB-80FD-EC01271740C4") IFontFileEnumerator
    : public IDWriteFontFileEnumerator
{
public:
    static HRESULT Create(_Out_ IFontFileEnumerator * *ppEnumerator, IDWriteFactory * pFactory);

    STDMETHOD(SetFilePaths)(const Vector<String>& filePaths) PURE;
};

// Loaders for font resources

interface DWRITE_DECLARE_INTERFACE("08D21408-6FC1-4E36-A4EB-4DA16BE3399E") IFontResourceLoader
    : public IDWriteFontFileLoader
{
public:
    static HRESULT Create(_Out_ IFontResourceLoader * *ppResLoader);
};

interface DWRITE_DECLARE_INTERFACE("F2C411F0-2FB0-4D0E-8C73-D2B8F30137A4") IFontResourceCollectionLoader
    : public IDWriteFontCollectionLoader
{
public:
    static HRESULT Create(_Out_ IFontResourceCollectionLoader * *ppCollectionLoader, IFontResourceLoader * pResLoader);

    STDMETHOD(AddResources)
    (const Vector<BinaryData>& data, _Out_ LPVOID* pCollectionKey, _Out_ uint32_t* pCollectionKeySize) PURE;
};

interface DWRITE_DECLARE_INTERFACE("0AD0EC74-7503-46E8-8899-520175ECCB4A") IFontResourceEnumerator
    : public IDWriteFontFileEnumerator
{
public:
    static HRESULT Create(_Out_ IFontResourceEnumerator * *ppEnumerator, IDWriteFactory * pFactory,
                          IFontResourceLoader * pResLoader);

    STDMETHOD(SetResources)(const Vector<BinaryData>& data) PURE;
};

interface DWRITE_DECLARE_INTERFACE("A6267450-27F3-4948-995F-FF8345A72F88") IFontResourceStream
    : public IDWriteFontFileStream
{
public:
    static HRESULT Create(_Out_ IFontResourceStream * *ppStream, const BinaryData& data);
};

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
