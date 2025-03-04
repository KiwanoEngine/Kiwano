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
#include <Unknwnbase.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{

MIDL_INTERFACE("fb99fa64-d9cf-4e0e-9c75-90514797b01d")
ID3DDeviceResourcesBase : public IUnknown
{
public:
    virtual HRESULT Initialize(HWND hwnd, Size logical_size, float dpi) = 0;

    virtual HRESULT Present(bool vsync) = 0;

    virtual void ClearRenderTarget(Color & clear_color) = 0;

    virtual HRESULT HandleDeviceLost() = 0;

    virtual HRESULT SetLogicalSize(Size logical_size) = 0;

    virtual HRESULT SetFullscreenState(bool fullscreen) = 0;

    virtual HRESULT GetFullscreenState(bool* fullscreen) = 0;

    virtual HRESULT ResizeTarget(UINT width, UINT height) = 0;

    virtual HRESULT GetDisplaySettings(DXGI_MODE_DESC * *mode_descs, int* num) = 0;

    virtual void DiscardResources() = 0;
};

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
