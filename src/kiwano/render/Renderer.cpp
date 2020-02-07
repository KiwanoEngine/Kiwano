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
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano/render/ShapeSink.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

Renderer::Renderer()
    : target_window_(nullptr)
    , vsync_(true)
    , clear_color_(Color::Black)
{
}

Renderer::~Renderer() {}

void Renderer::SetupComponent()
{
    KGE_SYS_LOG(L"Creating device resources");

    win32::ThrowIfFailed(::CoInitialize(nullptr));

    target_window_ = Window::Instance().GetHandle();
    output_size_   = Window::Instance().GetSize();

    d2d_res_ = nullptr;
    d3d_res_ = nullptr;

    HRESULT hr = target_window_ ? S_OK : E_FAIL;

    // Direct3D device resources
    if (SUCCEEDED(hr))
    {
        hr = ID3DDeviceResources::Create(&d3d_res_, target_window_);

        // Direct2D device resources
        if (SUCCEEDED(hr))
        {
            hr = ID2DDeviceResources::Create(&d2d_res_, d3d_res_->GetDXGIDevice(), d3d_res_->GetDXGISwapChain());

            // Other device resources
            if (SUCCEEDED(hr))
            {
                hr = render_ctx_.CreateDeviceResources(d2d_res_->GetFactory(), d2d_res_->GetDeviceContext());
            }

            // FontFileLoader and FontCollectionLoader
            if (SUCCEEDED(hr))
            {
                hr = IFontCollectionLoader::Create(&font_collection_loader_);

                if (SUCCEEDED(hr))
                {
                    hr = d2d_res_->GetDWriteFactory()->RegisterFontCollectionLoader(font_collection_loader_.get());
                }
            }

            // ResourceFontFileLoader and ResourceFontCollectionLoader
            if (SUCCEEDED(hr))
            {
                hr = IResourceFontFileLoader::Create(&res_font_file_loader_);

                if (SUCCEEDED(hr))
                {
                    hr = d2d_res_->GetDWriteFactory()->RegisterFontFileLoader(res_font_file_loader_.get());
                }

                if (SUCCEEDED(hr))
                {
                    hr = IResourceFontCollectionLoader::Create(&res_font_collection_loader_,
                                                               res_font_file_loader_.get());

                    if (SUCCEEDED(hr))
                    {
                        hr = d2d_res_->GetDWriteFactory()->RegisterFontCollectionLoader(
                            res_font_collection_loader_.get());
                    }
                }
            }
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::DestroyComponent()
{
    KGE_SYS_LOG(L"Destroying device resources");

    render_ctx_.DiscardDeviceResources();

    d2d_res_->GetDWriteFactory()->UnregisterFontFileLoader(res_font_file_loader_.get());
    res_font_file_loader_.reset();

    d2d_res_->GetDWriteFactory()->UnregisterFontCollectionLoader(res_font_collection_loader_.get());
    res_font_collection_loader_.reset();

    d2d_res_.reset();
    d3d_res_.reset();

    ::CoUninitialize();
}

void Renderer::BeginDraw()
{
    KGE_ASSERT(render_ctx_.IsValid());

    render_ctx_.SaveDrawingState();
    render_ctx_.BeginDraw();
}

void Renderer::EndDraw()
{
    KGE_ASSERT(render_ctx_.IsValid());

    render_ctx_.EndDraw();
    render_ctx_.RestoreDrawingState();
}

void Renderer::Clear()
{
    KGE_ASSERT(d3d_res_);

    HRESULT hr = d3d_res_->ClearRenderTarget(clear_color_);
    win32::ThrowIfFailed(hr);
}

void Renderer::Present()
{
    KGE_ASSERT(d3d_res_);

    HRESULT hr = d3d_res_->Present(vsync_);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        // 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
        hr = HandleDeviceLost();
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::HandleEvent(Event* evt)
{
    if (evt->IsType<WindowResizedEvent>())
    {
        auto window_evt = dynamic_cast<WindowResizedEvent*>(evt);
        ResizeTarget(window_evt->width, window_evt->height);
    }
}

HRESULT Renderer::HandleDeviceLost()
{
    KGE_ASSERT(d3d_res_ && d2d_res_ && render_ctx_.IsValid());

    HRESULT hr = d3d_res_->HandleDeviceLost();

    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->HandleDeviceLost(d3d_res_->GetDXGIDevice(), d3d_res_->GetDXGISwapChain());
    }

    if (SUCCEEDED(hr))
    {
        hr = render_ctx_.CreateDeviceResources(d2d_res_->GetFactory(), d2d_res_->GetDeviceContext());
    }
    return hr;
}

void Renderer::CreateTexture(Texture& texture, String const& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (!FileSystem::Instance().IsFileExists(file_path))
    {
        KGE_WARN(L"Texture file '%s' not found!", file_path.c_str());
        hr = E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        String full_path = FileSystem::Instance().GetFullPathForFile(file_path);

        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path);

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
                        texture.SetBitmap(bitmap);
                    }
                }
            }
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN(L"Load texture failed with HRESULT of %08X!", hr);
    }
}

