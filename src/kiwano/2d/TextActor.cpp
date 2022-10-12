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

TextActor::TextActor() {}

TextActor::TextActor(const String& text)
    : TextActor(text, TextStyle())
{
}

TextActor::TextActor(const String& text, const TextStyle& style)
{
    SetStyle(style);
    SetText(text);
}

TextActor::~TextActor() {}

void TextActor::OnRender(RenderContext& ctx)
{
    if (layout_)
    {
        ctx.SetCurrentBrush(fill_brush_);
        ctx.SetCurrentStrokeStyle(outline_stroke_);
        ctx.DrawTextLayout(*layout_, Point{}, outline_brush_);
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
    }
    catch (SystemError& e)
    {
        Fail(String("TextActor::SetText failed: ") + e.what());
    }
}

void TextActor::SetStyle(const TextStyle& style)
{
    style_ = style;
    if (layout_)
        layout_->Reset(content_, style);
}

void TextActor::SetFont(FontPtr font)
{
    if (style_.font != font)
    {
        style_.font = font;
        if (layout_)
            layout_->SetFont(font);
    }
}

void TextActor::SetUnderline(bool enable)
{
    if (style_.show_underline != enable)
    {
        style_.show_underline = enable;
        if (layout_)
            layout_->SetUnderline(enable);
    }
}

void TextActor::SetStrikethrough(bool enable)
{
    if (style_.show_strikethrough != enable)
    {
        style_.show_strikethrough = enable;
        if (layout_)
            layout_->SetStrikethrough(enable);
    }
}

void TextActor::SetWrapWidth(float wrap_width)
{
    if (style_.wrap_width != wrap_width)
    {
        style_.wrap_width = wrap_width;
        if (layout_)
            layout_->SetWrapWidth(wrap_width);
    }
}

void TextActor::SetLineSpacing(float line_spacing)
{
    if (style_.line_spacing != line_spacing)
    {
        style_.line_spacing = line_spacing;
        if (layout_)
            layout_->SetLineSpacing(line_spacing);
    }
}

void TextActor::SetAlignment(TextAlign align)
{
    if (style_.alignment != align)
    {
        style_.alignment = align;
        if (layout_)
            layout_->SetAlignment(align);
    }
}

void TextActor::SetFillBrush(BrushPtr brush)
{
    fill_brush_ = brush;
}

void TextActor::SetOutlineBrush(BrushPtr brush)
{
    outline_brush_ = brush;
}

void TextActor::SetOutlineStrokeStyle(StrokeStylePtr stroke)
{
    outline_stroke_ = stroke;
}

void TextActor::SetFillColor(const Color& color)
{
    if (fill_brush_)
    {
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
        layout_ = layout;
        ForceUpdateLayout();
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
}

void TextActor::ForceUpdateLayout()
{
    if (layout_)
    {
        layout_->UpdateIfDirty();
        SetSize(layout_->GetSize());
    }
    else
    {
        SetSize(Size());
    }
}

}  // namespace kiwano
