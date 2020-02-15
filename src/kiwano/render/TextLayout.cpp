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

#include <kiwano/render/Renderer.h>
#include <kiwano/render/TextLayout.h>

namespace kiwano
{
TextLayout::TextLayout()
    : dirty_flag_(DirtyFlag::Clean)
{
}

void TextLayout::Update()
{
    if (!IsDirty())
        return;

    Renderer::GetInstance().CreateTextLayout(*this);
}

void TextLayout::SetText(const String& text)
{
    text_ = text;
    dirty_flag_ |= DirtyFlag::DirtyLayout;
}

void TextLayout::SetStyle(const TextStyle& style)
{
    style_ = style;
    dirty_flag_ |= DirtyFlag::DirtyLayout;
}

void TextLayout::SetFont(FontPtr font)
{
    if (style_.font != font)
    {
        style_.font = font;
        dirty_flag_ |= DirtyFlag::DirtyFormat;
    }
}

void TextLayout::SetFontFamily(String const& family)
{
    if (style_.font_family != family)
    {
        style_.font_family = family;
        dirty_flag_ |= DirtyFlag::DirtyFormat;
    }
}

void TextLayout::SetFontSize(float size)
{
    if (style_.font_size != size)
    {
        style_.font_size = size;
        dirty_flag_ |= DirtyFlag::DirtyFormat;
    }
}

void TextLayout::SetFontWeight(uint32_t weight)
{
    if (style_.font_weight != weight)
    {
        style_.font_weight = weight;
        dirty_flag_ |= DirtyFlag::DirtyFormat;
    }
}

void TextLayout::SetItalic(bool italic)
{
    if (style_.italic != italic)
    {
        style_.italic = italic;
        dirty_flag_ |= DirtyFlag::DirtyFormat;
    }
}

uint32_t TextLayout::GetLineCount() const
{
    // Force to update layout
    const_cast<TextLayout*>(this)->Update();

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (text_layout_)
    {
        DWRITE_TEXT_METRICS metrics;
        if (SUCCEEDED(GetTextLayout()->GetMetrics(&metrics)))
        {
            return metrics.lineCount;
        }
    }
#else
    // not supported
#endif
    return 0;
}

Size TextLayout::GetLayoutSize() const
{
    // Force to update layout
    const_cast<TextLayout*>(this)->Update();

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (text_layout_)
    {
        DWRITE_TEXT_METRICS metrics;
        if (SUCCEEDED(GetTextLayout()->GetMetrics(&metrics)))
        {
            return (metrics.layoutWidth > 0) ? Size(metrics.layoutWidth, metrics.height)
                                             : Size(metrics.width, metrics.height);
        }
    }
#else
    // not supported
#endif
    return Size();
}

void TextLayout::SetWrapWidth(float wrap_width)
{
    if (style_.wrap_width != wrap_width)
    {
        style_.wrap_width = wrap_width;
        dirty_flag_ |= DirtyFlag::DirtyLayout;
    }
}

void TextLayout::SetLineSpacing(float line_spacing)
{
    if (style_.line_spacing != line_spacing)
    {
        style_.line_spacing = line_spacing;
        dirty_flag_ |= DirtyFlag::DirtyLayout;
    }
}

void TextLayout::SetAlignment(TextAlign align)
{
    if (style_.alignment != align)
    {
        style_.alignment = align;
        dirty_flag_ |= DirtyFlag::DirtyLayout;
    }
}

void TextLayout::SetUnderline(bool enable, uint32_t start, uint32_t length)
{
    // Force to update layout
    Update();

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    HRESULT hr = text_layout_ ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        hr = text_layout_->SetUnderline(enable, { start, length });
    }
    KGE_THROW_IF_FAILED(hr, "Apply underline style to text layout failed");
#else
    return;  // not supported
#endif
}

void TextLayout::SetStrikethrough(bool enable, uint32_t start, uint32_t length)
{
    // Force to update layout
    Update();

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    HRESULT hr = text_layout_ ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        hr = text_layout_->SetStrikethrough(enable, { start, length });
    }
    KGE_THROW_IF_FAILED(hr, "Apply strikethrough style to text layout failed");
#else
    return;  // not supported
#endif
}

}  // namespace kiwano
