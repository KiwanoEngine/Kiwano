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

#include "D3D10DeviceResources.h"

#include "../../base/Logger.h"

#pragma comment(lib, "d3d10_1.lib")

namespace kiwano
{

	namespace DX
	{
		HRESULT CreateD3DDevice(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type, std::uint32_t flags, ID3D10Device1 **device)
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

			for (std::uint32_t level = 0; level < ARRAYSIZE(levels); level++)
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

#if defined(KGE_DEBUG)
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
#endif

	} // namespace DX


	struct D3D10DeviceResources
		: public ID3D10DeviceResources
	{
	public:
		HRESULT Present(bool vsync);

		HRESULT ClearRenderTarget(Color& clear_color);

		HRESULT HandleDeviceLost();

		HRESULT SetLogicalSize(Size logical_size);

		HRESULT SetDpi(float dpi);

		void DiscardResources();

	public:
		unsigned long STDMETHODCALLTYPE AddRef();

		unsigned long STDMETHODCALLTYPE Release();

		HRESULT STDMETHODCALLTYPE QueryInterface(
			IID const& riid,
			void** ppvObject
		);

	public:
		D3D10DeviceResources();

		virtual ~D3D10DeviceResources();

		HRESULT CreateDeviceResources();

		HRESULT CreateWindowSizeDependentResources();

	public:
		HWND	hwnd_;
		float	dpi_;
		Size	logical_size_;
		Size	output_size_;
		unsigned long ref_count_;

		ComPtr<ID2DDeviceResources> d2d_res_;
	};


	D3D10DeviceResources::D3D10DeviceResources()
		: ref_count_(0)
		, hwnd_(nullptr)
	{
		dpi_ = 96.f; // dpi_ = (float)GetDpiForWindow(hwnd);
	}

	D3D10DeviceResources::~D3D10DeviceResources()
	{
		DiscardResources();
	}

