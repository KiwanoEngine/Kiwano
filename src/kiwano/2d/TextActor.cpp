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

#include <kiwano/2d/TextActor.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

 // 预留出描边的空间
const Point cached_texture_offset = Point{ 5, 5 };

TextActor::TextActor()
    : is_cache_dirty_(false)
{
}

TextActor::TextActor(const String& text)
    : TextActor(text, TextStyle())
{
}

TextActor::TextActor(const String& text, const TextStyle& style)
    : TextActor()
{
    SetStyle(style);
    SetText(text);
}

TextActor::~TextActor() {}

void TextActor::OnRender(RenderContext& ctx)
{
    if (layout_)
    {
        if (texture_cached_)
        {
            Rect dest_rect = GetBounds();
            dest_rect.left_top -= cached_texture_offset;
            dest_rect.right_bottom -= cached_texture_offset;
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

Size TextActor::GetSize() const
{
    const_cast<TextActor*>(this)->UpdateDirtyLayout();
    return Actor::GetSize();
}

void TextActor::SetText(const String& text)
{
    if (!layout_)
    {
        layout_ = MakePtr<TextLayout>();
    }

    try
    {
        layout_->Reset(text, style_);
        content_ = text;
        is_cache_dirty_ = true;
    }
    catch (SystemError& e)
    {
        Fail(String("TextActor::SetText failed: ") + e.what());
    }
}

void TextActor::SetStyle(const TextStyle& style)
{
    is_cache_dirty_ = true;
    style_          = style;
    if (layout_)
        layout_->Reset(content_, style);
}

void TextActor::SetFont(FontPtr font)
{
    if (style_.font != font)
    {
        is_cache_dirty_ = true;
        style_.font     = font;
        if (layout_)
            layout_->SetFont(font);
    }
}

void TextActor::SetUnderline(bool enable)
{
    if (style_.show_underline != enable)
    {
        is_cache_dirty_       = true;
        style_.show_underline = enable;
        if (layout_)
            layout_->SetUnderline(enable);
    }
}

void TextActor::SetStrikethrough(bool enable)
{
    if (style_.show_strikethrough != enable)
    {
        is_cache_dirty_           = true;
        style_.show_strikethrough = enable;
        if (layout_)
            layout_->SetStrikethrough(enable);
    }
}

void TextActor::SetWrapWidth(float wrap_width)
{
    if (style_.wrap_width != wrap_width)
    {
        is_cache_dirty_   = true;
        style_.wrap_width = wrap_width;
        if (layout_)
            layout_->SetWrapWidth(wrap_width);
    }
}

void TextActor::SetLineSpacing(float line_spacing)
{
    if (style_.line_spacing != line_spacing)
    {
        is_cache_dirty_     = true;
        style_.line_spacing = line_spacing;
        if (layout_)
            layout_->SetLineSpacing(line_spacing);
    }
}

void TextActor::SetAlignment(TextAlign align)
{
    if (style_.alignment != align)
    {
        is_cache_dirty_  = true;
        style_.alignment = align;
        if (layout_)
            layout_->SetAlignment(align);
    }
}

void TextActor::SetFillBrush(BrushPtr brush)
{
    fill_brush_     = brush;
    is_cache_dirty_ = true;
}

void TextActor::SetOutlineBrush(BrushPtr brush)
{
    outline_brush_  = brush;
    is_cache_dirty_ = true;
}

void TextActor::SetOutlineStrokeStyle(StrokeStylePtr stroke)
{
    outline_stroke_ = stroke;
    is_cache_dirty_ = true;
}

void TextActor::SetFillColor(const Color& color)
{
    if (fill_brush_)
    {
        is_cache_dirty_ = true;
        fill_brush_->SetColor(color);
    }
    else
    {
        SetFillBrush(MakePtr<Brush>(color));
    }
}

void TextActor::SetOutlineColor(const Color& outline_color)
{
    if (outline_brush_)
    {
        is_cache_dirty_ = true;
        outline_brush_->SetColor(outline_color);
    }
    else
    {
        SetOutlineBrush(MakePtr<Brush>(outline_color));
    }
}

void TextActor::SetTextLayout(TextLayoutPtr layout)
{
    if (layout_ != layout)
    {
        is_cache_dirty_ = true;
        layout_         = layout;
        ForceUpdateLayout();
    }
}

void TextActor::SetPreRenderEnabled(bool enable)
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

void TextActor::Update(Duration dt)
{
    this->UpdateDirtyLayout();
    Actor::Update(dt);
}

bool TextActor::CheckVisibility(RenderContext& ctx) const
{
    return layout_ && layout_->IsValid() && Actor::CheckVisibility(ctx);
}

void TextActor::UpdateDirtyLayout()
{
    if (layout_ && layout_->UpdateIfDirty())
    {
        ForceUpdateLayout();
    }
    else if (is_cache_dirty_)
    {
        UpdateCachedTexture();
    }
}

void TextActor::ForceUpdateLayout()
{
    if (layout_)
    {
        layout_->UpdateIfDirty();
        SetSize(layout_->GetSize());
        UpdateCachedTexture();
    }
    else
    {
        SetSize(Size());
    }
}

void TextActor::UpdateCachedTexture()
{
    // 有文字描边或其他额外渲染时，需要开启预渲染以提升性能
    this->SetPreRenderEnabled(outline_brush_ || style_.show_strikethrough || style_.show_underline
                              || (layout_ && layout_->GetContentLength() > 30));

    if (!texture_cached_)
    {
        return;
    }

    const auto expectedSize = layout_->GetSize() + cached_texture_offset * 2;
    if (!render_ctx_)
    {
        render_ctx_ = RenderContext::Create(*texture_cached_, expectedSize);
    }
    else if (render_ctx_->GetSize() != expectedSize)
    {
        render_ctx_->Resize(expectedSize);
    }

    render_ctx_->BeginDraw();
    render_ctx_->Clear();
    render_ctx_->SetCurrentBrush(fill_brush_);
    render_ctx_->SetCurrentStrokeStyle(outline_stroke_);
    render_ctx_->DrawTextLayout(*layout_, cached_texture_offset, outline_brush_);
    render_ctx_->EndDraw();
    is_cache_dirty_ = false;
}

}  // namespace kiwano
