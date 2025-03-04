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
#include <kiwano/render/DirectX/D3D10DeviceResources.h>

#pragma comment(lib, "d3d10_1.lib")

namespace kiwano
{
namespace graphics
{
namespace directx
{

// Global pointer for Direct3D11 device resources
static ComPtr<ID3D10DeviceResources> global_d3d10_device_resources;

HRESULT CreateD3DDevice(IDXGIAdapter* adapter, D3D10_DRIVER_TYPE driver_type, uint32_t flags, ID3D10Device1** device)
{
    HRESULT hr = S_OK;

    static const D3D10_FEATURE_LEVEL1 levels[] = {
        D3D10_FEATURE_LEVEL_10_1, D3D10_FEATURE_LEVEL_10_0, D3D10_FEATURE_LEVEL_9_3,
        D3D10_FEATURE_LEVEL_9_2,  D3D10_FEATURE_LEVEL_9_1,
    };

    for (uint32_t level = 0; level < ARRAYSIZE(levels); level++)
    {
        hr = D3D10CreateDevice1(adapter, driver_type, NULL, flags, levels[level], D3D10_1_SDK_VERSION, device);

        if (SUCCEEDED(hr))
            break;
    }
    return hr;
}

#if defined(KGE_DEBUG)
inline bool SdkLayersAvailable()
{
    HRESULT hr = CreateD3DDevice(NULL,
                                 D3D10_DRIVER_TYPE_NULL,     // There is no need to create a real hardware device.
                                 D3D10_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
                                 nullptr                     // No need to keep the D3D device reference.
    );

    return SUCCEEDED(hr);
}
#endif

struct D3D10DeviceResources : public ID3D10DeviceResources
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
    D3D10DeviceResources();

    virtual ~D3D10DeviceResources();

    HRESULT CreateDeviceResources();

    HRESULT CreateWindowSizeDependentResources();

public:
    unsigned long     ref_count_;
    HWND              hwnd_;
    DXGI_FORMAT       desired_color_format_;
    float             dpi_;
    Size              dip_size_;
    math::Vec2T<UINT> output_size_;
};

ComPtr<ID3D10DeviceResources> GetD3D10DeviceResources()
{
    if (!global_d3d10_device_resources)
    {
        global_d3d10_device_resources.Reset(new (std::nothrow) D3D10DeviceResources);
    }
    return global_d3d10_device_resources;
}

D3D10DeviceResources::D3D10DeviceResources()
    : ref_count_(0)
    , hwnd_(nullptr)
    , dpi_(96.f)  // GetDpiForWindow(hwnd)
    , desired_color_format_(DXGI_FORMAT_B8G8R8A8_UNORM)
{
}

D3D10DeviceResources::~D3D10DeviceResources()
{
    DiscardResources();
}

HRESULT D3D10DeviceResources::Initialize(HWND hwnd, Size logical_size, float dpi)
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

HRESULT D3D10DeviceResources::Present(bool vsync)
{
    HRESULT hr = E_FAIL;

    if (dxgi_swap_chain_)
    {
        // The first argument instructs DXGI to block until VSync.
        hr = dxgi_swap_chain_->Present(vsync ? 1 : 0, vsync ? 0 : DXGI_PRESENT_DO_NOT_WAIT);
    }
    return hr;
}

void D3D10DeviceResources::ClearRenderTarget(Color& clear_color)
{
    KGE_ASSERT(device_ != nullptr && rt_view_ != nullptr && ds_view_ != nullptr);

    auto rt_view = rt_view_.Get();
    device_->OMSetRenderTargets(1, &rt_view, ds_view_.Get());
    device_->ClearRenderTargetView(rt_view, reinterpret_cast<float*>(&clear_color));
}

void D3D10DeviceResources::DiscardResources()
{
    device_.Reset();
    rt_view_.Reset();
    ds_view_.Reset();
    dxgi_device_.Reset();
    dxgi_swap_chain_.Reset();
    dxgi_factory_.Reset();

    hwnd_ = nullptr;
}

HRESULT D3D10DeviceResources::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    uint32_t creation_flags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(KGE_DEBUG) && defined(KGE_ENABLE_DX_DEBUG)
    if (SdkLayersAvailable())
    {
        creation_flags |= D3D10_CREATE_DEVICE_DEBUG;
    }
#endif

    ComPtr<ID3D10Device1> device;
    hr = CreateD3DDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, creation_flags, &device);