void Renderer::CreateTexture(Texture& texture, Resource const& resource)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, resource);

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
                        texture.SetBitmap(bitmap);
                    }
                }
            }
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN(L"Load texture failed with HRESULT of %08X!", hr);
    }
}

void Renderer::CreateGifImage(GifImage& gif, String const& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (!FileSystem::Instance().IsFileExists(file_path))
    {
        KGE_WARN(L"Gif texture file '%s' not found!", file_path.c_str());
        hr = E_FAIL;
    }

    if (SUCCEEDED(hr))
    {
        String full_path = FileSystem::Instance().GetFullPathForFile(file_path);

        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path);

        if (SUCCEEDED(hr))
        {
            gif.SetDecoder(decoder);
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN(L"Load GIF texture failed with HRESULT of %08X!", hr);
    }
}

void Renderer::CreateGifImage(GifImage& gif, Resource const& resource)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICBitmapDecoder> decoder;
        hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, resource);

        if (SUCCEEDED(hr))
        {
            gif.SetDecoder(decoder);
        }
    }

    if (FAILED(hr))
    {
        KGE_WARN(L"Load GIF texture failed with HRESULT of %08X!", hr);
    }
}

void Renderer::CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (gif.GetDecoder() == nullptr)
    {
        hr = E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICBitmapFrameDecode> wic_frame;
        HRESULT                       hr = gif.GetDecoder()->GetFrame(UINT(frame_index), &wic_frame);

        if (SUCCEEDED(hr))
        {
            ComPtr<IWICFormatConverter> converter;
            d2d_res_->CreateBitmapConverter(converter, wic_frame, GUID_WICPixelFormat32bppPBGRA,
                                            WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

            if (SUCCEEDED(hr))
            {
                ComPtr<ID2D1Bitmap> raw_bitmap;
                hr = d2d_res_->CreateBitmapFromConverter(raw_bitmap, nullptr, converter);

                if (SUCCEEDED(hr))
                {
                    frame.texture = new Texture;
                    frame.texture->SetBitmap(raw_bitmap);
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
                        hr              = UIntMult(prop_val.uiVal, 10, &udelay);
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
        KGE_WARN(L"Load GIF frame failed with HRESULT of %08X!", hr);
    }
}

void Renderer::CreateFontCollection(Font& font, String const& file_path)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        if (!FileSystem::Instance().IsFileExists(file_path))
        {
            KGE_WARN(L"Font file '%s' not found!", file_path.c_str());
            hr = E_FAIL;
        }
    }

    if (SUCCEEDED(hr))
    {
        LPVOID   key       = nullptr;
        uint32_t key_size  = 0;
        String   full_path = FileSystem::Instance().GetFullPathForFile(file_path);

        hr = font_collection_loader_->AddFilePaths({ full_path }, &key, &key_size);

        if (SUCCEEDED(hr))
        {
            ComPtr<IDWriteFontCollection> font_collection;
            hr = d2d_res_->GetDWriteFactory()->CreateCustomFontCollection(font_collection_loader_.get(), key, key_size,
                                                                          &font_collection);

            if (SUCCEEDED(hr))
            {
                font.SetCollection(font_collection);
            }
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateFontCollection(Font& font, Resource const& res)
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
            hr = d2d_res_->GetDWriteFactory()->CreateCustomFontCollection(res_font_collection_loader_.get(), key,
                                                                          key_size, &font_collection);

            if (SUCCEEDED(hr))
            {
                font.SetCollection(font_collection);
            }
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateTextFormat(TextLayout& layout)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<IDWriteTextFormat> output;
    if (SUCCEEDED(hr))
    {
        const TextStyle& style = layout.GetStyle();

        hr = d2d_res_->CreateTextFormat(output, style.font_family, style.font ? style.font->GetCollection() : nullptr,
                                        DWRITE_FONT_WEIGHT(style.font_weight),
                                        style.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
                                        DWRITE_FONT_STRETCH_NORMAL, style.font_size);
    }

    if (SUCCEEDED(hr))
    {
        layout.SetTextFormat(output);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateTextLayout(TextLayout& layout)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<IDWriteTextLayout> output;
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->CreateTextLayout(output, layout.GetText(), layout.GetTextFormat());
    }

    if (SUCCEEDED(hr))
    {
        layout.SetTextLayout(output);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateLineShape(Shape& shape, Point const& begin_pos, Point const& end_pos)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<ID2D1PathGeometry> path_geo;
    ComPtr<ID2D1GeometrySink> path_sink;
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->GetFactory()->CreatePathGeometry(&path_geo);
    }

    if (SUCCEEDED(hr))
    {
        hr = path_geo->Open(&path_sink);
    }

    if (SUCCEEDED(hr))
    {
        path_sink->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
        path_sink->AddLine(DX::ConvertToPoint2F(end_pos));
        path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
        hr = path_sink->Close();
    }

    if (SUCCEEDED(hr))
    {
        shape.SetGeometry(path_geo);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateRectShape(Shape& shape, Rect const& rect)
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
        shape.SetGeometry(output);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateRoundedRectShape(Shape& shape, Rect const& rect, Vec2 const& radius)
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
        shape.SetGeometry(output);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateEllipseShape(Shape& shape, Point const& center, Vec2 const& radius)
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
        shape.SetGeometry(output);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateShapeSink(ShapeSink& sink)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    ComPtr<ID2D1PathGeometry> output;
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->GetFactory()->CreatePathGeometry(&output);
    }

    if (SUCCEEDED(hr))
    {
        sink.SetPathGeometry(output);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateTextureRenderTarget(TextureRenderContextPtr& render_context)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    TextureRenderContextPtr output;
    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1BitmapRenderTarget> bitmap_rt;
        hr = d2d_res_->GetDeviceContext()->CreateCompatibleRenderTarget(&bitmap_rt);

        if (SUCCEEDED(hr))
        {
            output = new TextureRenderContext;
            hr     = output->CreateDeviceResources(d2d_res_->GetFactory(), bitmap_rt);
        }

        if (SUCCEEDED(hr))
        {
            output->SetBitmapRenderTarget(bitmap_rt);
        }
    }

    if (SUCCEEDED(hr))
    {
        render_context = output;
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateBrush(Brush& brush, Color const& color)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<ID2D1SolidColorBrush> output;
        hr = d2d_res_->GetDeviceContext()->CreateSolidColorBrush(DX::ConvertToColorF(color), &output);

        if (SUCCEEDED(hr))
        {
            brush.SetBrush(output, Brush::Type::SolidColor);
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateBrush(Brush& brush, LinearGradientStyle const& style)
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
                collection.get(), &output);

            if (SUCCEEDED(hr))
            {
                brush.SetBrush(output, Brush::Type::LinearGradient);
            }
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateBrush(Brush& brush, RadialGradientStyle const& style)
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
                collection.get(), &output);

            if (SUCCEEDED(hr))
            {
                brush.SetBrush(output, Brush::Type::RadialGradient);
            }
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::CreateStrokeStyle(StrokeStyle& stroke_style, CapStyle cap, LineJoinStyle line_join,
                                 const float* dash_array, size_t dash_size, float dash_offset)
{
    HRESULT hr = S_OK;
    if (!d2d_res_)
    {
        hr = E_UNEXPECTED;
    }

    if (SUCCEEDED(hr))
    {
        D2D1_STROKE_STYLE_PROPERTIES style =
            D2D1::StrokeStyleProperties(D2D1_CAP_STYLE(cap), D2D1_CAP_STYLE(cap), D2D1_CAP_STYLE(cap),
                                        D2D1_LINE_JOIN(line_join), 10.0f, D2D1_DASH_STYLE_CUSTOM, dash_offset);

        ComPtr<ID2D1StrokeStyle> output;
        hr = d2d_res_->GetFactory()->CreateStrokeStyle(style, dash_array, dash_size, &output);

        if (SUCCEEDED(hr))
        {
            stroke_style.SetStrokeStyle(output);
        }
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::SetDpi(float dpi)
{
    KGE_ASSERT(d3d_res_ && d2d_res_);

    HRESULT hr = d3d_res_->SetDpi(dpi);
    if (SUCCEEDED(hr))
    {
        hr = d2d_res_->SetDpi(dpi);
    }

    win32::ThrowIfFailed(hr);
}

void Renderer::SetVSyncEnabled(bool enabled)
{
    vsync_ = enabled;
}

void Renderer::SetClearColor(const Color& color)
{
    clear_color_ = color;
}

void Renderer::ResizeTarget(uint32_t width, uint32_t height)
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
        hr = d2d_res_->SetLogicalSize(output_size_);
    }

    if (SUCCEEDED(hr))
    {
        render_ctx_.Resize(output_size_);
    }

    win32::ThrowIfFailed(hr);
}

}  // namespace kiwano
