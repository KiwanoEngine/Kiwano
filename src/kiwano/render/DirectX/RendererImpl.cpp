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

#include <kiwano/core/Exception.h>
#include <kiwano/core/Logger.h>
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/platform/Application.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/DirectX/TextureRenderContextImpl.h>
#include <kiwano/render/DirectX/RendererImpl.h>
#include <kiwano/render/DirectX/NativePtr.h>

namespace kiwano
{

Renderer& Renderer::GetInstance()
{
    return RendererImpl::GetInstance();
}

RendererImpl& RendererImpl::GetInstance()
{
    static RendererImpl instance;
    return instance;
}

RendererImpl::RendererImpl()
{
    render_ctx_ = new RenderContextImpl;
}

void RendererImpl::MakeContextForWindow(WindowPtr window)
{
    KGE_SYS_LOG("Creating device resources");

    KGE_THROW_IF_FAILED(::CoInitialize(nullptr), "CoInitialize failed");

    HWND target_window = window->GetHandle();
    output_size_       = window->GetSize();

    d2d_res_ = nullptr;
    d3d_res_ = nullptr;

    HRESULT hr = target_window ? S_OK : E_FAIL;

    // Direct3D device resources
    if (SUCCEEDED(hr))
    {
        hr = ID3DDeviceResources::Create(&d3d_res_, target_window);

        // Direct2D device resources
        if (SUCCEEDED(hr))
        {
            hr = ID2DDeviceResources::Create(&d2d_res_, d3d_res_->GetDXGIDevice(), d3d_res_->GetDXGISwapChain());

            // Other device resources
            if (SUCCEEDED(hr))
            {
                hr = render_ctx_->CreateDeviceResources(d2d_res_->GetFactory(), d2d_res_->GetDeviceContext());
            }

            // FontFileLoader and FontCollectionLoader
            if (SUCCEEDED(hr))
            {
                hr = IFontCollectionLoader::Create(&font_collection_loader_);

                if (SUCCEEDED(hr))
                {
                    hr = d2d_res_->GetDWriteFactory()->RegisterFontCollectionLoader(font_collection_loader_.Get());
                }
            }

            // ResourceFontFileLoader and ResourceFontCollectionLoader
            if (SUCCEEDED(hr))
            {
                hr = IResourceFontFileLoader::Create(&res_font_file_loader_);

                if (SUCCEEDED(hr))
                {
                    hr = d2d_res_->GetDWriteFactory()->RegisterFontFileLoader(res_font_file_loader_.Get());
                }

                if (SUCCEEDED(hr))
                {
                    hr = IResourceFontCollectionLoader::Create(&res_font_collection_loader_,
                                                               res_font_file_loader_.Get());

                    if (SUCCEEDED(hr))
                    {
                        hr = d2d_res_->GetDWriteFactory()->RegisterFontCollectionLoader(
                            res_font_collection_loader_.Get());
                    }
                }
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create render resources failed");
}

void RendererImpl::Destroy()
{
    KGE_SYS_LOG("Destroying device resources");

    d2d_res_->GetDWriteFactory()->UnregisterFontFileLoader(res_font_file_loader_.Get());
    res_font_file_loader_.Reset();

    d2d_res_->GetDWriteFactory()->UnregisterFontCollectionLoader(res_font_collection_loader_.Get());
    res_font_collection_loader_.Reset();

    render_ctx_.Reset();
    d2d_res_.Reset();
    d3d_res_.Reset();

    ::CoUninitialize();
}

void RendererImpl::BeginDraw()
{
    KGE_ASSERT(render_ctx_);

    render_ctx_->BeginDraw();
}

void RendererImpl::EndDraw()
{
    KGE_ASSERT(render_ctx_);

    render_ctx_->EndDraw();
}

void RendererImpl::Clear()
{
    KGE_ASSERT(d3d_res_);

    d3d_res_->ClearRenderTarget(clear_color_);
}

void RendererImpl::Present()
{
    KGE_ASSERT(d3d_res_);

    HRESULT hr = d3d_res_->Present(vsync_);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        // 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
        hr = HandleDeviceLost();
    }

    KGE_THROW_IF_FAILED(hr, "Unexpected DXGI exception");
}

HRESULT RendererImpl::HandleDeviceLost()
{
    KGE_ASSERT(d3d_res_ && d2d_res_ && render_ctx_);

    HRESULT hr = d3d_res_->HandleDeviceLost();

    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->HandleDeviceLost(d3d_res_->GetDXGIDevice(), d3d_res_->GetDXGISwapChain());
    }

    if (SUCCEEDED(hr))
    {
        hr = render_ctx_->CreateDeviceResources(d2d_res_->GetFactory(), d2d_res_->GetDeviceContext());
    }
    return hr;
}

void RendererImpl::CreateTexture(Texture& texture, String const& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_WARN("Texture file '%s' not found!", file_path.c_str());
        hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
    }

    if (SUCCEEDED(hr))
    {
        WideString full_path = string::ToWide(FileSystem::GetInstance().GetFullPathForFile(file_path));

        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path.c_str());

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICBitmapFrameDecode> source;
            hr = decoder->GetFrame(0, &source);

            if (SUCCEEDED(hr))
            {
                ComPtr<IWICFormatConverter> converter;
                hr = d2d_res_->CreateBitmapConverter(converter, source, GUID_WICPixelFormat32bppPBGRA,
                                                     WICBitmapDitherTypeNone, nullptr, 0.f,
                                                     WICBitmapPaletteTypeMedianCut);

                if (SUCCEEDED(hr))
                {
                    ComPtr<ID2D1Bitmap> bitmap;
                    hr = d2d_res_->CreateBitmapFromConverter(bitmap, nullptr, converter);

                    if (SUCCEEDED(hr))
                    {
                        NativePtr::Set(texture, bitmap);

                        texture.SetSize({ bitmap->GetSize().width, bitmap->GetSize().height });
                        texture.SetSizeInPixels({ bitmap->GetPixelSize().width, bitmap->GetPixelSize().height });
                    }
                }
            }
        }
    }

    if (FAILED(hr))
    {
        KGE_THROW_IF_FAILED(hr, "Load texture failed");
    }
}

