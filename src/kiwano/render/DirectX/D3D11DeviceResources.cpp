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

#include <kiwano/core/Logger.h>
#include <kiwano/render/DirectX/D3D11DeviceResources.h>

KGE_SUPPRESS_WARNING_PUSH
KGE_SUPPRESS_WARNING(4800)   // Implicit conversion from 'type' to bool
#include <versionhelpers.h>  // IsWindows10OrGreater
KGE_SUPPRESS_WARNING_POP

#pragma comment(lib, "d3d11.lib")

namespace kiwano
{
#if defined(KGE_DEBUG)
namespace DX
{
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
}  // namespace DX
#endif

struct D3D11DeviceResources : public ID3D11DeviceResources
{
public:
    HRESULT Present(bool vsync) override;

    void ClearRenderTarget(Color& clear_color) override;

    HRESULT HandleDeviceLost() override;

    HRESULT SetLogicalSize(Size logical_size) override;

    HRESULT SetDpi(float dpi) override;

    HRESULT SetFullscreenState(bool fullscreen) override;

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
    HWND          hwnd_;
    float         dpi_;
    Size          logical_size_;
    Size          output_size_;
    unsigned long ref_count_;

    D3D_FEATURE_LEVEL d3d_feature_level_;
};

D3D11DeviceResources::D3D11DeviceResources()
    : ref_count_(0)
    , hwnd_(nullptr)
    , d3d_feature_level_(D3D_FEATURE_LEVEL_9_1)
{
    dpi_ = 96.f;  // dpi_ = (float)GetDpiForWindow(hwnd);
}

D3D11DeviceResources::~D3D11DeviceResources()
{
    DiscardResources();
}

HRESULT ID3D11DeviceResources::Create(ID3D11DeviceResources** device_resources, HWND hwnd)
{
    HRESULT hr = E_FAIL;

    if (device_resources)
    {
        D3D11DeviceResources* res = new (std::nothrow) D3D11DeviceResources;
        if (res)
        {
            RECT rc;
            ::GetClientRect(hwnd, &rc);

            res->hwnd_           = hwnd;
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

HRESULT D3D11DeviceResources::Present(bool vsync)
{
    KGE_ASSERT(dxgi_swap_chain_ != nullptr);

    // The first argument instructs DXGI to block until VSync.
    return dxgi_swap_chain_->Present(vsync ? 1 : 0, 0);
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
    if (DX::SdkLayersAvailable())
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
        device_         = device;
        device_context_ = context;

        if (SUCCEEDED(hr))
        {
            ComPtr<IDXGIDevice> dxgi_device;
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
                ComPtr<IDXGIFactory> dxgi_factory;
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
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };

        swap_chain_desc.BufferCount                        = 2;
        swap_chain_desc.BufferDesc.Width                   = ::lround(output_size_.x);
        swap_chain_desc.BufferDesc.Height                  = ::lround(output_size_.y);
        swap_chain_desc.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
        swap_chain_desc.BufferDesc.RefreshRate.Numerator   = 60;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_desc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_CENTERED;
        swap_chain_desc.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.OutputWindow                       = hwnd_;
        swap_chain_desc.SampleDesc.Count                   = 1;
        swap_chain_desc.SampleDesc.Quality                 = 0;
        swap_chain_desc.Windowed                           = TRUE;

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

        hr = dxgi_factory_->CreateSwapChain(device_.Get(), &swap_chain_desc, &dxgi_swap_chain_);
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
    output_size_.x = DX::ConvertDipsToPixels(logical_size_.x, dpi_);
    output_size_.y = DX::ConvertDipsToPixels(logical_size_.y, dpi_);

    // Prevent zero size DirectX content from being created.
    output_size_.x = std::max(output_size_.x, 1.f);
    output_size_.y = std::max(output_size_.y, 1.f);

    hr = dxgi_swap_chain_->ResizeBuffers(2, /* Double-buffered swap chain */
                                         ::lround(output_size_.x), ::lround(output_size_.y), DXGI_FORMAT_UNKNOWN,
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

        CD3D11_TEXTURE2D_DESC tex_desc(DXGI_FORMAT_D24_UNORM_S8_UINT, static_cast<uint32_t>(output_size_.x),
                                       static_cast<uint32_t>(output_size_.y),
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
        CD3D11_VIEWPORT screen_viewport(0.0f, 0.0f, output_size_.x, output_size_.y);

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

        return CreateWindowSizeDependentResources();
    }
    return S_OK;
}

HRESULT D3D11DeviceResources::SetFullscreenState(bool fullscreen)
{
    HRESULT hr = dxgi_swap_chain_->SetFullscreenState(fullscreen ? TRUE : FALSE, nullptr);
    return hr;
}

HRESULT D3D11DeviceResources::ResizeTarget(UINT width, UINT height)
{
    DXGI_MODE_DESC desc = { 0 };
    desc.Width          = width;
    desc.Height         = height;
    desc.Format         = DXGI_FORMAT_UNKNOWN;

    HRESULT hr = dxgi_swap_chain_->ResizeTarget(&desc);
    return hr;
}

HRESULT D3D11DeviceResources::GetDisplaySettings(DXGI_MODE_DESC** mode_descs, int* num)
{
    ComPtr<IDXGIAdapter> dxgi_adapter;

    HRESULT hr = dxgi_device_->GetAdapter(&dxgi_adapter);
    if (SUCCEEDED(hr))
    {
        ComPtr<IDXGIOutput> output;
        hr = dxgi_adapter->EnumOutputs(0, &output);

        if (SUCCEEDED(hr))
        {
            UINT        modes_num = 0;
            DXGI_FORMAT format    = DXGI_FORMAT_B8G8R8A8_UNORM;
            UINT        flags     = DXGI_ENUM_MODES_INTERLACED;

            output->GetDisplayModeList(format, flags, &modes_num, 0);

            if (modes_num > 0)
            {
                DXGI_MODE_DESC* temp = new DXGI_MODE_DESC[modes_num];

                hr = output->GetDisplayModeList(format, flags, &modes_num, temp);
                if (SUCCEEDED(hr) && mode_descs && num)
                {
                    (*mode_descs) = temp;
                    (*num)        = (int)modes_num;
                }
                else
                {
                    delete[] temp;
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

}  // namespace kiwano
