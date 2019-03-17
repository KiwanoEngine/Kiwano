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

#include "D3D10DeviceResources.h"

#if defined(E2D_USE_DIRECTX10)

#include "../2d/Image.h"
#include "../base/logs.h"

#pragma comment(lib, "d3d10_1.lib")

namespace easy2d
{
#if defined(_DEBUG)
	namespace DX
	{
		HRESULT CreateD3DDevice(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type, UINT flags, ID3D10Device1 **device)
		{
			HRESULT hr = S_OK;

			static const D3D10_FEATURE_LEVEL1 levels[] =
			{
				D3D10_FEATURE_LEVEL_10_1,
				D3D10_FEATURE_LEVEL_10_0,
				D3D10_FEATURE_LEVEL_9_3,
				D3D10_FEATURE_LEVEL_9_2,
				D3D10_FEATURE_LEVEL_9_1,
			};

			for (UINT level = 0; level < ARRAYSIZE(levels); level++)
			{
				hr = D3D10CreateDevice1(
					adapter,
					driver_type,
					NULL,
					flags,
					levels[level],
					D3D10_1_SDK_VERSION,
					device
				);

				if (SUCCEEDED(hr))
					break;
			}
			return hr;
		}

		inline bool SdkLayersAvailable()
		{
			HRESULT hr = CreateD3DDevice(
				NULL,
				D3D10_DRIVER_TYPE_NULL,		// There is no need to create a real hardware device.
				D3D10_CREATE_DEVICE_DEBUG,	// Check for the SDK layers.
				nullptr						// No need to keep the D3D device reference.
			);

			return SUCCEEDED(hr);
		}
	}
#endif

	D3D10DeviceResources::D3D10DeviceResources()
		: hwnd_(nullptr)
	{
		dpi_ = 96.f; // dpi_ = (float)GetDpiForWindow(hwnd);
	}

	D3D10DeviceResources::~D3D10DeviceResources()
	{
		DiscardResources();
	}

	HRESULT D3D10DeviceResources::Create(D3D10DeviceResources** device_resources, HWND hwnd)
	{
		HRESULT hr = E_FAIL;

		if (device_resources)
		{
			D3D10DeviceResources* res = new (std::nothrow) D3D10DeviceResources;
			if (res)
			{
				hr = res->CreateDeviceIndependentResources();

				if (SUCCEEDED(hr))
				{
					RECT rc;
					GetClientRect(hwnd, &rc);

					res->hwnd_ = hwnd;
					res->logical_size_.x = float(rc.right - rc.left);
					res->logical_size_.y = float(rc.bottom - rc.top);

					hr = res->CreateDeviceResources();
				}

				if (SUCCEEDED(hr))
				{
					hr = res->CreateWindowSizeDependentResources();
				}

				if (SUCCEEDED(hr))
				{
					res->AddRef();

					DX::SafeRelease(*device_resources);
					(*device_resources) = res;
				}
				else
				{
					delete res;
					res = nullptr;
				}
			}
		}
		return hr;
	}

	void D3D10DeviceResources::DiscardResources()
	{
		d3d_device_.Reset();
		d3d_rt_view_.Reset();
		d3d_ds_view_.Reset();
		dxgi_swap_chain_.Reset();
		dxgi_factory_.Reset();

		hwnd_ = nullptr;
	}

	HRESULT D3D10DeviceResources::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		UINT creation_flags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(E2D_DEBUG)
		if (DX::SdkLayersAvailable())
		{
			creation_flags |= D3D10_CREATE_DEVICE_DEBUG;
		}
#endif

		ComPtr<ID3D10Device1> device;
		hr = DX::CreateD3DDevice(
			NULL,
			D3D10_DRIVER_TYPE_HARDWARE,
			creation_flags,
			&device
		);

