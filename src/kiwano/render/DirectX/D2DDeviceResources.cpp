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

#include <kiwano/render/DirectX/D2DDeviceResources.h>
#include <kiwano/core/Logger.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

namespace kiwano
{
struct D2DDeviceResources : public ID2DDeviceResources
{
public:
    D2DDeviceResources();

    virtual ~D2DDeviceResources();

    HRESULT CreateDeviceIndependentResources();

    HRESULT CreateDeviceResources(_In_ ComPtr<IDXGIDevice> dxgi_device, _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain);

    HRESULT CreateWindowSizeDependentResources();

    HRESULT CreateBitmapConverter(_Out_ ComPtr<IWICFormatConverter>& converter,
                                  _In_opt_ ComPtr<IWICBitmapSource> source, _In_ REFWICPixelFormatGUID format,
                                  WICBitmapDitherType dither, _In_opt_ ComPtr<IWICPalette> palette,
                                  double alpha_threshold_percent, WICBitmapPaletteType palette_translate) override;

    HRESULT CreateBitmapFromConverter(_Out_ ComPtr<ID2D1Bitmap>& bitmap,
                                      _In_opt_ const D2D1_BITMAP_PROPERTIES* properties,
                                      _In_ ComPtr<IWICFormatConverter> converter) override;

    HRESULT CreateBitmapDecoderFromFile(_Out_ ComPtr<IWICBitmapDecoder>& decoder, _In_ LPCWSTR file_path) override;

    HRESULT CreateBitmapDecoderFromResource(_Out_ ComPtr<IWICBitmapDecoder>& decoder, _In_ void* data,
                                            DWORD data_size) override;

    HRESULT CreateTextFormat(_Out_ ComPtr<IDWriteTextFormat>& text_format, _In_ LPCWSTR family,
                             _In_ ComPtr<IDWriteFontCollection> collection, DWRITE_FONT_WEIGHT weight,
                             DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, FLOAT font_size) override;

    HRESULT CreateTextLayout(_Out_ ComPtr<IDWriteTextLayout>& text_layout, _In_ LPCWSTR text,
                             UINT32 length, _In_ ComPtr<IDWriteTextFormat> text_format) override;

    HRESULT SetDpi(float dpi) override;

    HRESULT SetLogicalSize(float width, float height) override;

    HRESULT HandleDeviceLost(_In_ ComPtr<IDXGIDevice> dxgi_device,
                             _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain) override;

    void DiscardResources() override;

public:
    unsigned long STDMETHODCALLTYPE AddRef();

    unsigned long STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(IID const& riid, void** ppvObject);

private:
    unsigned long ref_count_;
    float         dpi_;

    ComPtr<IDXGISwapChain> dxgi_swap_chain_;
};

HRESULT ID2DDeviceResources::Create(_Out_ ID2DDeviceResources** device_resources, _In_ ComPtr<IDXGIDevice> dxgi_device,
                                    _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain)
{
    HRESULT hr = E_FAIL;
    if (device_resources)
    {
        D2DDeviceResources* res = new (std::nothrow) D2DDeviceResources;
        if (res)
        {
            hr = res->CreateDeviceIndependentResources();

            if (SUCCEEDED(hr))
            {
                hr = res->CreateDeviceResources(dxgi_device, dxgi_swap_chain);
            }

            if (SUCCEEDED(hr))
            {
                hr = res->CreateWindowSizeDependentResources();
            }
        }

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*device_resources);
            (*device_resources) = DX::SafeAcquire(res);
        }
        else
        {
            delete res;
            res = nullptr;
        }
    }
    return hr;
}

D2DDeviceResources::D2DDeviceResources()
    : ref_count_(0)
    , dpi_(96.f)
{
}

D2DDeviceResources::~D2DDeviceResources()
{
    DiscardResources();
}

STDMETHODIMP_(unsigned long) D2DDeviceResources::AddRef()
{
    return InterlockedIncrement(&ref_count_);
}

