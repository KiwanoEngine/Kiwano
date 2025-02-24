// Copyright (c) 2016-2019 Kiwano - Nomango
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

#include <kiwano/render/DirectX/RenderContextImpl.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{
RenderContextImpl::RenderContextImpl() {}

RenderContextImpl::~RenderContextImpl()
{
    DiscardDeviceResources();
}

HRESULT RenderContextImpl::CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1DeviceContext> ctx)
{
    if (!factory || !ctx)
        return E_INVALIDARG;

    device_ctx_ = ctx;
    text_renderer_.Reset();
    current_brush_.Reset();

    HRESULT hr = ITextRenderer::Create(&text_renderer_, device_ctx_.Get());

    if (SUCCEEDED(hr))
    {
        SetAntialiasMode(antialias_);
        SetTextAntialiasMode(text_antialias_);

        Resize(reinterpret_cast<const Size&>(device_ctx_->GetSize()));
    }

    // DrawingStateBlock
    if (SUCCEEDED(hr))
    {
        hr = factory->CreateDrawingStateBlock(&drawing_state_);
    }

    if (SUCCEEDED(hr))
    {
        ComPolicy::Set(this, ctx);
    }
    return hr;
}

void RenderContextImpl::DiscardDeviceResources()
{
    text_renderer_.Reset();
    device_ctx_.Reset();
    current_brush_.Reset();

    ComPolicy::Set(this, nullptr);
}

RefPtr<Image> RenderContextImpl::GetTarget() const
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    ComPtr<ID2D1Image> target;
    device_ctx_->GetTarget(&target);
    if (target)
    {
        RefPtr<Image> ptr = MakePtr<Image>();
        ComPolicy::Set(*ptr, target.Get());
        return ptr;
    }
    return nullptr;
}

void RenderContextImpl::SetTarget(const Image& bitmap)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    auto target = ComPolicy::Get<ID2D1Image>(bitmap);
    device_ctx_->SetTarget(target.Get());
}

void RenderContextImpl::BeginDraw()
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    SaveDrawingState();

    RenderContext::BeginDraw();
    device_ctx_->BeginDraw();
}

void RenderContextImpl::EndDraw()
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    HRESULT hr = device_ctx_->EndDraw();
    KGE_THROW_IF_FAILED(hr, "ID2D1DeviceContext EndDraw failed");
    RenderContext::EndDraw();

    RestoreDrawingState();
}

void RenderContextImpl::DrawImage(const Image& image, const Rect* src_rect)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    if (image.IsValid())
    {
        D2D1_INTERPOLATION_MODE mode;
        if (image.GetInterpolationMode() == InterpolationMode::Linear)
        {
            mode = D2D1_INTERPOLATION_MODE_LINEAR;
        }
        else
        {
            mode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        }

        auto d2d_image = ComPolicy::Get<ID2D1Image>(image);
        device_ctx_->DrawImage(d2d_image.Get(), nullptr, DX::ConvertToRectF(src_rect), mode,
                               D2D1_COMPOSITE_MODE_SOURCE_OVER);

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::CreateBitmap(Bitmap& bitmap, const PixelSize& size)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    ComPtr<ID2D1Bitmap> saved_bitmap;

    FLOAT dpi_x = 0.f, dpi_y = 0.f;
    device_ctx_->GetDpi(&dpi_x, &dpi_y);

    HRESULT hr = device_ctx_->CreateBitmap(D2D1::SizeU(size.x, size.y),
                                           D2D1::BitmapProperties(D2D1::PixelFormat(), dpi_x, dpi_y), &saved_bitmap);

    if (SUCCEEDED(hr))
    {
        ComPolicy::Set(bitmap, saved_bitmap);
    }

    KGE_THROW_IF_FAILED(hr, "Create bitmap failed");
}

