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

#include <kiwano/utils/Logger.h>
#include <kiwano/render/DirectX/D3D11DeviceResources.h>

KGE_SUPPRESS_WARNING_PUSH
KGE_SUPPRESS_WARNING(4800)   // Implicit conversion from 'type' to bool
#include <versionhelpers.h>  // IsWindows10OrGreater
KGE_SUPPRESS_WARNING_POP

#pragma comment(lib, "d3d11.lib")

namespace kiwano
{
namespace graphics
{
namespace directx
{

// Global pointer for Direct3D11 device resources
static ComPtr<ID3D11DeviceResources> global_d3d11_device_resources;

#if defined(KGE_DEBUG)
inline bool SdkLayersAvailable()
{
    HRESULT hr = D3D11CreateDevice(nullptr,
                                   D3D_DRIVER_TYPE_NULL,  // There is no need to create a real hardware device.
                                   0,
                                   D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
                                   nullptr,                    // Any feature level will do.
                                   0, D3D11_SDK_VERSION,
                                   nullptr,  // No need to keep the D3D device reference.
                                   nullptr,  // No need to know the feature level.
                                   nullptr   // No need to keep the D3D device context reference.
    );
    return SUCCEEDED(hr);
}
#endif

struct D3D11DeviceResources : public ID3D11DeviceResources
{
public:
    HRESULT Initialize(HWND hwnd, Size logical_size, float dpi) override;

    HRESULT Present(bool vsync) override;

    void ClearRenderTarget(Color& clear_color) override;

    HRESULT HandleDeviceLost() override;

    HRESULT SetLogicalSize(Size logical_size) override;

    HRESULT SetFullscreenState(bool fullscreen) override;

    HRESULT GetFullscreenState(bool* fullscreen) override;

    HRESULT ResizeTarget(UINT width, UINT height) override;

    HRESULT GetDisplaySettings(DXGI_MODE_DESC** mode_descs, int* num) override;

    void DiscardResources() override;

public:
    unsigned long STDMETHODCALLTYPE AddRef();

    unsigned long STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void** ppvObject);

public:
    D3D11DeviceResources();

    virtual ~D3D11DeviceResources();

    HRESULT CreateDeviceResources();

    HRESULT CreateWindowSizeDependentResources();

public:
    unsigned long     ref_count_;
    HWND              hwnd_;
    D3D_FEATURE_LEVEL d3d_feature_level_;
    DXGI_FORMAT       desired_color_format_;
    float             dpi_;
    Size              dip_size_;
    math::Vec2T<UINT> output_size_;
};

ComPtr<ID3D11DeviceResources> GetD3D11DeviceResources()
{
    if (!global_d3d11_device_resources)
    {
        global_d3d11_device_resources.Reset(new (std::nothrow) D3D11DeviceResources);
    }
    return global_d3d11_device_resources;
}

D3D11DeviceResources::D3D11DeviceResources()
    : ref_count_(0)
    , hwnd_(nullptr)
    , dpi_(96.f)  // GetDpiForWindow(hwnd)
    , d3d_feature_level_(D3D_FEATURE_LEVEL_9_1)
    , desired_color_format_(DXGI_FORMAT_B8G8R8A8_UNORM)
{
}

D3D11DeviceResources::~D3D11DeviceResources()
{
    DiscardResources();
}

HRESULT D3D11DeviceResources::Initialize(HWND hwnd, Size logical_size, float dpi)
{
    this->hwnd_     = hwnd;
    this->dpi_      = dpi;
    this->dip_size_ = logical_size;

    HRESULT hr = this->CreateDeviceResources();

    if (SUCCEEDED(hr))
    {
        hr = this->CreateWindowSizeDependentResources();
    }
    return hr;
}

HRESULT D3D11DeviceResources::Present(bool vsync)
{
    HRESULT hr = E_FAIL;

    if (dxgi_swap_chain_)
    {
        // The first argument instructs DXGI to block until VSync.
        hr = dxgi_swap_chain_->Present(vsync ? 1 : 0, vsync ? 0 : DXGI_PRESENT_DO_NOT_WAIT);
    }

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    device_context_->DiscardView(rt_view_.Get());

    // Discard the contents of the depth stencil.
    device_context_->DiscardView(ds_view_.Get());
    return hr;
}

void D3D11DeviceResources::ClearRenderTarget(Color& clear_color)
{
    KGE_ASSERT(device_context_ != nullptr && rt_view_ != nullptr && ds_view_ != nullptr);

    auto rt_view = rt_view_.Get();
    device_context_->OMSetRenderTargets(1, &rt_view, ds_view_.Get());
    device_context_->ClearRenderTargetView(rt_view, reinterpret_cast<float*>(&clear_color));
}

void D3D11DeviceResources::DiscardResources()
{
    device_.Reset();
    device_context_.Reset();
    rt_view_.Reset();
    ds_view_.Reset();
    dxgi_device_.Reset();
    dxgi_swap_chain_.Reset();
    dxgi_factory_.Reset();

    hwnd_ = nullptr;
}

HRESULT D3D11DeviceResources::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    static const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
                                                        D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
                                                        D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_9_2,
                                                        D3D_FEATURE_LEVEL_9_1 };

