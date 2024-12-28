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

TextActor::TextActor()
{
    render_comp_ = new TextRenderComponent;
    AddComponent(render_comp_);
}

TextActor::TextActor(StringView text, const TextStyle& style)
    : TextActor()
{
    SetStyle(style);
    SetText(text);
}

TextActor::~TextActor() {}

Size TextActor::GetSize() const
{
    const_cast<TextActor*>(this)->UpdateDirtyLayout();
    return Actor::GetSize();
}

void TextActor::SetText(StringView text)
{
    ResetLayout(text, style_);
}

void TextActor::SetStyle(const TextStyle& style)
{
    style_ = style;
    ResetLayout(content_, style_);
}

void TextActor::SetFont(const Font& font)
{
    style_.font = font;
    ResetLayout(content_, style_);
}

void TextActor::SetUnderline(bool enable)
{
    if (style_.show_underline != enable)
    {
        style_.show_underline = enable;
        ResetLayout(content_, style_);
    }
}

void TextActor::SetStrikethrough(bool enable)
{
    if (style_.show_strikethrough != enable)
    {
        style_.show_strikethrough = enable;
        ResetLayout(content_, style_);
    }
}

void TextActor::SetWrapWidth(float wrap_width)
{
    if (style_.wrap_width != wrap_width)
    {
        style_.wrap_width = wrap_width;
        ResetLayout(content_, style_);
    }
}

void TextActor::SetLineSpacing(float line_spacing)
{
    if (style_.line_spacing != line_spacing)
    {
        style_.line_spacing = line_spacing;
        ResetLayout(content_, style_);
    }
}

void TextActor::SetAlignment(TextAlign align)
{
    if (style_.alignment != align)
    {
        style_.alignment = align;
        ResetLayout(content_, style_);
    }
}

void TextActor::SetFillBrush(RefPtr<Brush> brush)
{
    render_comp_->SetFillBrush(brush);
}

void TextActor::SetOutlineBrush(RefPtr<Brush> brush)
{
    render_comp_->SetOutlineBrush(brush);
}

void TextActor::SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke)
{
    render_comp_->SetOutlineStrokeStyle(stroke);
}

void TextActor::SetFillColor(const Color& color)
{
    SetFillBrush(MakePtr<Brush>(color));
}

void TextActor::SetOutlineColor(const Color& outline_color)
{
    SetOutlineBrush(MakePtr<Brush>(outline_color));
}

void TextActor::SetTextLayout(RefPtr<TextLayout> layout)
{
    render_comp_->SetTextLayout(layout);
    ForceUpdateLayout();
}

void TextActor::Update(Duration dt)
{
    this->UpdateDirtyLayout();
    Actor::Update(dt);
}

void TextActor::UpdateDirtyLayout()
{
    auto layout = render_comp_->GetTextLayout();
    if (layout && layout->UpdateIfDirty())
    {
        ForceUpdateLayout();
    }
    else
    {
        UpdateCachedTexture();
    }
}

void TextActor::ForceUpdateLayout()
{
    auto layout = render_comp_->GetTextLayout();
    if (layout)
    {
        layout->UpdateIfDirty();
        SetSize(layout->GetSize());
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
    auto layout = render_comp_->GetTextLayout();
    render_comp_->SetPreRenderEnabled(render_comp_->GetOutlineBrush() || style_.show_strikethrough
                                      || style_.show_underline || (layout && layout->GetContentLength() > 30));
    render_comp_->UpdateCachedTexture();
}

void TextActor::ResetLayout(StringView content, const TextStyle& style)
{
    if (!render_comp_->GetTextLayout())
    {
        render_comp_->SetTextLayout(MakePtr<TextLayout>());
    }

    try
    {
        if (content_.c_str() != content.data())
            content_ = content;
        if (&style_ != &style)
            style_ = style;

        auto layout = render_comp_->GetTextLayout();
        layout->Reset(content, style);
        render_comp_->SetTextLayout(layout);
    }
    catch (SystemError& e)
    {
        Fail(String("TextActor::ResetLayout failed: ") + e.what());
    }
}

}  // namespace kiwano