    if (SUCCEEDED(hr))
    {
        device_ = device;

        if (SUCCEEDED(hr))
        {
            ComPtr<IDXGIDevice> dxgi_device;
            hr = device_->QueryInterface(IID_PPV_ARGS(&dxgi_device));

            if (SUCCEEDED(hr))
            {
                dxgi_device_ = dxgi_device;
            }
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

    if (SUCCEEDED(hr))
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };

        swap_chain_desc.BufferCount                        = 2;
        swap_chain_desc.BufferDesc.Width                   = (UINT)output_size_.x;
        swap_chain_desc.BufferDesc.Height                  = (UINT)output_size_.y;
        swap_chain_desc.BufferDesc.Format                  = desired_color_format_;
        swap_chain_desc.BufferDesc.RefreshRate.Numerator   = 60;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_desc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_CENTERED;
        swap_chain_desc.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.OutputWindow                       = hwnd_;
        swap_chain_desc.SampleDesc.Count                   = 1;
        swap_chain_desc.SampleDesc.Quality                 = 0;
        swap_chain_desc.Windowed                           = TRUE;
        swap_chain_desc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

        hr = dxgi_factory_->CreateSwapChain(device_.Get(), &swap_chain_desc, &dxgi_swap_chain_);
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
    rt_view_ = nullptr;
    ds_view_ = nullptr;
    device_->Flush();

    // Calculate the necessary render target size in pixels.
    output_size_.x = (UINT)::lround(DX::ConvertDipsToPixels(dip_size_.x, dpi_));
    output_size_.y = (UINT)::lround(DX::ConvertDipsToPixels(dip_size_.y, dpi_));

    hr = dxgi_swap_chain_->ResizeBuffers(2, output_size_.x, output_size_.y, DXGI_FORMAT_UNKNOWN, NULL);

    if (SUCCEEDED(hr))
    {
        // Create the render target view and set it on the device
        ComPtr<ID3D10Resource> dxgi_back_buffer;

        hr = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer));

        if (SUCCEEDED(hr))
        {
            D3D10_RENDER_TARGET_VIEW_DESC renderDesc;
            renderDesc.Format             = desired_color_format_;
            renderDesc.ViewDimension      = D3D10_RTV_DIMENSION_TEXTURE2D;
            renderDesc.Texture2D.MipSlice = 0;

            rt_view_ = nullptr;
            hr       = device_->CreateRenderTargetView(dxgi_back_buffer.Get(), &renderDesc, &rt_view_);
        }
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID3D10Texture2D> depth_stencil;

        D3D10_TEXTURE2D_DESC tex_desc;
        tex_desc.ArraySize          = 1;
        tex_desc.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
        tex_desc.CPUAccessFlags     = 0;
        tex_desc.Format             = DXGI_FORMAT_D16_UNORM;
        tex_desc.Width              = output_size_.x;
        tex_desc.Height             = output_size_.y;
        tex_desc.MipLevels          = 1;
        tex_desc.MiscFlags          = 0;
        tex_desc.SampleDesc.Count   = 1;
        tex_desc.SampleDesc.Quality = 0;
        tex_desc.Usage              = D3D10_USAGE_DEFAULT;

        hr = device_->CreateTexture2D(&tex_desc, NULL, &depth_stencil);

        if (SUCCEEDED(hr))
        {
            D3D10_DEPTH_STENCIL_VIEW_DESC desc;
            desc.Format             = DXGI_FORMAT_D16_UNORM;
            desc.ViewDimension      = D3D10_DSV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipSlice = 0;

            ds_view_.Reset();
            hr = device_->CreateDepthStencilView(depth_stencil.Get(), &desc, &ds_view_);
        }

        if (SUCCEEDED(hr))
        {
            ID3D10RenderTargetView* main_view = rt_view_.Get();
            device_->OMSetRenderTargets(1, &main_view, ds_view_.Get());
        }
    }

    if (SUCCEEDED(hr))
    {
        // Set a new viewport based on the new dimensions
        D3D10_VIEWPORT viewport;
        viewport.Width    = output_size_.x;
        viewport.Height   = output_size_.y;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;

        device_->RSSetViewports(1, &viewport);
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
    if (dip_size_ != logical_size)
    {
        dip_size_ = logical_size;

        return CreateWindowSizeDependentResources();
    }
    return S_OK;
}

HRESULT D3D10DeviceResources::SetFullscreenState(bool fullscreen)
{
    HRESULT hr = E_FAIL;
    if (dxgi_swap_chain_)
    {
        hr = dxgi_swap_chain_->SetFullscreenState(fullscreen ? TRUE : FALSE, nullptr);
    }
    return hr;
}

HRESULT D3D10DeviceResources::GetFullscreenState(bool* fullscreen)
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

HRESULT D3D10DeviceResources::ResizeTarget(UINT width, UINT height)
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

HRESULT D3D10DeviceResources::GetDisplaySettings(DXGI_MODE_DESC** mode_descs, int* num)
{
    HRESULT hr = E_FAIL;
    if (dxgi_swap_chain_)
    {
        ComPtr<IDXGIOutput> output;
        hr = dxgi_swap_chain_->GetContainingOutput(&output);

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

STDMETHODIMP D3D10DeviceResources::QueryInterface(const IID& riid, void** object)
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

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
