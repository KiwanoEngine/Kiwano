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
#include <d3d11.h>

namespace kiwano
{
MIDL_INTERFACE("3ede2b87-a202-4799-a39b-2308ad34cae8")
KGE_API ID3D11DeviceResources : public ID3DDeviceResourcesBase
{
public:
    static HRESULT Create(ID3D11DeviceResources * *device_resources, HWND hwnd);

    inline ID3D11Device* GetDevice()
    {
        KGE_ASSERT(device_);
        return device_.get();
    }
    inline ID3D11DeviceContext* GetDeviceContext()
    {
        KGE_ASSERT(device_context_);
        return device_context_.get();
    }
    inline ID3D11RenderTargetView* GetRenderTargetView()
    {
        KGE_ASSERT(rt_view_);
        return rt_view_.get();
    }
    inline ID3D11DepthStencilView* GetDepthStencilView()
    {
        KGE_ASSERT(ds_view_);
        return ds_view_.get();
    }
    inline IDXGIFactory* GetDXGIFactory()
    {
        KGE_ASSERT(dxgi_factory_);
        return dxgi_factory_.get();
    }
    inline IDXGIDevice* GetDXGIDevice()
    {
        KGE_ASSERT(dxgi_device_);
        return dxgi_device_.get();
    }
    inline IDXGISwapChain* GetDXGISwapChain()
    {
        KGE_ASSERT(dxgi_swap_chain_);
        return dxgi_swap_chain_.get();
    }

protected:
    ComPtr<ID3D11Device>           device_;
    ComPtr<ID3D11DeviceContext>    device_context_;
    ComPtr<ID3D11RenderTargetView> rt_view_;
    ComPtr<ID3D11DepthStencilView> ds_view_;
    ComPtr<IDXGIFactory>           dxgi_factory_;
    ComPtr<IDXGIDevice>            dxgi_device_;
    ComPtr<IDXGISwapChain>         dxgi_swap_chain_;
};

}  // namespace kiwano
