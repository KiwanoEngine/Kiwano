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
TextLayoutPtr TextLayout::Create()
{
    TextLayoutPtr ptr = new (std::nothrow) TextLayout;
    return ptr;
}

TextLayoutPtr TextLayout::Create(const String& content, const TextStyle& style)
{
    TextLayoutPtr ptr = new (std::nothrow) TextLayout;
    if (ptr)
    {
        ptr->Reset(content, style);
    }
    return ptr;
}

TextLayout::TextLayout()
    : dirty_flag_(DirtyFlag::Clean)
    , default_outline_width_(0.0f)
{
}

void TextLayout::Reset(const String& text, const TextStyle& style)
{
    if (!text.empty())
    {
        Renderer::GetInstance().CreateTextLayout(*this, text, style);

        SetAlignment(style.alignment);
        SetWrapWidth(style.wrap_width);
        SetLineSpacing(style.line_spacing);
        SetUnderline(style.show_underline, { 0, text.length() });
        SetStrikethrough(style.show_strikethrough, { 0, text.length() });
        SetDefaultFillBrush(style.fill_brush);
        SetDefaultOutlineBrush(style.outline_brush);
        SetDefaultOutlineWidth(style.outline_width);
        SetDefaultOutlineStrokeStyle(style.outline_stroke);
    }
    else
    {
        Clear();
    }
}

uint32_t TextLayout::GetLineCount() const
{
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

void TextLayout::SetFont(FontPtr font, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        IDWriteFontCollection* collection = font ? font->GetCollection().Get() : nullptr;

        HRESULT hr = text_layout_->SetFontCollection(collection, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontCollection failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    // not supported
#endif
}

void TextLayout::SetFontFamily(String const& family, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        WideString font_family = family.empty() ? L"" : string::ToWide(family);

        HRESULT hr = text_layout_->SetFontFamilyName(font_family.c_str(), { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontFamilyName failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    // not supported
#endif
}

void TextLayout::SetFontSize(float size, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        HRESULT hr = text_layout_->SetFontSize(size, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontSize failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    // not supported
#endif
}

void TextLayout::SetFontWeight(uint32_t weight, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        DWRITE_FONT_WEIGHT font_weight = DWRITE_FONT_WEIGHT(weight);

        HRESULT hr = text_layout_->SetFontWeight(font_weight, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontWeight failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    // not supported
#endif
}

void TextLayout::SetItalic(bool italic, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        DWRITE_FONT_STYLE font_style = italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL;

        HRESULT hr = text_layout_->SetFontStyle(font_style, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontStyle failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    // not supported
#endif
}

void TextLayout::SetUnderline(bool enable, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        HRESULT hr = text_layout_->SetUnderline(enable, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetUnderline failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    return;  // not supported
#endif
}

void TextLayout::SetStrikethrough(bool enable, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        HRESULT hr = text_layout_->SetStrikethrough(enable, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetStrikethrough failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    return;  // not supported
#endif
}

void TextLayout::SetFillBrush(BrushPtr brush, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        HRESULT hr = text_layout_->SetDrawingEffect(brush->GetBrush().Get(), { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetDrawingEffect failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    return;  // not supported
#endif
}

void TextLayout::SetOutlineBrush(BrushPtr brush, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    // TODO
    KGE_NOT_USED(range);
    SetDefaultOutlineBrush(brush);
#else
    return;  // not supported
#endif
}

void TextLayout::SetOutlineWidth(float width, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    // TODO
    KGE_NOT_USED(range);
    SetDefaultOutlineWidth(width);
#else
    return;  // not supported
#endif
}

void TextLayout::SetOutlineStrokeStyle(StrokeStylePtr stroke, TextRange range)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    // TODO
    KGE_NOT_USED(range);
    SetDefaultOutlineStrokeStyle(stroke);
#else
    return;  // not supported
#endif
}

void TextLayout::SetAlignment(TextAlign align)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT();
        switch (align)
        {
        case TextAlign::Left:
            alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
            break;
        case TextAlign::Right:
            alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
            break;
        case TextAlign::Center:
            alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
            break;
        case TextAlign::Justified:
            alignment = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
            break;
        }

        HRESULT hr = text_layout_->SetTextAlignment(alignment);
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetTextAlignment failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    // not supported
#endif
}

void TextLayout::SetWrapWidth(float wrap_width)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        DWRITE_WORD_WRAPPING wrapping = (wrap_width > 0) ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP;

        HRESULT hr = text_layout_->SetWordWrapping(wrapping);
        if (SUCCEEDED(hr))
        {
            if (wrap_width > 0)
            {
                hr = text_layout_->SetMaxWidth(wrap_width);
            }
            else
            {
                // Fix the layout width when the text does not wrap
                DWRITE_TEXT_METRICS metrics;
                hr = text_layout_->GetMetrics(&metrics);

                if (SUCCEEDED(hr))
                {
                    hr = text_layout_->SetMaxWidth(metrics.width);
                }
            }
        }
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetWordWrapping failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    return;  // not supported
#endif
}

void TextLayout::SetLineSpacing(float line_spacing)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(text_layout_);
    if (text_layout_)
    {
        HRESULT hr = S_OK;

        float spacing = line_spacing;
        if (spacing == 0.f)
        {
            hr = text_layout_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
        }
        else
        {
            hr = text_layout_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, spacing, spacing * 0.8f);
        }
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetLineSpacing failed");

        dirty_flag_ = DirtyFlag::Updated;
    }
#else
    return;  // not supported
#endif
}

}  // namespace kiwano