void RendererImpl::CreateTexture(Texture& texture, Resource const& resource)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        Resource::Data data = resource.GetData();

        hr = data ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICBitmapDecoder> decoder;
            hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, data.buffer, (DWORD)data.size);

            if (SUCCEEDED(hr))
            {
                ComPtr<IWICBitmapFrameDecode> source;
                hr = decoder->GetFrame(0, &source);

                if (SUCCEEDED(hr))
                {
                    ComPtr<IWICFormatConverter> converter;
                    hr = d2d_res_->CreateBitmapConverter(converter, source, GUID_WICPixelFormat32bppPBGRA,
                                                         WICBitmapDitherTypeNone, nullptr, 0.f,
                                                         WICBitmapPaletteTypeMedianCut);

                    if (SUCCEEDED(hr))
                    {
                        ComPtr<ID2D1Bitmap> bitmap;
                        hr = d2d_res_->CreateBitmapFromConverter(bitmap, nullptr, converter);

                        if (SUCCEEDED(hr))
                        {
                            NativePtr::Set(texture, bitmap);

                            texture.SetSize({ bitmap->GetSize().width, bitmap->GetSize().height });
                            texture.SetSizeInPixels({ bitmap->GetPixelSize().width, bitmap->GetPixelSize().height });
                        }
                    }
                }
            }
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN("Load texture failed with HRESULT of %08X!", hr);
    }
}

void RendererImpl::CreateGifImage(GifImage& gif, String const& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_WARN("Gif texture file '%s' not found!", file_path.c_str());
        hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
    }

    if (SUCCEEDED(hr))
    {
        WideString full_path = string::ToWide(FileSystem::GetInstance().GetFullPathForFile(file_path));

        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path.c_str());

        if (SUCCEEDED(hr))
        {
            NativePtr::Set(gif, decoder);
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN("Load GIF texture failed with HRESULT of %08X!", hr);
    }
}

void RendererImpl::CreateGifImage(GifImage& gif, Resource const& resource)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        Resource::Data data = resource.GetData();

        hr = data ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICBitmapDecoder> decoder;
            hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, data.buffer, (DWORD)data.size);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(gif, decoder);
            }
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN("Load GIF texture failed with HRESULT of %08X!", hr);
    }
}

