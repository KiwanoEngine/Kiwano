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
#include <kiwano/render/DirectX/helper.h>
#include <kiwano/render/DirectX/FontCollectionLoader.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{

MIDL_INTERFACE("5706684a-bf6d-4b03-b627-094758a33032")
KGE_API ID2DDeviceResources : public IUnknown
{
public:
    virtual HRESULT Initialize(_In_ ComPtr<IDXGIDevice> dxgi_device, _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain) = 0;

    virtual HRESULT CreateBitmapSourceFromMemory(_Out_ ComPtr<IWICBitmapSource> & source, _In_ UINT width,
                                                 _In_ UINT height, _In_ UINT cbStride, _In_ UINT cbBufferSize,
                                                 _In_ BYTE * buffer, _In_ REFWICPixelFormatGUID cPixelFormat) = 0;

    virtual HRESULT CreateBitmapConverter(_Out_    ComPtr<IWICFormatConverter> & converter,
                                          _In_opt_ ComPtr<IWICBitmapSource> source, _In_ REFWICPixelFormatGUID format,
                                          WICBitmapDitherType dither, _In_opt_ ComPtr<IWICPalette> palette,
                                          double alpha_threshold_percent, WICBitmapPaletteType palette_translate) = 0;

    virtual HRESULT CreateBitmapFromConverter(_Out_                                  ComPtr<ID2D1Bitmap> & bitmap,
                                              _In_opt_ const D2D1_BITMAP_PROPERTIES* properties,
                                              _In_ ComPtr<IWICFormatConverter> converter) = 0;

    virtual HRESULT CreateBitmapDecoderFromFile(_Out_ ComPtr<IWICBitmapDecoder> & decoder, _In_ LPCWSTR file_path) = 0;

    virtual HRESULT CreateBitmapDecoderFromResource(_Out_ ComPtr<IWICBitmapDecoder> & decoder, _In_ void* data,
                                                    DWORD data_size) = 0;

    virtual HRESULT CreateTextFormat(_Out_ ComPtr<IDWriteTextFormat> & text_format, _In_ LPCWSTR family,
                                     _In_ ComPtr<IDWriteFontCollection> collection, DWRITE_FONT_WEIGHT weight,
                                     DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, FLOAT font_size) = 0;

    virtual HRESULT CreateTextLayout(_Out_ ComPtr<IDWriteTextLayout> & text_layout, _In_ LPCWSTR text, UINT32 length,
                                     _In_ ComPtr<IDWriteTextFormat> text_format) = 0;

    virtual HRESULT CreateFontCollectionFromFiles(_Out_                 ComPtr<IDWriteFontCollection> & font_collection,
                                                  const Vector<String>& file_paths) = 0;

    virtual HRESULT CreateFontCollectionFromBinaryData(_Out_ ComPtr<IDWriteFontCollection> & font_collection,
                                                       const Vector<BinaryData>& data) = 0;

    virtual HRESULT GetFontFamilyNames(_Out_ Vector<String> & family_names,
                                       _In_ ComPtr<IDWriteFontCollection> font_collection) = 0;

    virtual HRESULT SetDpi(float dpi) = 0;

    virtual HRESULT SetLogicalSize(float width, float height) = 0;

    virtual HRESULT HandleDeviceLost(_In_ ComPtr<IDXGIDevice> dxgi_device,
                                     _In_ ComPtr<IDXGISwapChain> dxgi_swap_chain) = 0;

    virtual void DiscardResources() = 0;

    virtual void SetTargetBitmap(_In_ ComPtr<ID2D1Bitmap1> target) = 0;

    virtual void ResetTextRenderingParams(_In_ HMONITOR monitor) = 0;

    inline ID2D1Factory1* GetFactory()
    {
        KGE_ASSERT(factory_);
        return factory_.Get();
    }

    inline IWICImagingFactory* GetWICImagingFactory()
    {
        KGE_ASSERT(imaging_factory_);
        return imaging_factory_.Get();
    }

    inline IDWriteFactory* GetDWriteFactory()
    {
        KGE_ASSERT(dwrite_factory_);
        return dwrite_factory_.Get();
    }

    inline ID2D1Device* GetDevice()
    {
        KGE_ASSERT(device_);
        return device_.Get();
    }

    inline ID2D1DeviceContext* GetDeviceContext()
    {
        KGE_ASSERT(device_context_);
        return device_context_.Get();
    }

    inline ID2D1Bitmap1* GetTargetBitmap()
    {
        KGE_ASSERT(target_bitmap_);
        return target_bitmap_.Get();
    }

protected:
    ComPtr<ID2D1Factory1>      factory_;
    ComPtr<ID2D1Device>        device_;
    ComPtr<ID2D1DeviceContext> device_context_;
    ComPtr<ID2D1Bitmap1>       target_bitmap_;

    ComPtr<IWICImagingFactory> imaging_factory_;
    ComPtr<IDWriteFactory>     dwrite_factory_;
};

extern ComPtr<ID2DDeviceResources> GetD2DDeviceResources();

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