STDMETHODIMP_(unsigned long) D2DDeviceResources::Release()
{
    unsigned long newCount = InterlockedDecrement(&ref_count_);

    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

STDMETHODIMP D2DDeviceResources::QueryInterface(IID const& riid, void** object)
{
    if (__uuidof(ID2DDeviceResources) == riid)
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

void D2DDeviceResources::DiscardResources()
{
    factory_.Reset();
    device_.Reset();
    device_context_.Reset();
    target_bitmap_.Reset();

    imaging_factory_.Reset();
    dwrite_factory_.Reset();
}

HRESULT D2DDeviceResources::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    ComPtr<ID2D1Factory1>      factory;
    ComPtr<IWICImagingFactory> imaging_factory;
    ComPtr<IDWriteFactory>     dwrite_factory;

    D2D1_FACTORY_OPTIONS config;
    ZeroMemory(&config, sizeof(D2D1_FACTORY_OPTIONS));
#if defined(KGE_DEBUG) && defined(KGE_ENABLE_DX_DEBUG)
    config.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &config,
                           reinterpret_cast<void**>(&factory));

    if (SUCCEEDED(hr))
    {
        factory_ = factory;
    }

    if (SUCCEEDED(hr))
    {
        hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory),
                              reinterpret_cast<void**>(&imaging_factory));

        if (SUCCEEDED(hr))
        {
            imaging_factory_ = imaging_factory;
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                                 reinterpret_cast<IUnknown**>(&dwrite_factory));

        if (SUCCEEDED(hr))
        {
            dwrite_factory_ = dwrite_factory;
        }
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateDeviceResources(_In_ ComPtr<IDXGIDevice> dxgi_device,
                                                  _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain)
{
    if (!factory_)
        return E_UNEXPECTED;

    // Create the Direct2D device object and a corresponding context.
    ComPtr<ID2D1Device> device;

    HRESULT hr = factory_->CreateDevice(dxgi_device.Get(), &device);

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1DeviceContext> device_ctx;

        hr = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &device_ctx);

        if (SUCCEEDED(hr))
        {
            device_         = device;
            device_context_ = device_ctx;
            device_context_->SetDpi(dpi_, dpi_);
            dxgi_swap_chain_ = dxgi_swap_chain;
        }
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateWindowSizeDependentResources()
{
    if (!dxgi_swap_chain_ || !device_context_)
        return E_UNEXPECTED;

    // Create a Direct2D target bitmap associated with the
    // swap chain back buffer and set it as the current target.
    ComPtr<IDXGISurface> dxgi_back_buffer;

    HRESULT hr = dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer));

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1Bitmap1> target;
        hr = device_context_->CreateBitmapFromDxgiSurface(
            dxgi_back_buffer.Get(),
            D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                                    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi_, dpi_),
            &target);

        if (SUCCEEDED(hr))
        {
            target_bitmap_ = target;
            device_context_->SetTarget(target_bitmap_.Get());
        }
    }
    return hr;
}

HRESULT D2DDeviceResources::SetDpi(float dpi)
{
    if (!device_context_)
        return E_UNEXPECTED;

    device_context_->SetDpi(dpi, dpi);
    return CreateWindowSizeDependentResources();
}

HRESULT D2DDeviceResources::SetLogicalSize(float width, float height)
{
    return CreateWindowSizeDependentResources();
}

