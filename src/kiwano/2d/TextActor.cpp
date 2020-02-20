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
#include <kiwano/core/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

TextActorPtr TextActor::Create(const String& text)
{
    return TextActor::Create(text, TextStyle());
}

TextActorPtr TextActor::Create(const String& text, const TextStyle& style)
{
    TextActorPtr ptr = memory::New<TextActor>();
    if (ptr)
    {
        ptr->SetStyle(style);
        ptr->SetText(text);
    }
    return ptr;
}

TextActor::TextActor()
{
}

TextActor::~TextActor() {}

void TextActor::OnRender(RenderContext& ctx)
{
    if (layout_)
    {
        ctx.DrawTextLayout(*layout_);
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
        layout_ = TextLayout::Create();
    }
    layout_->Reset(text, style_);
}

void TextActor::SetStyle(const TextStyle& style)
{
    style_ = style;
    if (layout_)
        layout_->Reset(style);
}

void TextActor::SetFont(FontPtr font)
{
    if (style_.font != font)
    {
        style_.font = font;
        if (layout_)
            layout_->SetFont(font, { 0, layout_->GetContentLength() });
    }
}

void TextActor::SetFontFamily(const String& family)
{
    if (style_.font_family != family)
    {
        style_.font_family = family;
        if (layout_)
            layout_->SetFontFamily(family, { 0, layout_->GetContentLength() });
    }
}

void TextActor::SetFontSize(float size)
{
    if (style_.font_size != size)
    {
        style_.font_size = size;
        if (layout_)
            layout_->SetFontSize(size, { 0, layout_->GetContentLength() });
    }
}

void TextActor::SetFontWeight(uint32_t weight)
{
    if (style_.font_weight != weight)
    {
        style_.font_weight = weight;
        if (layout_)
            layout_->SetFontWeight(weight, { 0, layout_->GetContentLength() });
    }
}

void TextActor::SetItalic(bool italic)
{
    if (style_.italic != italic)
    {
        style_.italic = italic;
        if (layout_)
            layout_->SetItalic(italic, { 0, layout_->GetContentLength() });
    }
}

void TextActor::SetUnderline(bool enable)
{
    if (style_.show_underline != enable)
    {
        style_.show_underline = enable;
        if (layout_)
            layout_->SetUnderline(enable, { 0, layout_->GetContentLength() });
    }
}

void TextActor::SetStrikethrough(bool enable)
{
    if (style_.show_strikethrough != enable)
    {
        style_.show_strikethrough = enable;
        if (layout_)
            layout_->SetStrikethrough(enable, { 0, layout_->GetContentLength() });
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
    if (style_.fill_brush != brush)
    {
        style_.fill_brush = brush;
        if (layout_)
            layout_->SetDefaultFillBrush(brush);
    }
}

void TextActor::SetOutlineBrush(BrushPtr brush)
{
    if (style_.outline_brush != brush)
    {
        style_.outline_brush = brush;
        if (layout_)
            layout_->SetDefaultOutlineBrush(brush);
    }
}

void TextActor::SetOutlineStrokeStyle(StrokeStylePtr stroke)
{
    if (style_.outline_stroke != stroke)
    {
        style_.outline_stroke = stroke;
        if (layout_)
            layout_->SetDefaultOutlineStrokeStyle(stroke);
    }
}

void TextActor::SetFillColor(const Color& color)
{
    if (style_.fill_brush)
    {
        style_.fill_brush->SetColor(color);
    }
    else
    {
        SetFillBrush(Brush::Create(color));
    }
}

void TextActor::SetOutlineColor(const Color& outline_color)
{
    if (style_.outline_brush)
    {
        style_.outline_brush->SetColor(outline_color);
    }
    else
    {
        SetFillBrush(Brush::Create(outline_color));
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
    if (layout_ && layout_->UpdateWhenDirty())
    {
        ForceUpdateLayout();
    }
}

void TextActor::ForceUpdateLayout()
{
    if (layout_)
    {
        layout_->UpdateWhenDirty();
        SetSize(layout_->GetSize());
    }
    else
    {
        SetSize(Size());
    }
}

}  // namespace kiwano