	HRESULT ID3D10DeviceResources::Create(ID3D10DeviceResources** device_resources, ID2DDeviceResources* d2d_device_res, HWND hwnd)
	{
		HRESULT hr = E_FAIL;

		if (device_resources && d2d_device_res)
		{
			D3D10DeviceResources* res = new (std::nothrow) D3D10DeviceResources;
			if (res)
			{
				RECT rc;
				::GetClientRect(hwnd, &rc);

				res->hwnd_ = hwnd;
				res->d2d_res_ = d2d_device_res;
				res->logical_size_.x = float(rc.right - rc.left);
				res->logical_size_.y = float(rc.bottom - rc.top);

				hr = res->CreateDeviceResources();

				if (SUCCEEDED(hr))
				{
					hr = res->CreateWindowSizeDependentResources();
				}

				if (SUCCEEDED(hr))
				{
					res->AddRef();

					if (*device_resources)
					{
						(*device_resources)->Release();
					}
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

	HRESULT D3D10DeviceResources::Present(bool vsync)
	{
		KGE_ASSERT(dxgi_swap_chain_ != nullptr);

		// The first argument instructs DXGI to block until VSync.
		return dxgi_swap_chain_->Present(vsync ? 1 : 0, 0);
	}

	HRESULT D3D10DeviceResources::ClearRenderTarget(Color& clear_color)
	{
		KGE_ASSERT(device_ != nullptr && rt_view_ != nullptr && ds_view_ != nullptr);

		auto rt_view = rt_view_.get();
		device_->OMSetRenderTargets(1, &rt_view, ds_view_.get());
		device_->ClearRenderTargetView(rt_view, reinterpret_cast<float*>(&clear_color));
		return S_OK;
	}

	void D3D10DeviceResources::DiscardResources()
	{
		d2d_res_.reset();
		device_.reset();
		rt_view_.reset();
		ds_view_.reset();
		dxgi_swap_chain_.reset();
		dxgi_factory_.reset();

		hwnd_ = nullptr;
	}

	HRESULT D3D10DeviceResources::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		std::uint32_t creation_flags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(KGE_DEBUG) && defined(KGE_ENABLE_DX_DEBUG)
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
			device_ = device;

			ComPtr<IDXGIAdapter> dxgi_adapter;
			ComPtr<IDXGIDevice> dxgi_device;
			ComPtr<IDXGIFactory> dxgi_factory;
			ComPtr<ID2D1Device> d2d_device;

			if (SUCCEEDED(hr))
			{
				hr = device_->QueryInterface(IID_PPV_ARGS(&dxgi_device));
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
				hr = d2d_res_->GetFactory()->CreateDevice(dxgi_device.get(), &d2d_device);
			}

			if (SUCCEEDED(hr))
			{
				hr = d2d_res_->SetD2DDevice(d2d_device);
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
				hr = device_->QueryInterface(&dxgi_device);
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
					device_.get(),
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
		device_->OMSetRenderTargets(ARRAYSIZE(null_views), null_views, nullptr);
		d2d_res_->SetTargetBitmap(nullptr);
		rt_view_ = nullptr;
		ds_view_ = nullptr;
		device_->Flush();

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

				rt_view_ = nullptr;
				hr = device_->CreateRenderTargetView(dxgi_back_buffer.get(), &renderDesc, &rt_view_);
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
			tex_desc.Width = static_cast<std::uint32_t>(output_size_.x);
			tex_desc.Height = static_cast<std::uint32_t>(output_size_.y);
			tex_desc.MipLevels = 1;
			tex_desc.MiscFlags = 0;
			tex_desc.SampleDesc.Count = 1;
			tex_desc.SampleDesc.Quality = 0;
			tex_desc.Usage = D3D10_USAGE_DEFAULT;

			hr = device_->CreateTexture2D(&tex_desc, NULL, &depth_stencil);

			if (SUCCEEDED(hr))
			{
				D3D10_DEPTH_STENCIL_VIEW_DESC desc;
				desc.Format = DXGI_FORMAT_D16_UNORM;
				desc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = 0;

				ds_view_.reset();
				hr = device_->CreateDepthStencilView(depth_stencil.get(), &desc, &ds_view_);
			}

			if (SUCCEEDED(hr))
			{
				ID3D10RenderTargetView* main_view = rt_view_.get();
				device_->OMSetRenderTargets(1, &main_view, ds_view_.get());
			}
		}

		if (SUCCEEDED(hr))
		{
			// Set a new viewport based on the new dimensions
			D3D10_VIEWPORT viewport;
			viewport.Width = static_cast<std::uint32_t>(output_size_.x);
			viewport.Height = static_cast<std::uint32_t>(output_size_.y);
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			device_->RSSetViewports(1, &viewport);
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
				hr = d2d_res_->GetDeviceContext()->CreateBitmapFromDxgiSurface(
					dxgi_back_buffer.get(),
					D2D1::BitmapProperties1(
						D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
						D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
						dpi_,
						dpi_),
					&target);
			}

			if (SUCCEEDED(hr))
			{
				d2d_res_->SetTargetBitmap(target);
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

	HRESULT D3D10DeviceResources::SetLogicalSize(Size logical_size)
	{
		if (logical_size_ != logical_size)
		{
			logical_size_ = logical_size;

			return CreateWindowSizeDependentResources();
		}
		return S_OK;
	}

	HRESULT D3D10DeviceResources::SetDpi(float dpi)
	{
		if (dpi != dpi_)
		{
			dpi_ = dpi;

			RECT rc;
			GetClientRect(hwnd_, &rc);

			logical_size_.x = float(rc.right - rc.left);
			logical_size_.y = float(rc.bottom - rc.top);

			d2d_res_->GetDeviceContext()->SetDpi(dpi_, dpi_);

			return CreateWindowSizeDependentResources();
		}
		return S_OK;
	}

	STDMETHODIMP_(unsigned long) D3D10DeviceResources::AddRef()
	{
		return InterlockedIncrement(&ref_count_);
	}

	STDMETHODIMP_(unsigned long) D3D10DeviceResources::Release()
	{
		unsigned long newCount = InterlockedDecrement(&ref_count_);

		if (newCount == 0)
		{
			delete this;
			return 0;
		}

		return newCount;
	}

	STDMETHODIMP D3D10DeviceResources::QueryInterface(IID const& riid, void** object)
	{
		if (__uuidof(ID3D10DeviceResources) == riid)
		{
			*object = this;
		}
		else if (__uuidof(ID3DDeviceResourcesBase) == riid)
		{
			*object = this;
		}
		else if (__uuidof(IUnknown) == riid)
		{
			*object = this;
		}
		else
		{
			*object = nullptr;
			return E_FAIL;
		}

		AddRef();

		return S_OK;
	}

}