HRESULT D2DDeviceResources::HandleDeviceLost(_In_ ComPtr<IDXGIDevice> dxgi_device,
                                             _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain)
{
    dxgi_swap_chain_ = nullptr;

    HRESULT hr = CreateDeviceResources(dxgi_device, dxgi_swap_chain);

    if (SUCCEEDED(hr))
    {
        hr = CreateWindowSizeDependentResources();
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateBitmapConverter(_Out_ ComPtr<IWICFormatConverter>& converter,
                                                  _In_opt_ ComPtr<IWICBitmapSource> source,
                                                  _In_ REFWICPixelFormatGUID format, WICBitmapDitherType dither,
                                                  _In_opt_ ComPtr<IWICPalette> palette, double alpha_threshold_percent,
                                                  WICBitmapPaletteType palette_translate)
{
    if (!imaging_factory_)
        return E_UNEXPECTED;

    ComPtr<IWICFormatConverter> output;
    HRESULT                     hr = imaging_factory_->CreateFormatConverter(&output);

    if (SUCCEEDED(hr))
    {
        hr =
            output->Initialize(source.Get(), format, dither, palette.Get(), alpha_threshold_percent, palette_translate);
    }

    if (SUCCEEDED(hr))
    {
        converter = output;
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateBitmapFromConverter(_Out_ ComPtr<ID2D1Bitmap>& bitmap,
                                                      _In_opt_ const D2D1_BITMAP_PROPERTIES* properties,
                                                      _In_ ComPtr<IWICFormatConverter> converter)
{
    if (!device_context_)
        return E_UNEXPECTED;

    ComPtr<ID2D1Bitmap> output;

    HRESULT hr = device_context_->CreateBitmapFromWicBitmap(converter.Get(), properties, &output);

    if (SUCCEEDED(hr))
    {
        bitmap = output;
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateBitmapDecoderFromFile(_Out_ ComPtr<IWICBitmapDecoder>& decoder,
                                                        _In_ LPCWSTR                     file_path)
{
    if (!imaging_factory_)
        return E_UNEXPECTED;

    ComPtr<IWICBitmapDecoder> decoder_output;

    HRESULT hr = imaging_factory_->CreateDecoderFromFilename(file_path, nullptr, GENERIC_READ,
                                                             WICDecodeMetadataCacheOnLoad, &decoder_output);

    if (SUCCEEDED(hr))
    {
        decoder = decoder_output;
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateBitmapDecoderFromResource(_Out_ ComPtr<IWICBitmapDecoder>& decoder, _In_ void* data,
                                                            DWORD data_size)
{
    if (!imaging_factory_)
        return E_UNEXPECTED;

    HRESULT hr = data ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICStream> stream;
        hr = imaging_factory_->CreateStream(&stream);

        if (SUCCEEDED(hr))
        {
            hr = stream->InitializeFromMemory(static_cast<WICInProcPointer>(data), data_size);
        }

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICBitmapDecoder> decoder_output;
            hr = imaging_factory_->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad,
                                                           &decoder_output);

            if (SUCCEEDED(hr))
            {
                decoder = decoder_output;
            }
        }
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateTextFormat(_Out_ ComPtr<IDWriteTextFormat>& text_format, _In_ LPCWSTR family,
                                             _In_ ComPtr<IDWriteFontCollection> collection, DWRITE_FONT_WEIGHT weight,
                                             DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, FLOAT font_size)
{
    if (!dwrite_factory_)
        return E_UNEXPECTED;

    ComPtr<IDWriteTextFormat> output;
    HRESULT hr = dwrite_factory_->CreateTextFormat(family, collection.Get(), weight, style, stretch, font_size,
                                                   L"", &output);

    if (SUCCEEDED(hr))
    {
        text_format = output;
    }
    return hr;
}

HRESULT D2DDeviceResources::CreateTextLayout(_Out_ ComPtr<IDWriteTextLayout>& text_layout, _In_ LPCWSTR text,
                                             UINT32 length, _In_ ComPtr<IDWriteTextFormat> text_format)
{
    if (!dwrite_factory_)
        return E_UNEXPECTED;

    ComPtr<IDWriteTextLayout> output;

    HRESULT hr = dwrite_factory_->CreateTextLayout(text, length, text_format.Get(), 0, 0, &output);

    if (SUCCEEDED(hr))
    {
        text_layout = output;
    }
    return hr;
}

}  // namespace kiwano
