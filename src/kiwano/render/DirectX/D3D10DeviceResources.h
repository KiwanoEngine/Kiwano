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
#include <kiwano/macros.h>
#include <kiwano/render/DirectX/helper.h>
#include <kiwano/render/DirectX/D3DDeviceResourcesBase.h>
#include <d3d10_1.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{

MIDL_INTERFACE("3a150b9d-cc23-4022-a463-7e95452a54c4")
KGE_API ID3D10DeviceResources : public ID3DDeviceResourcesBase
{
public:
    inline ID3D10Device* GetDevice()
    {
        KGE_ASSERT(device_);
        return device_.Get();
    }

    inline ID3D10RenderTargetView* GetRenderTargetView()
    {
        KGE_ASSERT(rt_view_);
        return rt_view_.Get();
    }

    inline ID3D10DepthStencilView* GetDepthStencilView()
    {
        KGE_ASSERT(ds_view_);
        return ds_view_.Get();
    }

    inline IDXGIFactory* GetDXGIFactory()
    {
        KGE_ASSERT(dxgi_factory_);
        return dxgi_factory_.Get();
    }

    inline IDXGIDevice* GetDXGIDevice()
    {
        KGE_ASSERT(dxgi_device_);
        return dxgi_device_.Get();
    }

    inline IDXGISwapChain* GetDXGISwapChain()
    {
        KGE_ASSERT(dxgi_swap_chain_);
        return dxgi_swap_chain_.Get();
    }

protected:
    ComPtr<ID3D10Device>           device_;
    ComPtr<ID3D10RenderTargetView> rt_view_;
    ComPtr<ID3D10DepthStencilView> ds_view_;
    ComPtr<IDXGIFactory>           dxgi_factory_;
    ComPtr<IDXGIDevice>            dxgi_device_;
    ComPtr<IDXGISwapChain>         dxgi_swap_chain_;
};

extern ComPtr<ID3D10DeviceResources> GetD3D11DeviceResources();

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