    ComPtr<ID3D11Device>        device;
    ComPtr<ID3D11DeviceContext> context;

    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    uint32_t creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(KGE_DEBUG) && defined(KGE_ENABLE_DX_DEBUG)
    if (SdkLayersAvailable())
    {
        creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
    }
#endif

    hr = D3D11CreateDevice(nullptr,  // Specify nullptr to use the default adapter.
                           D3D_DRIVER_TYPE_HARDWARE,
                           0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
                           creation_flags,             // Set debug and Direct2D compatibility flags.
                           feature_levels,             // List of feature levels this app can support.
                           ARRAYSIZE(feature_levels),  // Size of the list above.
                           D3D11_SDK_VERSION,
                           &device,              // Returns the Direct3D device created.
                           &d3d_feature_level_,  // Returns feature level of device created.
                           &context              // Returns the device immediate context.
    );

    if (FAILED(hr))
    {
        // If the initialization fails, fall back to the WARP device.
        // For more information on WARP, see:
        // http://go.microsoft.com/fwlink/?LinkId=286690
        hr = D3D11CreateDevice(nullptr,
                               D3D_DRIVER_TYPE_WARP,  // Create a WARP device instead of a hardware device.
                               0, creation_flags, feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &device,
                               &d3d_feature_level_, &context);
    }

    if (SUCCEEDED(hr))
    {
        hr = device->QueryInterface(IID_PPV_ARGS(&device_));
    }

    if (SUCCEEDED(hr))
    {
        hr = context->QueryInterface(IID_PPV_ARGS(&device_context_));
    }

    if (SUCCEEDED(hr))
    {
        if (SUCCEEDED(hr))
        {
            ComPtr<IDXGIDevice3> dxgi_device;
            hr = device_->QueryInterface(IID_PPV_ARGS(&dxgi_device));

            if (SUCCEEDED(hr))
            {
                dxgi_device_ = dxgi_device;
            }
        }

        if (SUCCEEDED(hr))
        {
            ComPtr<IDXGIAdapter> dxgi_adapter;
            hr = dxgi_device_->GetAdapter(&dxgi_adapter);

            if (SUCCEEDED(hr))
            {
                ComPtr<IDXGIFactory2> dxgi_factory;
                hr = dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));

                if (SUCCEEDED(hr))
                {
                    dxgi_factory_ = dxgi_factory;
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = dxgi_factory_->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER);
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = { 0 };

        swap_chain_desc.Width              = output_size_.x;
        swap_chain_desc.Height             = output_size_.y;
        swap_chain_desc.Format             = desired_color_format_;
        swap_chain_desc.Stereo             = false;
        swap_chain_desc.SampleDesc.Count   = 1;  // Don't use multi-sampling.
        swap_chain_desc.SampleDesc.Quality = 0;
        swap_chain_desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferCount        = 2;  // Use double-buffering to minimize latency.
        swap_chain_desc.Flags              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.Scaling            = DXGI_SCALING_STRETCH;
        swap_chain_desc.AlphaMode          = DXGI_ALPHA_MODE_IGNORE;

#if defined(_WIN32_WINNT_WIN10)
        // DXGI_SWAP_EFFECT_FLIP_DISCARD is supported starting with Windows 10.
        if (IsWindows10OrGreater())
        {
            swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        }
        else
#endif
#if defined(_WIN32_WINNT_WIN10) || defined(_WIN32_WINNT_WINBLUE)
            // DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL is supported starting with Windows 8.
            if (IsWindows8OrGreater())
            {
                swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            }
            else
#endif
            {
                swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            }

        hr = dxgi_factory_->CreateSwapChainForHwnd(device_.Get(), hwnd_, &swap_chain_desc, nullptr, nullptr,
                                                   &dxgi_swap_chain_);
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
    device_context_->OMSetRenderTargets(ARRAYSIZE(null_views), null_views, nullptr);
    rt_view_ = nullptr;
    ds_view_ = nullptr;
    device_context_->Flush();

    // Calculate the necessary render target size in pixels.
    output_size_.x = (UINT)::lround(DX::ConvertDipsToPixels(dip_size_.x, dpi_));
    output_size_.y = (UINT)::lround(DX::ConvertDipsToPixels(dip_size_.y, dpi_));

    hr = dxgi_swap_chain_->ResizeBuffers(2, output_size_.x, output_size_.y, DXGI_FORMAT_UNKNOWN,
                                         DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    if (SUCCEEDED(hr))
    {
        // Create the render target view and set it on the device
        ComPtr<ID3D11Resource> dxgi_back_buffer;

        hr = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer));

        if (SUCCEEDED(hr))
        {
            rt_view_ = nullptr;
            hr       = device_->CreateRenderTargetView(dxgi_back_buffer.Get(), nullptr, &rt_view_);
        }
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID3D11Texture2D> depth_stencil;

        CD3D11_TEXTURE2D_DESC tex_desc(DXGI_FORMAT_D24_UNORM_S8_UINT, output_size_.x, output_size_.y,
                                       1,  // This depth stencil view has only one texture.
                                       1,  // Use a single mipmap level.
                                       D3D11_BIND_DEPTH_STENCIL);

        hr = device_->CreateTexture2D(&tex_desc, nullptr, &depth_stencil);

        if (SUCCEEDED(hr))
        {
            CD3D11_DEPTH_STENCIL_VIEW_DESC desc(D3D11_DSV_DIMENSION_TEXTURE2D);

            ds_view_.Reset();
            hr = device_->CreateDepthStencilView(depth_stencil.Get(), &desc, &ds_view_);
        }

        if (SUCCEEDED(hr))
        {
            ID3D11RenderTargetView* main_view = rt_view_.Get();
            device_context_->OMSetRenderTargets(1, &main_view, ds_view_.Get());
        }
    }

