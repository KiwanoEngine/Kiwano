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
#include <kiwano/render/DirectX/NativePtr.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

RenderContextImpl::RenderContextImpl() {}

RenderContextImpl::~RenderContextImpl()
{
    DiscardDeviceResources();
}

HRESULT RenderContextImpl::CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> render_target)
{
    if (!factory || !render_target)
        return E_INVALIDARG;

    render_target_ = render_target;
    text_renderer_.Reset();
    current_brush_.Reset();

    HRESULT hr = ITextRenderer::Create(&text_renderer_, render_target_.Get());

    if (SUCCEEDED(hr))
    {
        SetAntialiasMode(antialias_);
        SetTextAntialiasMode(text_antialias_);

        Resize(reinterpret_cast<const Size&>(render_target->GetSize()));
    }

    // DrawingStateBlock
    if (SUCCEEDED(hr))
    {
        hr = factory->CreateDrawingStateBlock(&drawing_state_);
    }

    if (SUCCEEDED(hr))
    {
        NativePtr::Set(this, render_target);
    }
    return hr;
}

void RenderContextImpl::DiscardDeviceResources()
{
    text_renderer_.Reset();
    render_target_.Reset();
    current_brush_.Reset();

    ResetNativePointer();
}

void RenderContextImpl::BeginDraw()
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    SaveDrawingState();

    RenderContext::BeginDraw();

    render_target_->BeginDraw();
}

void RenderContextImpl::EndDraw()
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    HRESULT hr = render_target_->EndDraw();
    KGE_THROW_IF_FAILED(hr, "ID2D1RenderTarget EndDraw failed");

    RenderContext::EndDraw();

    RestoreDrawingState();
}

