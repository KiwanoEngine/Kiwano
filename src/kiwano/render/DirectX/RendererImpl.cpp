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
#include <kiwano/event/Events.h>
#include <kiwano/platform/NativeObject.hpp>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/platform/Application.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/DirectX/RendererImpl.h>

#define KGE_SET_STATUS_IF_FAILED(ERRCODE, OBJ, MESSAGE)                                   \
    if (FAILED(ERRCODE))                                                                  \
    {                                                                                     \
        OBJ.Fail(strings::Format("%s failed (%#x): %s", __FUNCTION__, ERRCODE, MESSAGE)); \
    }

namespace kiwano
{

using namespace kiwano::graphics::directx;

inline DXGI_FORMAT ConvertPixelFormat(PixelFormat format, UINT32& pitch)
{
    switch (format)
    {
    //case PixelFormat::Bpp32RGB:
    //    pitch = 4;
    //    return DXGI_FORMAT_R8G8B8X8_UNORM;
    case PixelFormat::Bpp32RGBA:
        pitch = 4;
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case PixelFormat::Bpp32BGR:
        pitch = 4;
        return DXGI_FORMAT_B8G8R8X8_UNORM;
    case PixelFormat::Bpp32BGRA:
        pitch = 4;
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

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
    : monitor_(nullptr)
{
}

void RendererImpl::MakeContextForWindow(WindowPtr window)
{
    KGE_DEBUG_LOGF("Creating device resources");

    KGE_THROW_IF_FAILED(::CoInitialize(nullptr), "CoInitialize failed");

    HWND       target_window = window->GetHandle();
    Resolution resolution    = window->GetCurrentResolution();
    HRESULT    hr            = target_window ? S_OK : E_FAIL;

    output_size_ = Size{ float(resolution.width), float(resolution.height) };

    // Initialize Direct3D resources
    if (SUCCEEDED(hr))
    {
        monitor_ = ::MonitorFromWindow(target_window, MONITOR_DEFAULTTONULL);

        auto d3d_res = graphics::directx::GetD3DDeviceResources();

        hr = d3d_res->Initialize(target_window, output_size_);
        if (FAILED(hr))
        {
            d3d_res->DiscardResources();
        }
        else
        {
            d3d_res_ = d3d_res;
        }
    }

    // Initialize Direct2D resources
    if (SUCCEEDED(hr))
    {
        auto d2d_res = graphics::directx::GetD2DDeviceResources();

        hr = d2d_res->Initialize(d3d_res_->GetDXGIDevice(), d3d_res_->GetDXGISwapChain());
        if (FAILED(hr))
        {
            d2d_res->DiscardResources();
        }
        else
        {
            d2d_res_ = d2d_res;
        }
    }

    // Initialize other device resources
    if (SUCCEEDED(hr))
    {
        RenderContextImplPtr ctx = MakePtr<RenderContextImpl>();

        hr = ctx->CreateDeviceResources(d2d_res_->GetFactory(), d2d_res_->GetDeviceContext());
        if (SUCCEEDED(hr))
        {
            render_ctx_ = ctx;
        }
    }

    //if (SUCCEEDED(hr))
    //{
    //    IDWriteFactory* dwrite = d2d_res_->GetDWriteFactory();
    //    if (dwrite)
    //    {
    //        ComPtr<IDWriteFontCollection> system_collection;
    //        if (SUCCEEDED(dwrite->GetSystemFontCollection(&system_collection, FALSE)))
    //        {
    //            Vector<String> family_names;
    //            if (SUCCEEDED(d2d_res_->GetFontFamilyNames(family_names, system_collection)))
    //            {
    //                // dummy font
    //                FontPtr font = MakePtr<Font>();
    //                for (const auto& name : family_names)
    //                {
    //                    FontCache::GetInstance().AddFontByFamily(name, font);
    //                }
    //            }
    //        }
    //    }
    //}

    KGE_THROW_IF_FAILED(hr, "Create render resources failed");
}

void RendererImpl::Destroy()
{
    KGE_DEBUG_LOGF("Destroying device resources");

    Renderer::Destroy();

    if (d2d_res_)
    {
        render_ctx_.Reset();
        d2d_res_->DiscardResources();
        d2d_res_.Reset();
    }

    if (d3d_res_)
    {
        d3d_res_->DiscardResources();
        d3d_res_.Reset();
    }

    ::CoUninitialize();
}

void RendererImpl::HandleEvent(EventModuleContext& ctx)
{
    Renderer::HandleEvent(ctx);

    auto evt = ctx.evt->Cast<WindowMovedEvent>();
    if (evt)
    {
        HMONITOR monitor = ::MonitorFromWindow(evt->window->GetHandle(), MONITOR_DEFAULTTONULL);
        if (monitor_ != monitor)
        {
            monitor_ = monitor;

            if (d2d_res_)
            {
                d2d_res_->ResetTextRenderingParams(monitor);
            }
        }
    }
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
    if (FAILED(hr) && hr != DXGI_ERROR_WAS_STILL_DRAWING)
    {
        KGE_THROW_IF_FAILED(hr, "Unexpected DXGI exception");
    }
}

void RendererImpl::CreateTexture(Texture& texture, const String& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        KGE_SET_STATUS_IF_FAILED(hr, texture,
                                 strings::Format("Texture file '%s' not found!", file_path.c_str()).c_str());
        return;
    }

    if (SUCCEEDED(hr))
    {
        WideString full_path = strings::NarrowToWide(FileSystem::GetInstance().GetFullPathForFile(file_path));

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
                        ComPolicy::Set(texture, bitmap);

                        texture.SetSize({ bitmap->GetSize().width, bitmap->GetSize().height });
                        texture.SetSizeInPixels({ bitmap->GetPixelSize().width, bitmap->GetPixelSize().height });
                    }
                }
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, texture, "Load texture failed");
}

