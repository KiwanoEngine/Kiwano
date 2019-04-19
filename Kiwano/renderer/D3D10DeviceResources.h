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
#include <d3d10_1.h>

namespace kiwano
{
	class KGE_API D3D10DeviceResources
		: public D2DDeviceResources
	{
	public:
		static HRESULT Create(
			D3D10DeviceResources** device_resources,
			HWND hwnd
		);

		HRESULT HandleDeviceLost();

		HRESULT SetLogicalSize(
			Size logical_size
		);

		HRESULT SetDpi(
			float dpi
		);

		void DiscardResources();

		inline ID3D10Device*			GetD3DDevice() const			{ return d3d_device_.Get(); }
		inline ID3D10Device*			GetD3DDeviceContext() const		{ return d3d_device_.Get(); }
		inline ID3D10RenderTargetView*	GetD3DRenderTargetView() const	{ return d3d_rt_view_.Get(); }
		inline ID3D10DepthStencilView*	GetD3DDepthStencilView() const	{ return d3d_ds_view_.Get(); }
		inline IDXGIFactory*			GetDXGIFactory() const			{ return dxgi_factory_.Get(); }
		inline IDXGISwapChain*			GetDXGISwapChain() const		{ return dxgi_swap_chain_.Get(); }

		inline Size const&				GetLogicalSize() const			{ return logical_size_; }
		inline Size const&				GetOutputSize() const			{ return output_size_; }
		inline float					GetDpi() const					{ return dpi_; }

	protected:
		D3D10DeviceResources();

		virtual ~D3D10DeviceResources();

	protected:
		HRESULT CreateDeviceResources();

		HRESULT CreateWindowSizeDependentResources();

	private:
		HWND	hwnd_;
		float	dpi_;
		Size	logical_size_;
		Size	output_size_;

		ComPtr<ID3D10Device>			d3d_device_;
		ComPtr<ID3D10RenderTargetView>	d3d_rt_view_;
		ComPtr<ID3D10DepthStencilView>	d3d_ds_view_;
		ComPtr<IDXGISwapChain>			dxgi_swap_chain_;
		ComPtr<IDXGIFactory>			dxgi_factory_;
	};

}

#endif // !KGE_USE_DIRECTX10