void RenderContextImpl::DrawTexture(const Texture& texture, const Rect* src_rect, const Rect* dest_rect)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    if (texture.IsValid())
    {
        D2D1_BITMAP_INTERPOLATION_MODE mode;
        if (texture.GetBitmapInterpolationMode() == InterpolationMode::Linear)
        {
            mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
        }
        else
        {
            mode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        }

        auto bitmap = NativePtr::Get<ID2D1Bitmap>(texture);
        render_target_->DrawBitmap(bitmap.Get(), dest_rect ? &DX::ConvertToRectF(*dest_rect) : nullptr, brush_opacity_,
                                   mode, src_rect ? &DX::ConvertToRectF(*src_rect) : nullptr);

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::DrawTextLayout(const TextLayout& layout, const Point& offset, BrushPtr current_outline_brush)
{
    KGE_ASSERT(text_renderer_ && "Text renderer has not been initialized!");

    if (layout.IsValid())
    {
        auto  native         = NativePtr::Get<IDWriteTextLayout>(layout);
        auto  fill_brush     = NativePtr::Get<ID2D1Brush>(current_brush_);
        auto  outline_brush  = NativePtr::Get<ID2D1Brush>(current_outline_brush);
        auto  outline_stroke = NativePtr::Get<ID2D1StrokeStyle>(current_stroke_);
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
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    if (shape.IsValid())
    {
        auto  geometry     = NativePtr::Get<ID2D1Geometry>(shape);
        auto  brush        = NativePtr::Get<ID2D1Brush>(current_brush_);
        auto  stroke_style = NativePtr::Get<ID2D1StrokeStyle>(current_stroke_);
        float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

        render_target_->DrawGeometry(geometry.Get(), brush.Get(), stroke_width, stroke_style.Get());

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::DrawLine(const Point& point1, const Point& point2)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativePtr::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativePtr::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_target_->DrawLine(DX::ConvertToPoint2F(point1), DX::ConvertToPoint2F(point2), brush.Get(), stroke_width,
                             stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawRectangle(const Rect& rect)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativePtr::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativePtr::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_target_->DrawRectangle(DX::ConvertToRectF(rect), brush.Get(), stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawRoundedRectangle(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativePtr::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativePtr::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_target_->DrawRoundedRectangle(D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), brush.Get(),
                                         stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativePtr::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativePtr::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_target_->DrawEllipse(D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), brush.Get(),
                                stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillShape(const Shape& shape)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    if (shape.IsValid())
    {
        auto brush    = NativePtr::Get<ID2D1Brush>(current_brush_);
        auto geometry = NativePtr::Get<ID2D1Geometry>(shape);
        render_target_->FillGeometry(geometry.Get(), brush.Get());

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::FillRectangle(const Rect& rect)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = NativePtr::Get<ID2D1Brush>(current_brush_);
    render_target_->FillRectangle(DX::ConvertToRectF(rect), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillRoundedRectangle(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = NativePtr::Get<ID2D1Brush>(current_brush_);
    render_target_->FillRoundedRectangle(D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = NativePtr::Get<ID2D1Brush>(current_brush_);
    render_target_->FillEllipse(D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::CreateTexture(Texture& texture, math::Vec2T<uint32_t> size)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    ComPtr<ID2D1Bitmap> saved_bitmap;

    HRESULT hr = render_target_->CreateBitmap(D2D1::SizeU(size.x, size.y), D2D1::BitmapProperties(), &saved_bitmap);

    if (SUCCEEDED(hr))
    {
        NativePtr::Set(texture, saved_bitmap);
    }

    KGE_THROW_IF_FAILED(hr, "Create texture failed");
}

void RenderContextImpl::PushClipRect(const Rect& clip_rect)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    D2D1_ANTIALIAS_MODE mode;
    if (antialias_)
    {
        mode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    }
    else
    {
        mode = D2D1_ANTIALIAS_MODE_ALIASED;
    }
    render_target_->PushAxisAlignedClip(DX::ConvertToRectF(clip_rect), mode);
}

void RenderContextImpl::PopClipRect()
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    render_target_->PopAxisAlignedClip();
}

void RenderContextImpl::PushLayer(Layer& layer)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    auto native = NativePtr::Get<ID2D1Layer>(layer);
    auto mask   = NativePtr::Get<ID2D1Geometry>(layer.GetMaskShape());

    if (!native)
    {
        HRESULT hr = render_target_->CreateLayer(&native);

        if (SUCCEEDED(hr))
        {
            NativePtr::Set(layer, native);
        }
        KGE_THROW_IF_FAILED(hr, "Create ID2D1Layer failed");
    }

    auto params = D2D1::LayerParameters(DX::ConvertToRectF(layer.GetClipRect()), mask.Get(),
                                        antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED,
                                        DX::ConvertToMatrix3x2F(layer.GetMaskTransform()), layer.GetOpacity(), nullptr,
                                        D2D1_LAYER_OPTIONS_NONE);

    render_target_->PushLayer(params, native.Get());
}

void RenderContextImpl::PopLayer()
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    render_target_->PopLayer();
}

void RenderContextImpl::Clear()
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    render_target_->Clear();
}

void RenderContextImpl::Clear(const Color& clear_color)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");
    render_target_->Clear(DX::ConvertToColorF(clear_color));
}

Size RenderContextImpl::GetSize() const
{
    if (render_target_)
    {
        return reinterpret_cast<const Size&>(render_target_->GetSize());
    }
    return Size();
}

void RenderContextImpl::SetCurrentBrush(BrushPtr brush)
{
    RenderContext::SetCurrentBrush(brush);

    if (current_brush_ && current_brush_->IsValid())
    {
        NativePtr::Get<ID2D1Brush>(current_brush_)->SetOpacity(brush_opacity_);
    }
}

void RenderContextImpl::SetCurrentStrokeStyle(StrokeStylePtr stroke_style)
{
    RenderContext::SetCurrentStrokeStyle(stroke_style);

    if (current_stroke_ && !current_stroke_->IsValid())
    {
        Renderer::GetInstance().CreateStrokeStyle(*current_stroke_);
    }
}

void RenderContextImpl::SetTransform(const Matrix3x2& matrix)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    if (fast_global_transform_)
    {
        render_target_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
    }
    else
    {
        Matrix3x2 result = matrix * global_transform_;
        render_target_->SetTransform(DX::ConvertToMatrix3x2F(&result));
    }
}

void RenderContextImpl::SetAntialiasMode(bool enabled)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    render_target_->SetAntialiasMode(enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED);
    antialias_ = enabled;
}

void RenderContextImpl::SetTextAntialiasMode(TextAntialiasMode mode)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

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
    render_target_->SetTextAntialiasMode(antialias_mode);
}

bool RenderContextImpl::CheckVisibility(const Rect& bounds, const Matrix3x2& transform)
{
    KGE_ASSERT(render_target_ && "Render target has not been initialized!");

    if (fast_global_transform_)
    {
        return visible_size_.Intersects(transform.Transform(bounds));
    }
    return visible_size_.Intersects(Matrix3x2(transform * global_transform_).Transform(bounds));
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
        render_target_->SaveDrawingState(drawing_state_.Get());
    }
}

void RenderContextImpl::RestoreDrawingState()
{
    KGE_ASSERT(IsValid());

    if (drawing_state_)
    {
        render_target_->RestoreDrawingState(drawing_state_.Get());
    }
}

}  // namespace kiwano
