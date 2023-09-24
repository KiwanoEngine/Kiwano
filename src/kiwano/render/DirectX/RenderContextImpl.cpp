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

    render_ctx_ = ctx;
    text_renderer_.Reset();
    current_brush_.Reset();

    HRESULT hr = ITextRenderer::Create(&text_renderer_, render_ctx_.Get());

    if (SUCCEEDED(hr))
    {
        SetAntialiasMode(antialias_);
        SetTextAntialiasMode(text_antialias_);

        Resize(reinterpret_cast<const Size&>(render_ctx_->GetSize()));
    }

    // DrawingStateBlock
    if (SUCCEEDED(hr))
    {
        hr = factory->CreateDrawingStateBlock(&drawing_state_);
    }

    if (SUCCEEDED(hr))
    {
        NativeObject::Set(this, ctx);
    }
    return hr;
}

void RenderContextImpl::DiscardDeviceResources()
{
    text_renderer_.Reset();
    render_ctx_.Reset();
    current_brush_.Reset();

    ResetNativePointer();
}

TexturePtr RenderContextImpl::GetTarget() const
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    ComPtr<ID2D1Image> target;
    render_ctx_->GetTarget(&target);
    if (target)
    {
        TexturePtr ptr = MakePtr<Texture>();
        NativeObject::Set(*ptr, target.Get());
    }
    return nullptr;
}

void RenderContextImpl::BeginDraw()
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    SaveDrawingState();

    RenderContext::BeginDraw();

    render_ctx_->BeginDraw();
}

void RenderContextImpl::EndDraw()
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    HRESULT hr = render_ctx_->EndDraw();
    KGE_THROW_IF_FAILED(hr, "ID2D1RenderTarget EndDraw failed");

    RenderContext::EndDraw();

    RestoreDrawingState();
}

void RenderContextImpl::CreateTexture(Texture& texture, const PixelSize& size)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    ComPtr<ID2D1Bitmap> saved_bitmap;

    HRESULT hr = render_ctx_->CreateBitmap(D2D1::SizeU(size.x, size.y), D2D1::BitmapProperties(), &saved_bitmap);

    if (SUCCEEDED(hr))
    {
        NativeObject::Set(texture, saved_bitmap);
    }

    KGE_THROW_IF_FAILED(hr, "Create texture failed");
}

