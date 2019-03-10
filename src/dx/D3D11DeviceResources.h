// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "helper.hpp"
#include "D2DDeviceResources.h"
#include <d3d11.h>

namespace easy2d
{
	class E2D_API D3D11DeviceResources
		: public D2DDeviceResources
	{
	public:
		static HRESULT Create(
			D3D11DeviceResources** device_resources,
			HWND hwnd
		);

		HRESULT HandleDeviceLost();

		void SetLogicalSize(
			Size logical_size
		);

		void SetDpi(
			float dpi
		);

		void DiscardResources();

		inline ID3D11Device*			GetD3DDevice() const			{ return d3d_device_.Get(); }
		inline ID3D11DeviceContext*		GetD3DDeviceContext() const		{ return d3d_device_context_.Get(); }
		inline IDXGIFactory*			GetDXGIFactory() const			{ return dxgi_factory_.Get(); }
		inline IDXGISwapChain*			GetDXGISwapChain() const		{ return dxgi_swap_chain_.Get(); }

		inline D3D_FEATURE_LEVEL		GetDeviceFeatureLevel() const	{ return d3d_feature_level_; }
		inline Size const&				GetLogicalSize() const			{ return logical_size_; }
		inline Size const&				GetOutputSize() const			{ return output_size_; }
		inline float					GetDpi() const					{ return dpi_; }

	protected:
		D3D11DeviceResources();

		virtual ~D3D11DeviceResources();

	protected:
		HRESULT CreateDeviceResources();

		HRESULT CreateWindowSizeDependentResources();

	private:
		HWND hwnd_;
		float dpi_;
		Size logical_size_;
		Size output_size_;
		D3D_FEATURE_LEVEL d3d_feature_level_;

		ComPtr<ID3D11Device>		d3d_device_;
		ComPtr<ID3D11DeviceContext>	d3d_device_context_;
		ComPtr<IDXGISwapChain>		dxgi_swap_chain_;
		ComPtr<IDXGIFactory>		dxgi_factory_;
	};

}