void RendererImpl::CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    auto decoder = NativePtr::Get<IWICBitmapDecoder>(gif);

    if (!decoder)
    {
        hr = E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICBitmapFrameDecode> wic_frame;

        hr = decoder->GetFrame(UINT(frame_index), &wic_frame);

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICFormatConverter> converter;
            d2d_res_->CreateBitmapConverter(converter, wic_frame, GUID_WICPixelFormat32bppPBGRA,
                                            WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

            if (SUCCEEDED(hr))
            {
                ComPtr<ID2D1Bitmap> bitmap;
                hr = d2d_res_->CreateBitmapFromConverter(bitmap, nullptr, converter);

                if (SUCCEEDED(hr))
                {
                    frame.texture = new Texture;
                    NativePtr::Set(frame.texture, bitmap);

                    frame.texture->SetSize({ bitmap->GetSize().width, bitmap->GetSize().height });
                    frame.texture->SetSizeInPixels({ bitmap->GetPixelSize().width, bitmap->GetPixelSize().height });
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            PROPVARIANT prop_val;
            PropVariantInit(&prop_val);

            // Get Metadata Query Reader from the frame
            ComPtr<IWICMetadataQueryReader> metadata_reader;
            hr = wic_frame->GetMetadataQueryReader(&metadata_reader);

            // Get the Metadata for the current frame
            if (SUCCEEDED(hr))
            {
                hr = metadata_reader->GetMetadataByName(L"/imgdesc/Left", &prop_val);
                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        frame.rect.left_top.x = static_cast<float>(prop_val.uiVal);
                    }
                    PropVariantClear(&prop_val);
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = metadata_reader->GetMetadataByName(L"/imgdesc/Top", &prop_val);
                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        frame.rect.left_top.y = static_cast<float>(prop_val.uiVal);
                    }
                    PropVariantClear(&prop_val);
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = metadata_reader->GetMetadataByName(L"/imgdesc/Width", &prop_val);
                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        frame.rect.right_bottom.x = frame.rect.left_top.x + static_cast<float>(prop_val.uiVal);
                    }
                    PropVariantClear(&prop_val);
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = metadata_reader->GetMetadataByName(L"/imgdesc/Height", &prop_val);
                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        frame.rect.right_bottom.y = frame.rect.left_top.y + static_cast<float>(prop_val.uiVal);
                    }
                    PropVariantClear(&prop_val);
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = metadata_reader->GetMetadataByName(L"/grctlext/Delay", &prop_val);

                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);

                    if (SUCCEEDED(hr))
                    {
                        uint32_t udelay = 0;

                        hr = UIntMult(prop_val.uiVal, 10, &udelay);
                        if (SUCCEEDED(hr))
                        {
                            frame.delay.SetMilliseconds(static_cast<long>(udelay));
                        }
                    }
                    PropVariantClear(&prop_val);
                }
                else
                {
                    frame.delay = 0;
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = metadata_reader->GetMetadataByName(L"/grctlext/Disposal", &prop_val);

                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI1) ? S_OK : E_FAIL;
                    if (SUCCEEDED(hr))
                    {
                        frame.disposal_type = GifImage::DisposalType(prop_val.bVal);
                    }
                    ::PropVariantClear(&prop_val);
                }
                else
                {
                    frame.disposal_type = GifImage::DisposalType::Unknown;
                }
            }

            ::PropVariantClear(&prop_val);
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN("Load GIF frame failed with HRESULT of %08X!", hr);
    }
}

void RendererImpl::CreateFontCollection(Font& font, String const& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        if (!FileSystem::GetInstance().IsFileExists(file_path))
        {
            KGE_WARN("Font file '%s' not found!", file_path.c_str());
            hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }
    }

    if (SUCCEEDED(hr))
    {
        LPVOID   key       = nullptr;
        uint32_t key_size  = 0;
        String   full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);

        hr = font_collection_loader_->AddFilePaths({ full_path }, &key, &key_size);

        if (SUCCEEDED(hr))
        {
            ComPtr<IDWriteFontCollection> font_collection;
            hr = d2d_res_->GetDWriteFactory()->CreateCustomFontCollection(font_collection_loader_.Get(), key, key_size,
                                                                          &font_collection);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(font, font_collection);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create font collection failed");
}

void RendererImpl::CreateFontCollection(Font& font, Resource const& res)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        LPVOID   key      = nullptr;
        uint32_t key_size = 0;

        hr = res_font_collection_loader_->AddResources(Vector<Resource>{ res }, &key, &key_size);

        if (SUCCEEDED(hr))
        {
            ComPtr<IDWriteFontCollection> font_collection;
            hr = d2d_res_->GetDWriteFactory()->CreateCustomFontCollection(res_font_collection_loader_.Get(), key,
                                                                          key_size, &font_collection);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(font, font_collection);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create font collection failed");
}

