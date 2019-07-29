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

#include "../macros.h"

#if defined(KGE_USE_DIRECTX10)

#include "D2DDeviceResources.h"
#include "D3DDeviceResourcesBase.h"
#include <d3d10_1.h>

namespace kiwano
{
	MIDL_INTERFACE("3a150b9d-cc23-4022-a463-7e95452a54c4")
	KGE_API ID3D10DeviceResources
		: public ID3DDeviceResourcesBase
	{
	public:
		static HRESULT Create(ID3D10DeviceResources** device_resources, ID2DDeviceResources* d2d_device_res, HWND hwnd);

		inline ID3D10Device*			GetD3DDevice() const			{ KGE_ASSERT(d3d_device_); return d3d_device_.Get(); }
		inline ID3D10RenderTargetView*	GetD3DRenderTargetView() const	{ KGE_ASSERT(d3d_rt_view_); return d3d_rt_view_.Get(); }
		inline ID3D10DepthStencilView*	GetD3DDepthStencilView() const	{ KGE_ASSERT(d3d_ds_view_); return d3d_ds_view_.Get(); }
		inline IDXGIFactory*			GetDXGIFactory() const			{ KGE_ASSERT(dxgi_factory_); return dxgi_factory_.Get(); }
		inline IDXGISwapChain*			GetDXGISwapChain() const		{ KGE_ASSERT(dxgi_swap_chain_); return dxgi_swap_chain_.Get(); }

	protected:
		ComPtr<ID3D10Device>			d3d_device_;
		ComPtr<ID3D10RenderTargetView>	d3d_rt_view_;
		ComPtr<ID3D10DepthStencilView>	d3d_ds_view_;
		ComPtr<IDXGISwapChain>			dxgi_swap_chain_;
		ComPtr<IDXGIFactory>			dxgi_factory_;
	};

}

#endif // !KGE_USE_DIRECTX10