void RendererImpl::CreateTexture(Texture& texture, const BinaryData& data)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        hr = data.IsValid() ? S_OK : E_FAIL;

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
                            ComPolicy::Set(texture, bitmap);

                            texture.SetSize({ bitmap->GetSize().width, bitmap->GetSize().height });
                            texture.SetSizeInPixels({ bitmap->GetPixelSize().width, bitmap->GetPixelSize().height });
                        }
                    }
                }
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, texture, "Load texture failed");
}

void RendererImpl::CreateTexture(Texture& texture, const PixelSize& size, const BinaryData& data, PixelFormat format)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        hr = data.IsValid() ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1Bitmap1> output;
            UINT32               pitch       = 0;
            const auto           dxgi_format = ConvertPixelFormat(format, pitch);

            hr = d2d_res_->GetDeviceContext()->CreateBitmap(
                DX::ConvertToSizeU(size), data.buffer, UINT(size.x) * pitch,
                D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
                                        D2D1::PixelFormat(dxgi_format, D2D1_ALPHA_MODE_PREMULTIPLIED)),
                &output);
            if (SUCCEEDED(hr))
            {
                ComPolicy::Set(texture, output);

                texture.SetSize({ output->GetSize().width, output->GetSize().height });
                texture.SetSizeInPixels({ output->GetPixelSize().width, output->GetPixelSize().height });
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, texture, "Load texture from memory failed");
}

void RendererImpl::CreateGifImage(GifImage& gif, const String& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        KGE_SET_STATUS_IF_FAILED(hr, gif,
                                 strings::Format("Gif texture file '%s' not found!", file_path.c_str()).c_str());
        return;
    }

    if (SUCCEEDED(hr))
    {
        WideString full_path = strings::NarrowToWide(FileSystem::GetInstance().GetFullPathForFile(file_path));

        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path.c_str());

        if (SUCCEEDED(hr))
        {
            ComPolicy::Set(gif, decoder);
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, gif, "Load GIF texture failed");
}