void RenderContextImpl::DrawTexture(const Texture& texture, const Rect* src_rect, const Rect* dest_rect)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

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

        auto bitmap = NativeObject::Get<ID2D1Bitmap>(texture);
        render_ctx_->DrawBitmap(bitmap.Get(), dest_rect ? &DX::ConvertToRectF(*dest_rect) : nullptr, brush_opacity_,
                                   mode, src_rect ? &DX::ConvertToRectF(*src_rect) : nullptr);

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::DrawTextLayout(const TextLayout& layout, const Point& offset, BrushPtr current_outline_brush)
{
    KGE_ASSERT(text_renderer_ && "Text renderer has not been initialized!");

    if (layout.IsValid())
    {
        auto  native         = NativeObject::Get<IDWriteTextLayout>(layout);
        auto  fill_brush     = NativeObject::Get<ID2D1Brush>(current_brush_);
        auto  outline_brush  = NativeObject::Get<ID2D1Brush>(current_outline_brush);
        auto  outline_stroke = NativeObject::Get<ID2D1StrokeStyle>(current_stroke_);
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
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    if (shape.IsValid())
    {
        auto  geometry     = NativeObject::Get<ID2D1Geometry>(shape);
        auto  brush        = NativeObject::Get<ID2D1Brush>(current_brush_);
        auto  stroke_style = NativeObject::Get<ID2D1StrokeStyle>(current_stroke_);
        float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

        render_ctx_->DrawGeometry(geometry.Get(), brush.Get(), stroke_width, stroke_style.Get());

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::DrawLine(const Point& point1, const Point& point2)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativeObject::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativeObject::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_ctx_->DrawLine(DX::ConvertToPoint2F(point1), DX::ConvertToPoint2F(point2), brush.Get(), stroke_width,
                             stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawRectangle(const Rect& rect)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativeObject::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativeObject::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_ctx_->DrawRectangle(DX::ConvertToRectF(rect), brush.Get(), stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawRoundedRectangle(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativeObject::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativeObject::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_ctx_->DrawRoundedRectangle(D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), brush.Get(),
                                         stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::DrawEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto  brush        = NativeObject::Get<ID2D1Brush>(current_brush_);
    auto  stroke_style = NativeObject::Get<ID2D1StrokeStyle>(current_stroke_);
    float stroke_width = current_stroke_ ? current_stroke_->GetWidth() : 1.0f;

    render_ctx_->DrawEllipse(D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), brush.Get(),
                                stroke_width, stroke_style.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillShape(const Shape& shape)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    if (shape.IsValid())
    {
        auto brush    = NativeObject::Get<ID2D1Brush>(current_brush_);
        auto geometry = NativeObject::Get<ID2D1Geometry>(shape);
        render_ctx_->FillGeometry(geometry.Get(), brush.Get());

        IncreasePrimitivesCount();
    }
}

void RenderContextImpl::FillRectangle(const Rect& rect)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = NativeObject::Get<ID2D1Brush>(current_brush_);
    render_ctx_->FillRectangle(DX::ConvertToRectF(rect), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillRoundedRectangle(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = NativeObject::Get<ID2D1Brush>(current_brush_);
    render_ctx_->FillRoundedRectangle(D2D1::RoundedRect(DX::ConvertToRectF(rect), radius.x, radius.y), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::FillEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

    auto brush = NativeObject::Get<ID2D1Brush>(current_brush_);
    render_ctx_->FillEllipse(D2D1::Ellipse(DX::ConvertToPoint2F(center), radius.x, radius.y), brush.Get());

    IncreasePrimitivesCount();
}

void RenderContextImpl::PushClipRect(const Rect& clip_rect)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    D2D1_ANTIALIAS_MODE mode;
    if (antialias_)
    {
        mode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    }
    else
    {
        mode = D2D1_ANTIALIAS_MODE_ALIASED;
    }
    render_ctx_->PushAxisAlignedClip(DX::ConvertToRectF(clip_rect), mode);
}

void RenderContextImpl::PopClipRect()
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    render_ctx_->PopAxisAlignedClip();
}

void RenderContextImpl::PushLayer(Layer& layer)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    auto native = NativeObject::Get<ID2D1Layer>(layer);
    auto mask   = NativeObject::Get<ID2D1Geometry>(layer.GetMaskShape());

    if (!native)
    {
        HRESULT hr = render_ctx_->CreateLayer(&native);

        if (SUCCEEDED(hr))
        {
            NativeObject::Set(layer, native);
        }
        KGE_THROW_IF_FAILED(hr, "Create ID2D1Layer failed");
    }

    auto params = D2D1::LayerParameters(DX::ConvertToRectF(layer.GetClipRect()), mask.Get(),
                                        antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED,
                                        DX::ConvertToMatrix3x2F(layer.GetMaskTransform()), layer.GetOpacity(), nullptr,
                                        D2D1_LAYER_OPTIONS_NONE);

    render_ctx_->PushLayer(params, native.Get());
}

void RenderContextImpl::PopLayer()
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    render_ctx_->PopLayer();
}

void RenderContextImpl::Clear()
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    render_ctx_->Clear();
}

void RenderContextImpl::Clear(const Color& clear_color)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    render_ctx_->Clear(DX::ConvertToColorF(clear_color));
}

Size RenderContextImpl::GetSize() const
{
    if (render_ctx_)
    {
        return reinterpret_cast<const Size&>(render_ctx_->GetSize());
    }
    return Size();
}

void RenderContextImpl::SetCurrentBrush(BrushPtr brush)
{
    RenderContext::SetCurrentBrush(brush);

    if (current_brush_ && current_brush_->IsValid())
    {
        NativeObject::Get<ID2D1Brush>(current_brush_)->SetOpacity(brush_opacity_);
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
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    if (fast_global_transform_)
    {
        render_ctx_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
    }
    else
    {
        Matrix3x2 result = matrix * global_transform_;
        render_ctx_->SetTransform(DX::ConvertToMatrix3x2F(&result));
    }
}

void RenderContextImpl::SetBlendMode(BlendMode blend)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
    render_ctx_->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND(blend));
}

void RenderContextImpl::SetAntialiasMode(bool enabled)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

    render_ctx_->SetAntialiasMode(enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED);
    antialias_ = enabled;
}

void RenderContextImpl::SetTextAntialiasMode(TextAntialiasMode mode)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

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
    render_ctx_->SetTextAntialiasMode(antialias_mode);
}

bool RenderContextImpl::CheckVisibility(const Rect& bounds, const Matrix3x2& transform)
{
    KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

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
        render_ctx_->SaveDrawingState(drawing_state_.Get());
    }
}

void RenderContextImpl::RestoreDrawingState()
{
    KGE_ASSERT(IsValid());

    if (drawing_state_)
    {
        render_ctx_->RestoreDrawingState(drawing_state_.Get());
    }
}

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