void RenderContextImpl::DrawBitmap(const Bitmap& bitmap, const Rect* src_rect, const Rect* dest_rect)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    if (bitmap.IsValid())
    {
        D2D1_BITMAP_INTERPOLATION_MODE mode;
        if (bitmap.GetInterpolationMode() == InterpolationMode::Linear)
        {
            mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
        }
        else
        {
            mode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        }

        auto d2d_bitmap = ComPolicy::Get<ID2D1Bitmap>(bitmap);
        device_ctx_->DrawBitmap(d2d_bitmap.Get(), dest_rect ? &DX::ConvertToRectF(*dest_rect) : nullptr, brush_opacity_,
                                mode, src_rect ? &DX::ConvertToRectF(*src_rect) : nullptr);

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::DrawTextLayout(const TextLayout& layout, const Point& offset,
                                       RefPtr<Brush> current_outline_brush)
{
    KGE_ASSERT(text_renderer_ && "Text renderer has not been initialized!");

    if (layout.IsValid())
    {
        auto  native         = ComPolicy::Get<IDWriteTextLayout>(layout);
        auto  fill_brush     = ComPolicy::Get<ID2D1Brush>(current_brush_);
        auto  outline_brush  = ComPolicy::Get<ID2D1Brush>(current_outline_brush);
        auto  outline_stroke = ComPolicy::Get<ID2D1StrokeStyle>(current_stroke_);
        float outline_width  = 1.0f;

        if (fill_brush)
        {
            fill_brush->SetOpacity(brush_opacity_);
        }

        if (outline_brush)
        {
            outline_brush->SetOpacity(brush_opacity_);
        }

        if (current_stroke_)
        {
            outline_width = current_stroke_->GetWidth();
        }

        HRESULT hr = text_renderer_->DrawTextLayout(native.Get(), offset.x, offset.y, fill_brush.Get(),
                                                    outline_brush.Get(), outline_width, outline_stroke.Get());

        if (SUCCEEDED(hr))
        {
            IncreasePrimitivesCount(text_renderer_->GetLastPrimitivesCount());
        }
        else
        {
            KGE_ERRORF("Failed to draw text layout with HRESULT of %08X", hr);
        }
    }
}

void RenderContextImpl::DrawShape(const Shape& shape)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    if (shape.IsValid())
    {
        auto  geometry     = ComPolicy::Get<ID2D1Geometry>(shape);
        auto  brush        = ComPolicy::Get<ID2D1Brush>(current_brush_);
        auto  stroke_style = ComPolicy::Get<ID2D1StrokeStyle>(current_stroke_);
        float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

        device_ctx_->DrawGeometry(geometry.Get(), brush.Get(), stroke_width, stroke_style.Get());

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::DrawLine(const Point& point1, const Point& point2)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = ComPolicy::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = ComPolicy::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    device_ctx_->DrawLine(DX::ConvertToPoint2F(point1), DX::ConvertToPoint2F(point2), brush.Get(), stroke_width,
                          stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawRectangle(const Rect& rect)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = ComPolicy::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = ComPolicy::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    device_ctx_->DrawRectangle(DX::ConvertToRectF(rect), brush.Get(), stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawRoundedRectangle(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = ComPolicy::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = ComPolicy::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    device_ctx_->DrawRoundedRectangle(D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), brush.Get(),
                                      stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = ComPolicy::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = ComPolicy::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    device_ctx_->DrawEllipse(D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), brush.Get(), stroke_width,
                             stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillShape(const Shape& shape)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    if (shape.IsValid())
    {
        auto brush    = ComPolicy::Get<ID2D1Brush>(current_brush_);
        auto geometry = ComPolicy::Get<ID2D1Geometry>(shape);
        device_ctx_->FillGeometry(geometry.Get(), brush.Get());

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::FillRectangle(const Rect& rect)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = ComPolicy::Get<ID2D1Brush>(current_brush_);
    device_ctx_->FillRectangle(DX::ConvertToRectF(rect), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillRoundedRectangle(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = ComPolicy::Get<ID2D1Brush>(current_brush_);
    device_ctx_->FillRoundedRectangle(D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = ComPolicy::Get<ID2D1Brush>(current_brush_);
    device_ctx_->FillEllipse(D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::PushClipRect(const Rect& clip_rect)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    D2D1_ANTIALIAS_MODE mode;
    if (antialias_)
    {
        mode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    }
    else
    {
        mode = D2D1_ANTIALIAS_MODE_ALIASED;
    }
    device_ctx_->PushAxisAlignedClip(DX::ConvertToRectF(clip_rect), mode);
}

void RenderContextImpl::PopClipRect()
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    device_ctx_->PopAxisAlignedClip();
}

void RenderContextImpl::PushLayer(Layer& layer)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    auto mask   = ComPolicy::Get<ID2D1Geometry>(layer.mask);
    auto params = D2D1::LayerParameters1(DX::ConvertToRectF(layer.bounds), mask.Get(),
                                         antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED,
                                         DX::ConvertToMatrix3x2F(layer.mask_transform), layer.opacity, nullptr,
                                         D2D1_LAYER_OPTIONS1_NONE);

    device_ctx_->PushLayer(params, nullptr);
}

void RenderContextImpl::PopLayer()
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    device_ctx_->PopLayer();
}

void RenderContextImpl::Clear()
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    device_ctx_->Clear();
}

void RenderContextImpl::Clear(const Color& clear_color)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    device_ctx_->Clear(DX::ConvertToColorF(clear_color));
}

Size RenderContextImpl::GetSize() const
{
    if (device_ctx_)
    {
        return reinterpret_cast<const Size&>(device_ctx_->GetSize());
    }
    return Size();
}

void RenderContextImpl::SetCurrentBrush(RefPtr<Brush> brush)
{
    RenderContext::SetCurrentBrush(brush);

    if (current_brush_ && current_brush_->IsValid())
    {
        ComPolicy::Get<ID2D1Brush>(current_brush_)->SetOpacity(brush_opacity_);
    }
}

void RenderContextImpl::SetCurrentStrokeStyle(RefPtr<StrokeStyle> stroke_style)
{
    RenderContext::SetCurrentStrokeStyle(stroke_style);

    if (current_stroke_ && !current_stroke_->IsValid())
    {
        Renderer::GetInstance().CreateStrokeStyle(*current_stroke_);
    }
}

Matrix3x2 RenderContextImpl::GetTransform() const
{
    Matrix3x2 transform;
    device_ctx_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(&transform));
    return std::move(transform);
}

void RenderContextImpl::SetTransform(const Matrix3x2& matrix)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    device_ctx_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
}

void RenderContextImpl::SetBlendMode(BlendMode blend)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    device_ctx_->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND(blend));
}