void RendererImpl::CreateGifImage(GifImage& gif, const BinaryData& data)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        hr = data.IsValid() ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICBitmapDecoder> decoder;
            hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, data.buffer, (DWORD)data.size);

            if (SUCCEEDED(hr))
            {
                ComPolicy::Set(gif, decoder);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, gif, "Load GIF texture failed");
}

void RendererImpl::CreateGifImageFrame(GifImage::Frame& frame, const GifImage& gif, size_t frame_index)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    auto decoder = ComPolicy::Get<IWICBitmapDecoder>(gif);

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
                    frame.texture = MakePtr<Texture>();
                    ComPolicy::Set(frame.texture, bitmap);

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

    KGE_SET_STATUS_IF_FAILED(hr, const_cast<GifImage&>(gif), "Load GIF frame failed");
}

void RendererImpl::CreateFontCollection(Font& font, Vector<String>& family_names, const String& file_path)
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
            hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            KGE_SET_STATUS_IF_FAILED(hr, font, strings::Format("Font file '%s' not found!", file_path.c_str()).c_str());
            return;
        }
    }

    if (SUCCEEDED(hr))
    {
        String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);

        ComPtr<IDWriteFontCollection> font_collection;
        hr = d2d_res_->CreateFontCollectionFromFiles(font_collection, { full_path });

        if (SUCCEEDED(hr))
        {
            d2d_res_->GetFontFamilyNames(family_names, font_collection);  // ignore the result
            ComPolicy::Set(font, font_collection);
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, font, "Create font collection failed");
}

void RendererImpl::CreateFontCollection(Font& font, Vector<String>& family_names, const BinaryData& data)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<IDWriteFontCollection> font_collection;
        hr = d2d_res_->CreateFontCollectionFromBinaryData(font_collection, Vector<BinaryData>{ data });

        if (SUCCEEDED(hr))
        {
            d2d_res_->GetFontFamilyNames(family_names, font_collection);  // ignore the result
            ComPolicy::Set(font, font_collection);
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, font, "Create font collection failed");
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
        layout.SetDirtyFlag(TextLayout::DirtyFlag::Dirty);
        return;
    }

    if (SUCCEEDED(hr))
    {
        FontPtr font = style.font;
        if (!font)
        {
            font = new Font;
        }

        float font_size    = font->GetSize();
        auto  font_weight  = DWRITE_FONT_WEIGHT(font->GetWeight());
        auto  font_style   = DWRITE_FONT_STYLE(font->GetPosture());
        auto  font_stretch = DWRITE_FONT_STRETCH(font->GetStretch());
        auto  collection   = ComPolicy::Get<IDWriteFontCollection>(font);

        WideString font_family;

        String name = font->GetFamilyName();
        if (!name.empty())
        {
            font_family = strings::NarrowToWide(name);
        }

        ComPtr<IDWriteTextFormat> format;
        hr = d2d_res_->CreateTextFormat(format, font_family.c_str(), collection, font_weight, font_style, font_stretch,
                                        font_size);

        if (SUCCEEDED(hr))
        {
            WideString wide = strings::NarrowToWide(content);

            ComPtr<IDWriteTextLayout> output;
            hr = d2d_res_->CreateTextLayout(output, wide.c_str(), UINT32(wide.length()), format);

            if (SUCCEEDED(hr))
            {
                ComPolicy::Set(layout, output);
                layout.SetDirtyFlag(TextLayout::DirtyFlag::Dirty);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, layout, "Create text layout failed");
}

void RendererImpl::CreateLineShape(Shape& shape, const Point& begin_pos, const Point& end_pos)
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
                ComPolicy::Set(shape, path_geo);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, shape, "Create ID2D1PathGeometry failed");
}

void RendererImpl::CreateRectShape(Shape& shape, const Rect& rect)
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
        ComPolicy::Set(shape, output);
    }

    KGE_SET_STATUS_IF_FAILED(hr, shape, "Create ID2D1RectangleGeometry failed");
}