void RendererImpl::CreateTextLayout(TextLayout& layout, const String& content, const TextStyle& style)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (content.empty())
    {
        layout.Clear();
        layout.SetDirtyFlag(TextLayout::DirtyFlag::Updated);
        return;
    }

    if (SUCCEEDED(hr))
    {
        WideString         font_family = style.font_family.empty() ? L"" : string::ToWide(style.font_family);
        DWRITE_FONT_WEIGHT font_weight = DWRITE_FONT_WEIGHT(style.font_weight);
        DWRITE_FONT_STYLE  font_style  = style.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL;
        auto               collection  = NativePtr::Get<IDWriteFontCollection>(style.font);

        ComPtr<IDWriteTextFormat> format;
        hr = d2d_res_->CreateTextFormat(format, font_family.c_str(), collection, font_weight, font_style,
                                        DWRITE_FONT_STRETCH_NORMAL, style.font_size);

        if (SUCCEEDED(hr))
        {
            WideString wide = string::ToWide(content);

            ComPtr<IDWriteTextLayout> output;
            hr = d2d_res_->CreateTextLayout(output, wide.c_str(), wide.length(), format);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(layout, output);
                layout.SetDirtyFlag(TextLayout::DirtyFlag::Updated);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create text layout failed");
}

void RendererImpl::CreateLineShape(Shape& shape, Point const& begin_pos, Point const& end_pos)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1PathGeometry> path_geo;
        hr = d2d_res_->GetFactory()->CreatePathGeometry(&path_geo);

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1GeometrySink> path_sink;
            hr = path_geo->Open(&path_sink);

            if (SUCCEEDED(hr))
            {
                path_sink->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
                path_sink->AddLine(DX::ConvertToPoint2F(end_pos));
                path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
                hr = path_sink->Close();
            }

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(shape, path_geo);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1PathGeometry failed");
}

void RendererImpl::CreateRectShape(Shape& shape, Rect const& rect)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<ID2D1RectangleGeometry> output;
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->GetFactory()->CreateRectangleGeometry(DX::ConvertToRectF(rect), &output);
    }

    if (SUCCEEDED(hr))
    {
        NativePtr::Set(shape, output);
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1RectangleGeometry failed");
}

void RendererImpl::CreateRoundedRectShape(Shape& shape, Rect const& rect, Vec2 const& radius)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<ID2D1RoundedRectangleGeometry> output;
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->GetFactory()->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), &output);
    }

    if (SUCCEEDED(hr))
    {
        NativePtr::Set(shape, output);
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1RoundedRectangleGeometry failed");
}

void RendererImpl::CreateEllipseShape(Shape& shape, Point const& center, Vec2 const& radius)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<ID2D1EllipseGeometry> output;
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->GetFactory()->CreateEllipseGeometry(
            D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), &output);
    }

    if (SUCCEEDED(hr))
    {
        NativePtr::Set(shape, output);
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1EllipseGeometry failed");
}

void RendererImpl::CreateShapeSink(ShapeMaker& maker)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1PathGeometry> geometry;

        hr = d2d_res_->GetFactory()->CreatePathGeometry(&geometry);

        if (SUCCEEDED(hr))
        {
            ShapePtr shape = new Shape;
            NativePtr::Set(shape, geometry);

            maker.SetShape(shape);
        }
    }
    KGE_THROW_IF_FAILED(hr, "Create ID2D1PathGeometry failed");
}

void RendererImpl::CreateBrush(Brush& brush, Color const& color)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1SolidColorBrush> solid_brush;

        if (brush.GetType() == Brush::Type::SolidColor && brush.IsValid())
        {
            hr = NativePtr::Get<ID2D1Brush>(brush)->QueryInterface(&solid_brush);
            if (SUCCEEDED(hr))
            {
                solid_brush->SetColor(DX::ConvertToColorF(color));
            }
        }
        else
        {
            hr = d2d_res_->GetDeviceContext()->CreateSolidColorBrush(DX::ConvertToColorF(color), &solid_brush);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(brush, solid_brush);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1SolidBrush failed");
}