void RenderContextImpl::SetAntialiasMode(bool enabled)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    device_ctx_->SetAntialiasMode(enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED);
    antialias_ = enabled;
}

void RenderContextImpl::SetTextAntialiasMode(TextAntialiasMode mode)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");

    D2D1_TEXT_ANTIALIAS_MODE antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
    switch (mode)
    {
    case TextAntialiasMode::Default:
        antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
        break;
    case TextAntialiasMode::ClearType:
        antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
        break;
    case TextAntialiasMode::GrayScale:
        antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE;
        break;
    case TextAntialiasMode::None:
        antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
        break;
    default:
        break;
    }

    text_antialias_ = mode;
    device_ctx_->SetTextAntialiasMode(antialias_mode);
}

bool RenderContextImpl::CheckVisibility(const Rect& bounds, const Matrix3x2& transform)
{
    KGE_ASSERT(device_ctx_ && "Render target has not been initialized!");
    return visible_size_.Intersects(transform.Transform(bounds));
}

void RenderContextImpl::Resize(const Size& size)
{
    visible_size_ = Rect(Point(), size);
}

void RenderContextImpl::SaveDrawingState()
{
    KGE_ASSERT(IsValid());

    if (drawing_state_)
    {
        device_ctx_->SaveDrawingState(drawing_state_.Get());
    }
}

void RenderContextImpl::RestoreDrawingState()
{
    KGE_ASSERT(IsValid());

    if (drawing_state_)
    {
        device_ctx_->RestoreDrawingState(drawing_state_.Get());
    }
}

HRESULT CommandListRenderContextImpl::CreateDeviceResources(ComPtr<ID2D1Factory>       factory,
                                                            ComPtr<ID2D1DeviceContext> ctx)
{
    HRESULT hr = RenderContextImpl::CreateDeviceResources(factory, ctx);
    if (SUCCEEDED(hr))
    {
        hr = device_ctx_->CreateCommandList(&cmd_list_);
    }
    return hr;
}

RefPtr<Image> CommandListRenderContextImpl::GetTarget() const
{
    RefPtr<Image> ptr = MakePtr<Image>();
    ComPolicy::Set(*ptr, cmd_list_.Get());
    return ptr;
}

void CommandListRenderContextImpl::SetTarget(const Image& target)
{
    KGE_THROW_IF_FAILED(E_NOTIMPL, "Cannot set target to command list");
}

void CommandListRenderContextImpl::BeginDraw()
{
    device_ctx_->GetTarget(&original_target_);
    device_ctx_->SetTarget(cmd_list_.Get());

    RenderContextImpl::BeginDraw();
}

void CommandListRenderContextImpl::EndDraw()
{
    RenderContextImpl::EndDraw();

    HRESULT hr = cmd_list_->Close();
    KGE_THROW_IF_FAILED(hr, "ID2D1CommandList Close failed");

    device_ctx_->SetTarget(original_target_.Get());
    original_target_ = nullptr;
}

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
