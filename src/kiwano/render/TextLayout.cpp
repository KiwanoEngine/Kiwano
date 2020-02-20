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

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/NativePtr.h>
#endif

namespace kiwano
{

TextLayoutPtr TextLayout::Create()
{
    TextLayoutPtr ptr = memory::New<TextLayout>();
    return ptr;
}

TextLayoutPtr TextLayout::Create(const String& content, const TextStyle& style)
{
    TextLayoutPtr ptr = memory::New<TextLayout>();
    if (ptr)
    {
        ptr->Reset(content, style);
    }
    return ptr;
}

TextLayout::TextLayout()
    : dirty_flag_(DirtyFlag::Clean)
    , line_count_(0)
{
}

void TextLayout::Reset(const String& content, const TextStyle& style)
{
    content_ = content;
    if (!content_.empty())
    {
        Renderer::GetInstance().CreateTextLayout(*this, content_, style);

        SetAlignment(style.alignment);
        SetWrapWidth(style.wrap_width);
        SetLineSpacing(style.line_spacing);

        SetDefaultFillBrush(style.fill_brush);
        SetDefaultOutlineBrush(style.outline_brush);
        SetDefaultOutlineStrokeStyle(style.outline_stroke);

        if (style.show_underline)
            SetUnderline(style.show_underline, { 0, content_.length() });

        if (style.show_strikethrough)
            SetStrikethrough(style.show_strikethrough, { 0, content_.length() });
    }
    else
    {
        Clear();
    }

    SetDirtyFlag(DirtyFlag::Dirty);
}

Size TextLayout::GetSize() const
{
    const_cast<TextLayout*>(this)->UpdateWhenDirty();
    return size_;
}

uint32_t TextLayout::GetLineCount() const
{
    const_cast<TextLayout*>(this)->UpdateWhenDirty();
    return line_count_;
}

void TextLayout::SetFont(FontPtr font, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        auto collection = NativePtr::Get<IDWriteFontCollection>(font);

        HRESULT hr = native->SetFontCollection(collection.Get(), { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontCollection failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetFontFamily(const String& family, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        WideString font_family = family.empty() ? L"" : string::ToWide(family);

        HRESULT hr = native->SetFontFamilyName(font_family.c_str(), { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontFamilyName failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetFontSize(float size, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = native->SetFontSize(size, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontSize failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetFontWeight(uint32_t weight, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        DWRITE_FONT_WEIGHT font_weight = DWRITE_FONT_WEIGHT(weight);

        HRESULT hr = native->SetFontWeight(font_weight, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontWeight failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetItalic(bool italic, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        DWRITE_FONT_STYLE font_style = italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL;

        HRESULT hr = native->SetFontStyle(font_style, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontStyle failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetUnderline(bool enable, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = native->SetUnderline(enable, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetUnderline failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetStrikethrough(bool enable, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = native->SetStrikethrough(enable, { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetStrikethrough failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetFillBrush(BrushPtr brush, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr =
            native->SetDrawingEffect(NativePtr::Get<ID2D1Brush>(brush).Get(), { range.start, range.length });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetDrawingEffect failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetOutlineBrush(BrushPtr brush, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    // TODO
    KGE_NOT_USED(range);
    SetDefaultOutlineBrush(brush);
#else
    return;  // not supported
#endif
}

void TextLayout::SetOutlineStrokeStyle(StrokeStylePtr stroke, TextRange range)
{
    KGE_ASSERT(content_.size() >= (range.start + range.length));
    if (range.length == 0)
        return;

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
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
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

        HRESULT hr = native->SetTextAlignment(alignment);
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetTextAlignment failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetWrapWidth(float wrap_width)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = S_OK;
        if (wrap_width > 0)
        {
            hr = native->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
            if (SUCCEEDED(hr))
            {
                hr = native->SetMaxWidth(wrap_width);
            }
        }
        else
        {
            hr = native->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        }
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetWordWrapping failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetLineSpacing(float line_spacing)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = NativePtr::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = S_OK;

        float spacing = line_spacing;
        if (spacing == 0.f)
        {
            hr = native->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
        }
        else
        {
            hr = native->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, spacing, spacing * 0.8f);
        }
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetLineSpacing failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

bool TextLayout::UpdateWhenDirty()
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (dirty_flag_ == DirtyFlag::Dirty)
    {
        SetDirtyFlag(DirtyFlag::Clean);

        line_count_ = 0;
        size_       = Size();

        auto native = NativePtr::Get<IDWriteTextLayout>(this);
        if (content_.empty() || !native)
            return true;

        HRESULT hr = S_OK;

        DWRITE_TEXT_METRICS metrics;
        hr = native->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            if (native->GetWordWrapping() == DWRITE_WORD_WRAPPING_NO_WRAP)
            {
                // Fix the layout width when the text does not wrap
                hr = native->SetMaxWidth(metrics.widthIncludingTrailingWhitespace);
                if (SUCCEEDED(hr))
                {
                    hr = native->GetMetrics(&metrics);
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            line_count_ = metrics.lineCount;

            if (metrics.layoutWidth > 0)
            {
                size_ = Size(metrics.layoutWidth, metrics.height);
            }
            else
            {
                size_ = Size(metrics.widthIncludingTrailingWhitespace, metrics.height);
            }
        }

        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::GetMetrics failed");
        return true;
    }
#else
    // not supported
#endif
    return false;
}

}  // namespace kiwano