void RendererImpl::CreateBrush(Brush& brush, LinearGradientStyle const& style)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1GradientStopCollection> collection;
        hr = d2d_res_->GetDeviceContext()->CreateGradientStopCollection(
            reinterpret_cast<const D2D1_GRADIENT_STOP*>(&style.stops[0]), UINT32(style.stops.size()), D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE(style.extend_mode), &collection);

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1LinearGradientBrush> output;
            hr = d2d_res_->GetDeviceContext()->CreateLinearGradientBrush(
                D2D1::LinearGradientBrushProperties(DX::ConvertToPoint2F(style.begin), DX::ConvertToPoint2F(style.end)),
                collection.Get(), &output);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(brush, output);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1LinearGradientBrush failed");
}

void RendererImpl::CreateBrush(Brush& brush, RadialGradientStyle const& style)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1GradientStopCollection> collection;
        hr = d2d_res_->GetDeviceContext()->CreateGradientStopCollection(
            reinterpret_cast<const D2D1_GRADIENT_STOP*>(&style.stops[0]), UINT32(style.stops.size()), D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE(style.extend_mode), &collection);

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1RadialGradientBrush> output;
            hr = d2d_res_->GetDeviceContext()->CreateRadialGradientBrush(
                D2D1::RadialGradientBrushProperties(DX::ConvertToPoint2F(style.center),
                                                    DX::ConvertToPoint2F(style.offset), style.radius.x, style.radius.y),
                collection.Get(), &output);

            if (SUCCEEDED(hr))
            {
                NativePtr::Set(brush, output);
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1RadialGradientBrush failed");
}

void RendererImpl::CreateStrokeStyle(StrokeStyle& stroke_style)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        D2D1_CAP_STYLE  cap         = D2D1_CAP_STYLE(stroke_style.GetCapStyle());
        D2D1_LINE_JOIN  line_join   = D2D1_LINE_JOIN(stroke_style.GetLineJoinStyle());
        D2D1_DASH_STYLE dash_style  = D2D1_DASH_STYLE_SOLID;
        const float*    dash_array  = nullptr;
        uint32_t        dash_count  = 0;
        float           dash_offset = stroke_style.GetDashOffset();
        const auto&     dashes      = stroke_style.GetDashArray();

        if (!dashes.empty())
        {
            dash_array = &dashes[0];
            dash_count = uint32_t(dashes.size());
            dash_style = D2D1_DASH_STYLE_CUSTOM;
        }

        auto params = D2D1::StrokeStyleProperties(cap, cap, cap, line_join, 10.0f, dash_style, dash_offset);

        ComPtr<ID2D1StrokeStyle> output;
        hr = d2d_res_->GetFactory()->CreateStrokeStyle(params, dash_array, dash_count, &output);

        if (SUCCEEDED(hr))
        {
            NativePtr::Set(stroke_style, output);
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create ID2D1StrokeStyle failed");
}

TextureRenderContextPtr RendererImpl::CreateTextureRenderContext(const Size* desired_size)
{
    TextureRenderContextImplPtr ptr = new TextureRenderContextImpl;

    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1BitmapRenderTarget> bitmap_rt;

        if (desired_size)
        {
            hr = d2d_res_->GetDeviceContext()->CreateCompatibleRenderTarget(DX::ConvertToSizeF(*desired_size),
                                                                            &bitmap_rt);
        }
        else
        {
            hr = d2d_res_->GetDeviceContext()->CreateCompatibleRenderTarget(&bitmap_rt);
        }

        if (SUCCEEDED(hr))
        {
            hr = ptr->CreateDeviceResources(d2d_res_->GetFactory(), bitmap_rt);
        }

        if (SUCCEEDED(hr))
        {
            ptr->bitmap_rt_ = bitmap_rt;
        }
    }

    if (SUCCEEDED(hr))
        return ptr;

    return nullptr;
}

void RendererImpl::Resize(uint32_t width, uint32_t height)
{
    HRESULT hr = S_OK;

    if (!d3d_res_)
        hr = E_UNEXPECTED;

    if (SUCCEEDED(hr))
    {
        output_size_.x = static_cast<float>(width);
        output_size_.y = static_cast<float>(height);

        hr = d3d_res_->SetLogicalSize(output_size_);
    }

    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->SetLogicalSize(output_size_.x, output_size_.y);
    }

    if (SUCCEEDED(hr))
    {
        render_ctx_->Resize(output_size_);
    }

    KGE_THROW_IF_FAILED(hr, "Resize render target failed");
}

}  // namespace kiwano
