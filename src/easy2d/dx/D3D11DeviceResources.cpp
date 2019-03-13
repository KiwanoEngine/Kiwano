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

#include "D3D11DeviceResources.h"

#if !defined(E2D_USE_DIRECTX10)

#include "../core/Image.h"
#include "../core/logs.h"

#pragma comment(lib, "d3d11.lib")

namespace easy2d
{
#if defined(_DEBUG)
	namespace DX
	{
		inline bool SdkLayersAvailable()
		{
			HRESULT hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_NULL,		// There is no need to create a real hardware device.
				0,
				D3D11_CREATE_DEVICE_DEBUG,	// Check for the SDK layers.
				nullptr,					// Any feature level will do.
				0,
				D3D11_SDK_VERSION,
				nullptr,					// No need to keep the D3D device reference.
				nullptr,					// No need to know the feature level.
				nullptr						// No need to keep the D3D device context reference.
			);

			return SUCCEEDED(hr);
		}
	}
#endif

	D3D11DeviceResources::D3D11DeviceResources()
		: hwnd_(nullptr)
		, d3d_feature_level_(D3D_FEATURE_LEVEL_9_1)
	{
		dpi_ = 96.f; // dpi_ = (float)GetDpiForWindow(hwnd);
	}

	D3D11DeviceResources::~D3D11DeviceResources()
	{
		DiscardResources();
	}

	HRESULT D3D11DeviceResources::Create(D3D11DeviceResources** device_resources, HWND hwnd)
	{
		HRESULT hr = E_FAIL;

		if (device_resources)
		{
			D3D11DeviceResources* res = new (std::nothrow) D3D11DeviceResources;
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

	void D3D11DeviceResources::DiscardResources()
	{
		d3d_device_.Reset();
		d3d_device_context_.Reset();
		d3d_rt_view_.Reset();
		d3d_ds_view_.Reset();
		dxgi_swap_chain_.Reset();
		dxgi_factory_.Reset();

		hwnd_ = nullptr;
	}

	HRESULT D3D11DeviceResources::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		static const D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(E2D_DEBUG)
		if (DX::SdkLayersAvailable())
		{
			creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		hr = D3D11CreateDevice(
			nullptr,					// Specify nullptr to use the default adapter.
			D3D_DRIVER_TYPE_HARDWARE,
			0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
			creation_flags,				// Set debug and Direct2D compatibility flags.
			feature_levels,				// List of feature levels this app can support.
			ARRAYSIZE(feature_levels),	// Size of the list above.
			D3D11_SDK_VERSION,
			&device,					// Returns the Direct3D device created.
			&d3d_feature_level_,		// Returns feature level of device created.
			&context					// Returns the device immediate context.
		);

		if (FAILED(hr))
		{
			// If the initialization fails, fall back to the WARP device.
			// For more information on WARP, see: 
			// http://go.microsoft.com/fwlink/?LinkId=286690
			hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
				0,
				creation_flags,
				feature_levels,
				ARRAYSIZE(feature_levels),
				D3D11_SDK_VERSION,
				&device,
				&d3d_feature_level_,
				&context
			);
		}

		if (SUCCEEDED(hr))
		{
			d3d_device_ = device;
			d3d_device_context_ = context;

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
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

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

	HRESULT D3D11DeviceResources::CreateWindowSizeDependentResources()
	{
		if (!dxgi_swap_chain_)
			return E_UNEXPECTED;

		HRESULT hr = S_OK;

		// Clear the previous window size specific context.
		ID3D11RenderTargetView* null_views[] = { nullptr };
		d3d_device_context_->OMSetRenderTargets(ARRAYSIZE(null_views), null_views, nullptr);
		SetTargetBitmap(nullptr);
		d3d_rt_view_ = nullptr;
		d3d_ds_view_ = nullptr;
		d3d_device_context_->Flush();

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
			ComPtr<ID3D11Resource> dxgi_back_buffer;

			hr = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer));

			if (SUCCEEDED(hr))
			{
				d3d_rt_view_ = nullptr;
				hr = d3d_device_->CreateRenderTargetView(dxgi_back_buffer.Get(), nullptr, &d3d_rt_view_);
			}
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<ID3D11Texture2D> depth_stencil;

			CD3D11_TEXTURE2D_DESC tex_desc(
				DXGI_FORMAT_D24_UNORM_S8_UINT,
				static_cast<UINT>(output_size_.x),
				static_cast<UINT>(output_size_.y),
				1, // This depth stencil view has only one texture.
				1, // Use a single mipmap level.
				D3D11_BIND_DEPTH_STENCIL
			);

			hr = d3d_device_->CreateTexture2D(&tex_desc, nullptr, &depth_stencil);

			if (SUCCEEDED(hr))
			{
				CD3D11_DEPTH_STENCIL_VIEW_DESC desc(D3D11_DSV_DIMENSION_TEXTURE2D);

				d3d_ds_view_.Reset();
				hr = d3d_device_->CreateDepthStencilView(depth_stencil.Get(), &desc, &d3d_ds_view_);
			}

			if (SUCCEEDED(hr))
			{
				ID3D11RenderTargetView* main_view = d3d_rt_view_.Get();
				d3d_device_context_->OMSetRenderTargets(1, &main_view, d3d_ds_view_.Get());
			}
		}

		if (SUCCEEDED(hr))
		{
			// Set the 3D rendering viewport to target the entire window.
			CD3D11_VIEWPORT screen_viewport(
				0.0f,
				0.0f,
				output_size_.x,
				output_size_.y);

			d3d_device_context_->RSSetViewports(1, &screen_viewport);
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

	HRESULT D3D11DeviceResources::HandleDeviceLost()
	{
		dxgi_swap_chain_ = nullptr;

		HRESULT hr = CreateDeviceResources();

		if (SUCCEEDED(hr))
		{
			hr = CreateWindowSizeDependentResources();
		}
		return hr;
	}

	HRESULT D3D11DeviceResources::SetLogicalSize(Size logical_size)
	{
		if (logical_size_ != logical_size)
		{
			logical_size_ = logical_size;

			return CreateWindowSizeDependentResources();
		}
		return S_OK;
	}

	HRESULT D3D11DeviceResources::SetDpi(float dpi)
	{
		if (dpi != dpi_)
		{
			dpi_ = dpi;

			RECT rc;
			GetClientRect(hwnd_, &rc);

			logical_size_.x = float(rc.right - rc.left);
			logical_size_.y = float(rc.bottom - rc.top);

			GetD2DDeviceContext()->SetDpi(dpi_, dpi_);

			return CreateWindowSizeDependentResources();
		}
		return S_OK;
	}

}

#endif // !E2D_USE_DIRECTX10