    if (SUCCEEDED(hr))
    {
        // Set the 3D rendering viewport to target the entire window.
        CD3D11_VIEWPORT screen_viewport(0.0f, 0.0f, (float)output_size_.x, (float)output_size_.y);

        device_context_->RSSetViewports(1, &screen_viewport);
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
    if (dip_size_ != logical_size)
    {
        dip_size_ = logical_size;

        return CreateWindowSizeDependentResources();
    }
    return S_OK;
}

HRESULT D3D11DeviceResources::SetFullscreenState(bool fullscreen)
{
    HRESULT hr = E_FAIL;
    if (dxgi_swap_chain_)
    {
        hr = dxgi_swap_chain_->SetFullscreenState(fullscreen ? TRUE : FALSE, nullptr);
    }
    return hr;
}

HRESULT D3D11DeviceResources::GetFullscreenState(bool* fullscreen)
{
    HRESULT hr = E_FAIL;
    if (dxgi_swap_chain_)
    {
        BOOL is_fullscreen;
        hr = dxgi_swap_chain_->GetFullscreenState(&is_fullscreen, nullptr);

        if (SUCCEEDED(hr))
        {
            (*fullscreen) = (is_fullscreen == TRUE);
        }
    }
    return hr;
}

HRESULT D3D11DeviceResources::ResizeTarget(UINT width, UINT height)
{
    HRESULT hr = E_FAIL;
    if (dxgi_swap_chain_)
    {
        DXGI_MODE_DESC desc = { 0 };
        desc.Width          = width;
        desc.Height         = height;
        desc.Format         = DXGI_FORMAT_UNKNOWN;

        hr = dxgi_swap_chain_->ResizeTarget(&desc);
    }
    return hr;
}

HRESULT D3D11DeviceResources::GetDisplaySettings(DXGI_MODE_DESC** mode_descs, int* num)
{
    HRESULT hr = E_FAIL;
    if (dxgi_swap_chain_)
    {
        ComPtr<IDXGIOutput> output;
        HRESULT             hr = dxgi_swap_chain_->GetContainingOutput(&output);

        if (SUCCEEDED(hr))
        {
            UINT num_of_supported_modes = 0;
            output->GetDisplayModeList(desired_color_format_, 0, &num_of_supported_modes, 0);

            if (num_of_supported_modes > 0)
            {
                DXGI_MODE_DESC* supported_modes = new DXGI_MODE_DESC[num_of_supported_modes];
                ZeroMemory(supported_modes, sizeof(DXGI_MODE_DESC) * num_of_supported_modes);

                hr = output->GetDisplayModeList(desired_color_format_, 0, &num_of_supported_modes, supported_modes);
                if (SUCCEEDED(hr) && mode_descs && num)
                {
                    (*mode_descs) = supported_modes;
                    (*num)        = (int)num_of_supported_modes;
                }
                else
                {
                    delete[] supported_modes;
                }
            }
            else
            {
                hr = E_FAIL;
            }
        }
    }
    return hr;
}

STDMETHODIMP_(unsigned long) D3D11DeviceResources::AddRef()
{
    return InterlockedIncrement(&ref_count_);
}

STDMETHODIMP_(unsigned long) D3D11DeviceResources::Release()
{
    unsigned long newCount = InterlockedDecrement(&ref_count_);

    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

STDMETHODIMP D3D11DeviceResources::QueryInterface(const IID& riid, void** object)
{
    if (__uuidof(ID3D11DeviceResources) == riid)
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

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
