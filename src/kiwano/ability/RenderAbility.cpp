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

#include <kiwano/ability/RenderAbility.h>
#include <kiwano/actor/Actor.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

//
// RenderAbility
//

RenderAbility::RenderAbility()
    : visible_(true)
{
    SetName("__KGE_RENDER_COMPONENT__");
}

bool RenderAbility::CheckVisibility(RenderContext& ctx)
{
    auto actor = GetAttachedActor();
    if (!actor)
    {
        visible_ = false;
        return visible_;
    }

    // TODO: friend class 的侵入设计不太好
    if (actor->dirty_flag_.Has(Actor::DirtyFlag::DirtyVisibility))
    {
        actor->dirty_flag_.Unset(Actor::DirtyFlag::DirtyVisibility);

        const auto actor = GetAttachedActor();
        if (actor->GetSize().IsOrigin())
        {
            visible_ = false;
        }
        else
        {
            visible_ = ctx.CheckVisibility(actor->GetBounds(), actor->GetTransformMatrix());
        }
    }
    return visible_;
}

void RenderAbility::OnRender(RenderContext& ctx)
{
    if (CheckVisibility(ctx))
    {
        const auto actor = GetAttachedActor();
        ctx.SetTransform(actor->GetTransformMatrix());
        ctx.SetBrushOpacity(actor->GetDisplayedOpacity());
    }
}

void RenderAbility::OnUpdate(Duration dt)
{
    Component::OnUpdate(dt);
    Renderer::GetInstance().PushRenderObject(this);

    Hold(this);
}

void RenderAbility::AfterRender(RenderContext& ctx)
{
    Unhold(this);
}

//
// TextureRenderAbility
//

namespace
{
// 预留出描边的空间
static const Point cached_text_texture_offset = Point{ 5, 5 };
}  // namespace

TextureRenderAbility::TextureRenderAbility()
    : TextureRenderAbility(nullptr)
{
}

TextureRenderAbility::TextureRenderAbility(RefPtr<Texture> texture, const Rect& src_rect, const Rect& dest_rect)
    : texture_(texture)
    , src_rect_(src_rect)
    , dest_rect_(dest_rect)
{
}

void TextureRenderAbility::OnRender(RenderContext& ctx)
{
    RenderAbility::OnRender(ctx);
    if (!IsVisible())
        return;

    if (texture_ && texture_->IsValid())
    {
        auto src_rect = src_rect_.IsEmpty() ? nullptr : &src_rect_;
        ctx.DrawTexture(*texture_, src_rect, &(GetAttachedActor()->GetBounds()));
    }
}

bool TextureRenderAbility::CheckVisibility(RenderContext& ctx)
{
    return texture_ && texture_->IsValid() && RenderAbility::CheckVisibility(ctx);
}

//
// TextRenderAbility
//

TextRenderAbility::TextRenderAbility()
    : TextRenderAbility(nullptr)
{
}

TextRenderAbility::TextRenderAbility(RefPtr<TextLayout> text_layout)
    : is_cache_dirty_(false)
    , layout_(text_layout)
{
}

void TextRenderAbility::SetPreRenderEnabled(bool enable)
{
    const bool enabled = texture_cached_ != nullptr;
    if (enabled != enable)
    {
        if (enable)
        {
            texture_cached_ = MakePtr<Texture>();
        }
        else
        {
            texture_cached_ = nullptr;
        }
        render_ctx_     = nullptr;
        is_cache_dirty_ = true;
    }
}

void TextRenderAbility::OnRender(RenderContext& ctx)
{
    RenderAbility::OnRender(ctx);
    if (!IsVisible())
        return;

    if (layout_)
    {
        if (texture_cached_)
        {
            Rect dest_rect = GetAttachedActor()->GetBounds();
            dest_rect.left_top -= cached_text_texture_offset;
            dest_rect.right_bottom += cached_text_texture_offset;
            ctx.DrawTexture(*texture_cached_, nullptr, &dest_rect);
        }
        else
        {
            ctx.SetCurrentBrush(fill_brush_);
            ctx.SetCurrentStrokeStyle(outline_stroke_);
            ctx.DrawTextLayout(*layout_, Point{}, outline_brush_);
        }
    }
}

bool TextRenderAbility::CheckVisibility(RenderContext& ctx)
{
    return layout_ && layout_->IsValid() && RenderAbility::CheckVisibility(ctx);
}

void TextRenderAbility::UpdateCachedTexture()
{
    if (!is_cache_dirty_ || !texture_cached_)
        return;

    const auto expectedSize = layout_->GetSize() + cached_text_texture_offset * 2;
    if (!render_ctx_)
    {
        const auto pixelSize = PixelSize((uint32_t)math::Ceil(expectedSize.x), (uint32_t)math::Ceil(expectedSize.y));
        render_ctx_          = RenderContext::Create(texture_cached_, pixelSize);
    }
    else if (render_ctx_->GetSize() != expectedSize)
    {
        render_ctx_->Resize(expectedSize);
    }

    render_ctx_->BeginDraw();
    render_ctx_->Clear();
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->SetCurrentStrokeStyle(outline_stroke_);
    render_ctx_->DrawTextLayout(*layout_, cached_text_texture_offset, outline_brush_);
    render_ctx_->EndDraw();
    is_cache_dirty_ = false;
}

//
// ShapeRenderAbility
//

ShapeRenderAbility::ShapeRenderAbility() {}

ShapeRenderAbility::ShapeRenderAbility(RefPtr<Shape> shape, RefPtr<Brush> fill_brush, RefPtr<Brush> stroke_brush)
    : shape_(shape)
    , fill_brush_(fill_brush)
    , stroke_brush_(stroke_brush)
{
}

void ShapeRenderAbility::OnRender(RenderContext& ctx)
{
    RenderAbility::OnRender(ctx);
    if (!IsVisible())
        return;

    if (shape_)
    {
        if (stroke_brush_)
        {
            ctx.SetCurrentBrush(stroke_brush_);
            ctx.SetCurrentStrokeStyle(stroke_style_);
            ctx.DrawShape(*shape_);
        }

        if (fill_brush_)
        {
            ctx.SetCurrentBrush(fill_brush_);
            ctx.FillShape(*shape_);
        }
    }
}

bool ShapeRenderAbility::CheckVisibility(RenderContext& ctx)
{
    return shape_ && RenderAbility::CheckVisibility(ctx);
}

//
// LayerRenderAbility
//

LayerRenderAbility::LayerRenderAbility(const Layer& layer)
    : layer_(layer)
{
}

void LayerRenderAbility::OnRender(RenderContext& ctx)
{
    auto actor       = GetAttachedActor();
    layer_.transform = actor->GetTransformMatrix();  // TODO: refactor
    Renderer::GetInstance().PushRenderGroup(layer_);
}

void LayerRenderAbility::AfterRender(RenderContext& ctx)
{
    // TODO 有问题
    Renderer::GetInstance().PopRenderGroup();
}

bool LayerRenderAbility::CheckVisibility(RenderContext& ctx)
{
    return true;
}

}  // namespace kiwano