void RendererImpl::CreateRoundedRectShape(Shape& shape, const Rect& rect, const Vec2& radius)
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
        ComPolicy::Set(shape, output);
    }

    KGE_SET_STATUS_IF_FAILED(hr, shape, "Create ID2D1RoundedRectangleGeometry failed");
}

void RendererImpl::CreateEllipseShape(Shape& shape, const Point& center, const Vec2& radius)
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
        ComPolicy::Set(shape, output);
    }

    KGE_SET_STATUS_IF_FAILED(hr, shape, "Create ID2D1EllipseGeometry failed");
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
            ShapePtr shape = MakePtr<Shape>();
            ComPolicy::Set(shape, geometry);

            maker.SetShape(shape);
        }
    }
    KGE_SET_STATUS_IF_FAILED(hr, maker, "Create ID2D1PathGeometry failed");
}

void RendererImpl::CreateBrush(Brush& brush, const Color& color)
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
            hr = ComPolicy::Get<ID2D1Brush>(brush)->QueryInterface(&solid_brush);
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
                ComPolicy::Set(brush, solid_brush);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, brush, "Create ID2D1SolidBrush failed");
}

void RendererImpl::CreateBrush(Brush& brush, const LinearGradientStyle& style)
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
                ComPolicy::Set(brush, output);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, brush, "Create ID2D1LinearGradientBrush failed");
}

void RendererImpl::CreateBrush(Brush& brush, const RadialGradientStyle& style)
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
                ComPolicy::Set(brush, output);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, brush, "Create ID2D1RadialGradientBrush failed");
}

void RendererImpl::CreateBrush(Brush& brush, TexturePtr texture)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        auto bitmap = ComPolicy::Get<ID2D1Bitmap>(texture);

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1BitmapBrush> output;
            hr = d2d_res_->GetDeviceContext()->CreateBitmapBrush(bitmap.Get(), &output);

            if (SUCCEEDED(hr))
            {
                ComPolicy::Set(brush, output);
            }
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, brush, "Create ID2D1RadialGradientBrush failed");
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
            ComPolicy::Set(stroke_style, output);
        }
    }

    KGE_SET_STATUS_IF_FAILED(hr, stroke_style, "Create ID2D1StrokeStyle failed");
}

RenderContextPtr RendererImpl::CreateTextureRenderContext(TexturePtr texture, const PixelSize& desired_size)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }
    else if (texture == nullptr)
    {
        hr = E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        RenderContextImplPtr ptr = MakePtr<RenderContextImpl>();

        ComPtr<ID2D1DeviceContext> render_ctx;
        hr = d2d_res_->GetDevice()->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
                                                        &render_ctx);

        if (SUCCEEDED(hr))
        {
            hr = ptr->CreateDeviceResources(d2d_res_->GetFactory(), render_ctx);
        }

        if (SUCCEEDED(hr))
        {
            ComPtr<ID2D1Bitmap1> output;
            hr = render_ctx->CreateBitmap(
                DX::ConvertToSizeU(desired_size), nullptr, 0,
                D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
                                        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
                &output);

            if (SUCCEEDED(hr))
            {
                render_ctx->SetTarget(output.Get());
                ComPolicy::Set(texture, output);

                texture->SetSize({ output->GetSize().width, output->GetSize().height });
                texture->SetSizeInPixels({ output->GetPixelSize().width, output->GetPixelSize().height });
                return ptr;
            }
        }
    }

    KGE_THROW_IF_FAILED(hr, "Create render context failed");
    return nullptr;
}

void RendererImpl::Resize(uint32_t width, uint32_t height)
{
    HRESULT hr = S_OK;

    if (!d3d_res_)
        hr = E_UNEXPECTED;

    if (SUCCEEDED(hr))
    {
        // Clear resources
        d2d_res_->GetDeviceContext()->SetTarget(nullptr);
    }

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