		if (SUCCEEDED(hr))
		{
			d3d_device_ = device;

			ComPtr<IDXGIAdapter> dxgi_adapter;
			ComPtr<IDXGIDevice> dxgi_device;
			ComPtr<IDXGIFactory> dxgi_factory;
			ComPtr<ID2D1Device> d2d_device;

			if (SUCCEEDED(hr))
			{
				hr = d3d_device_->QueryInterface(IID_PPV_ARGS(&dxgi_device));
			}

			if (SUCCEEDED(hr))
			{
				hr = dxgi_device->GetAdapter(&dxgi_adapter);
			}

			if (SUCCEEDED(hr))
			{
				hr = dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));
			}

			if (SUCCEEDED(hr))
			{
				dxgi_factory_ = dxgi_factory;
			}

			// Create the Direct2D device object and a corresponding context.
			if (SUCCEEDED(hr))
			{
				hr = GetD2DFactory()->CreateDevice(dxgi_device.Get(), &d2d_device);
			}

			if (SUCCEEDED(hr))
			{
				hr = SetD2DDevice(d2d_device);
			}
		}

		if (SUCCEEDED(hr))
		{
			// Setup swap chain
			DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };

			swap_chain_desc.BufferCount = 2;
			swap_chain_desc.BufferDesc.Width = ::lround(output_size_.x);
			swap_chain_desc.BufferDesc.Height = ::lround(output_size_.y);
			swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc.OutputWindow = hwnd_;
			swap_chain_desc.SampleDesc.Count = 1;
			swap_chain_desc.SampleDesc.Quality = 0;
			swap_chain_desc.Windowed = TRUE;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			ComPtr<IDXGIDevice> dxgi_device;
			if (SUCCEEDED(hr))
			{
				hr = d3d_device_->QueryInterface(&dxgi_device);
			}

			ComPtr<IDXGIAdapter> dxgi_adapter;
			if (SUCCEEDED(hr))
			{
				hr = dxgi_device->GetAdapter(&dxgi_adapter);
			}

			ComPtr<IDXGIFactory> dxgi_factory;
			if (SUCCEEDED(hr))
			{
				hr = dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));
			}

			if (SUCCEEDED(hr))
			{
				hr = dxgi_factory->CreateSwapChain(
					d3d_device_.Get(),
					&swap_chain_desc,
					&dxgi_swap_chain_);
			}
		}

		return hr;
	}

	HRESULT D3D10DeviceResources::CreateWindowSizeDependentResources()
	{
		if (!dxgi_swap_chain_)
			return E_UNEXPECTED;

		HRESULT hr = S_OK;

		// Clear the previous window size specific context.
		ID3D10RenderTargetView* null_views[] = { nullptr };
		d3d_device_->OMSetRenderTargets(ARRAYSIZE(null_views), null_views, nullptr);
		SetTargetBitmap(nullptr);
		d3d_rt_view_ = nullptr;
		d3d_ds_view_ = nullptr;
		d3d_device_->Flush();

		// Calculate the necessary render target size in pixels.
		output_size_.x = DX::ConvertDipsToPixels(logical_size_.x, dpi_);
		output_size_.y = DX::ConvertDipsToPixels(logical_size_.y, dpi_);

		// Prevent zero size DirectX content from being created.
		output_size_.x = std::max(output_size_.x, 1.f);
		output_size_.y = std::max(output_size_.y, 1.f);

		hr = dxgi_swap_chain_->ResizeBuffers(
			2, // Double-buffered swap chain.
			::lround(output_size_.x),
			::lround(output_size_.y),
			DXGI_FORMAT_UNKNOWN,
			0);

		if (SUCCEEDED(hr))
		{
			// Create the render target view and set it on the device
			ComPtr<ID3D10Resource> dxgi_back_buffer;

			hr = dxgi_swap_chain_->GetBuffer(
				0,
				IID_PPV_ARGS(&dxgi_back_buffer)
			);

			if (SUCCEEDED(hr))
			{
				D3D10_RENDER_TARGET_VIEW_DESC renderDesc;
				renderDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				renderDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
				renderDesc.Texture2D.MipSlice = 0;

				d3d_rt_view_ = nullptr;
				hr = d3d_device_->CreateRenderTargetView(dxgi_back_buffer.Get(), &renderDesc, &d3d_rt_view_);
			}
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<ID3D10Texture2D> depth_stencil;

			D3D10_TEXTURE2D_DESC tex_desc;
			tex_desc.ArraySize = 1;
			tex_desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
			tex_desc.CPUAccessFlags = 0;
			tex_desc.Format = DXGI_FORMAT_D16_UNORM;
			tex_desc.Width = static_cast<UINT>(output_size_.x);
			tex_desc.Height = static_cast<UINT>(output_size_.y);
			tex_desc.MipLevels = 1;
			tex_desc.MiscFlags = 0;
			tex_desc.SampleDesc.Count = 1;
			tex_desc.SampleDesc.Quality = 0;
			tex_desc.Usage = D3D10_USAGE_DEFAULT;

			hr = d3d_device_->CreateTexture2D(&tex_desc, NULL, &depth_stencil);

			if (SUCCEEDED(hr))
			{
				D3D10_DEPTH_STENCIL_VIEW_DESC desc;
				desc.Format = DXGI_FORMAT_D16_UNORM;
				desc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = 0;

				d3d_ds_view_.Reset();
				hr = d3d_device_->CreateDepthStencilView(depth_stencil.Get(), &desc, &d3d_ds_view_);
			}

			if (SUCCEEDED(hr))
			{
				ID3D10RenderTargetView* main_view = d3d_rt_view_.Get();
				d3d_device_context_->OMSetRenderTargets(1, &main_view, d3d_ds_view_.Get());
			}
		}

		if (SUCCEEDED(hr))
		{
			// Set a new viewport based on the new dimensions
			D3D10_VIEWPORT viewport;
			viewport.Width = static_cast<UINT>(output_size_.x);
			viewport.Height = static_cast<UINT>(output_size_.y);
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			d3d_device_->RSSetViewports(1, &viewport);
		}

		// Create a Direct2D target bitmap associated with the
		// swap chain back buffer and set it as the current target.
		if (SUCCEEDED(hr))
		{
			ComPtr<IDXGISurface> dxgi_back_buffer;
			hr = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer));

			ComPtr<ID2D1Bitmap1> target;
			if (SUCCEEDED(hr))
			{
				hr = GetD2DDeviceContext()->CreateBitmapFromDxgiSurface(
					dxgi_back_buffer.Get(),
					D2D1::BitmapProperties1(
						D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
						D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
						dpi_,
						dpi_),
					&target);
			}

			if (SUCCEEDED(hr))
			{
				SetTargetBitmap(target);
			}
		}

		return hr;
	}

	HRESULT D3D10DeviceResources::HandleDeviceLost()
	{
		dxgi_swap_chain_ = nullptr;

		HRESULT hr = CreateDeviceResources();

		if (SUCCEEDED(hr))
		{
			hr = CreateWindowSizeDependentResources();
		}
		return hr;
	}

	void D3D10DeviceResources::SetLogicalSize(Size logical_size)
	{
		if (logical_size_ != logical_size)
		{
			logical_size_ = logical_size;

			ThrowIfFailed(
				CreateWindowSizeDependentResources()
			);
		}
	}

	void D3D10DeviceResources::SetDpi(float dpi)
	{
		if (dpi != dpi_)
		{
			dpi_ = dpi;

			RECT rc;
			GetClientRect(hwnd_, &rc);

			logical_size_.x = float(rc.right - rc.left);
			logical_size_.y = float(rc.bottom - rc.top);

			GetD2DDeviceContext()->SetDpi(dpi_, dpi_);

			ThrowIfFailed(
				CreateWindowSizeDependentResources()
			);
		}
	}

}

#endif // !E2D_USE_DIRECTX10
