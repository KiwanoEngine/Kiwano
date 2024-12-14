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
#include <kiwano/render/DirectX/helper.h>
#endif

namespace kiwano
{

TextLayout::TextLayout()
    : dirty_flag_(DirtyFlag::Clean)
    , line_count_(0)
    , content_length_(0)
{
}

TextLayout::TextLayout(StringView content, const TextStyle& style)
    : TextLayout()
{
    Reset(content, style);
}

void TextLayout::Clear()
{
    ResetNative();
}

void TextLayout::Reset(StringView content, const TextStyle& style)
{
    content_length_ = (uint32_t)content.size();
    if (content_length_)
    {
        Renderer::GetInstance().CreateTextLayout(*this, content, style);

        SetAlignment(style.alignment);
        SetWrapWidth(style.wrap_width);
        SetLineSpacing(style.line_spacing);

        if (style.show_underline)
            SetUnderline(style.show_underline);

        if (style.show_strikethrough)
            SetStrikethrough(style.show_strikethrough);
    }
    else
    {
        Clear();
    }

    SetDirtyFlag(DirtyFlag::Dirty);
}

Size TextLayout::GetSize() const
{
    const_cast<TextLayout*>(this)->UpdateIfDirty();
    return size_;
}

uint32_t TextLayout::GetLineCount() const
{
    const_cast<TextLayout*>(this)->UpdateIfDirty();
    return line_count_;
}

void TextLayout::SetFont(const Font& font)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = ComPolicy::Get<IDWriteTextLayout>((const NativeObject*)(this));
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = S_OK;

        // reset font collection
        {
            auto collection = ComPolicy::Get<IDWriteFontCollection>(font.collection);

            hr = native->SetFontCollection(collection.Get(), { 0, content_length_ });
            KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontCollection failed");
        }

        // reset font family
        {
            WideString font_family = font.family_name.empty() ? L"" : strings::NarrowToWide(font.family_name);

            hr = native->SetFontFamilyName(font_family.c_str(), { 0, content_length_ });
            KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontFamilyName failed");
        }

        // reset font size
        {
            hr = native->SetFontSize(font.size, { 0, content_length_ });
            KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontSize failed");
        }

        // reset font weight
        {
            auto font_weight = DWRITE_FONT_WEIGHT(font.weight);

            hr = native->SetFontWeight(font_weight, { 0, content_length_ });
            KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontWeight failed");
        }

        // reset font style
        {
            auto font_style = DWRITE_FONT_STYLE(font.posture);

            hr = native->SetFontStyle(font_style, { 0, content_length_ });
            KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontStyle failed");
        }

        // reset font stretch
        {
            auto font_stretch = DWRITE_FONT_STRETCH(font.stretch);

            hr = native->SetFontStretch(font_stretch, { 0, content_length_ });
            KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetFontStretch failed");
        }
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetUnderline(bool enable)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = ComPolicy::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = native->SetUnderline(enable, { 0, content_length_ });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetUnderline failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetStrikethrough(bool enable)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = ComPolicy::Get<IDWriteTextLayout>(this);
    KGE_ASSERT(native);

    if (native)
    {
        HRESULT hr = native->SetStrikethrough(enable, { 0, content_length_ });
        KGE_THROW_IF_FAILED(hr, "IDWriteTextLayout::SetStrikethrough failed");
    }
#else
    // not supported
#endif

    SetDirtyFlag(DirtyFlag::Dirty);
}

void TextLayout::SetAlignment(TextAlign align)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = ComPolicy::Get<IDWriteTextLayout>(this);
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
    auto native = ComPolicy::Get<IDWriteTextLayout>(this);
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
    auto native = ComPolicy::Get<IDWriteTextLayout>(this);
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

bool TextLayout::UpdateIfDirty()
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (dirty_flag_ == DirtyFlag::Dirty)
    {
        SetDirtyFlag(DirtyFlag::Clean);

        line_count_ = 0;
        size_       = Size();

        auto native = ComPolicy::Get<IDWriteTextLayout>(this);
        if (content_length_ == 0 || !native)
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
