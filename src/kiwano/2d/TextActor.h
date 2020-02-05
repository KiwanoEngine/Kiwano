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

#pragma once
#include <kiwano/2d/Actor.h>
#include <kiwano/render/Color.h>
#include <kiwano/render/TextLayout.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(TextActor);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 文本角色
 */
class KGE_API TextActor : public Actor
{
public:
    /// \~chinese
    /// @brief 构建空的文本角色
    TextActor();

    /// \~chinese
    /// @brief 构建文本角色
    /// @param text 文字内容
    explicit TextActor(const String& text);

    /// \~chinese
    /// @brief 构建文本角色
    /// @param text 文字内容
    /// @param style 文本样式
    TextActor(const String& text, const TextStyle& style);

    virtual ~TextActor();

    /// \~chinese
    /// @brief 获取文本
    const String& GetText() const;

    /// \~chinese
    /// @brief 获取文本样式
    const TextStyle& GetStyle() const;

    /// \~chinese
    /// @brief 获取文本布局
    const TextLayout& GetLayout() const;

    /// \~chinese
    /// @brief 获取文本布局大小
    Size GetLayoutSize() const;

    /// \~chinese
    /// @brief 获取填充画刷
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief 获取描边画刷
    BrushPtr GetOutlineBrush() const;

    /// \~chinese
    /// @brief 获取字体
    FontPtr GetFont() const;

    /// \~chinese
    /// @brief 设置文本
    void SetText(String const& text);

    /// \~chinese
    /// @brief 设置文本样式
    void SetStyle(const TextStyle& style);

    /// \~chinese
    /// @brief 设置字体
    void SetFont(FontPtr font);

    /// \~chinese
    /// @brief 设置字体族
    void SetFontFamily(String const& family);

    /// \~chinese
    /// @brief 设置字号（默认值为 18）
    void SetFontSize(float size);

    /// \~chinese
    /// @brief 设置字体粗细值（默认值为 FontWeight::Normal）
    void SetFontWeight(uint32_t weight);

    /// \~chinese
    /// @brief 设置文字填充画刷
    void SetFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置文字填充颜色（默认值为 Color::White）
    void SetFillColor(Color const& color);

    /// \~chinese
    /// @brief 设置文字斜体（默认值为 false）
    void SetItalic(bool italic);

    /// \~chinese
    /// @brief 设置文本自动换行的宽度（默认为 0）
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief 设置行间距（默认为 0）
    void SetLineSpacing(float line_spacing);

    /// \~chinese
    /// @brief 设置对齐方式（默认为 TextAlign::Left）
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief 设置文字描边画刷
    void SetOutlineBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置文字描边颜色
    void SetOutlineColor(Color const& outline_color);

    /// \~chinese
    /// @brief 设置文字描边线宽
    void SetOutlineWidth(float outline_width);

    /// \~chinese
    /// @brief 设置文字描边线相交样式
    void SetOutlineStroke(const StrokeStyle& outline_stroke);

    /// \~chinese
    /// @brief 设置是否显示下划线（默认值为 false）
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief 设置是否显示删除线（默认值为 false）
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief 更新文字布局
    /// @details 文字布局是懒更新的，手动更新文字布局以更新节点状态
    void UpdateLayout();

    void OnRender(RenderContext& ctx) override;

    void OnUpdate(Duration dt) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool       show_underline_;
    bool       show_strikethrough_;
    TextLayout text_layout_;
};

/** @} */

inline const String& TextActor::GetText() const
{
    return text_layout_.GetText();
}

inline FontPtr TextActor::GetFont() const
{
    return text_layout_.GetStyle().font;
}

inline const TextStyle& TextActor::GetStyle() const
{
    return text_layout_.GetStyle();
}

inline const TextLayout& TextActor::GetLayout() const
{
    return text_layout_;
}

inline Size TextActor::GetLayoutSize() const
{
    return text_layout_.GetLayoutSize();
}

inline BrushPtr TextActor::GetFillBrush() const
{
    return text_layout_.GetFillBrush();
}

inline BrushPtr TextActor::GetOutlineBrush() const
{
    return text_layout_.GetOutlineBrush();
}

inline void TextActor::SetText(String const& text)
{
    text_layout_.SetText(text);
}

inline void TextActor::SetStyle(const TextStyle& style)
{
    text_layout_.SetStyle(style);
}

inline void TextActor::SetFont(FontPtr font)
{
    text_layout_.SetFont(font);
}

inline void TextActor::SetFontFamily(String const& family)
{
    text_layout_.SetFontFamily(family);
}

inline void TextActor::SetFontSize(float size)
{
    text_layout_.SetFontSize(size);
}

inline void TextActor::SetFontWeight(uint32_t weight)
{
    text_layout_.SetFontWeight(weight);
}

inline void TextActor::SetItalic(bool italic)
{
    text_layout_.SetItalic(italic);
}

inline void TextActor::SetWrapWidth(float wrap_width)
{
    text_layout_.SetWrapWidth(wrap_width);
}

inline void TextActor::SetLineSpacing(float line_spacing)
{
    text_layout_.SetLineSpacing(line_spacing);
}

inline void TextActor::SetAlignment(TextAlign align)
{
    text_layout_.SetAlignment(align);
}

inline void TextActor::SetUnderline(bool enable)
{
    show_underline_ = enable;
}

inline void TextActor::SetStrikethrough(bool enable)
{
    show_strikethrough_ = enable;
}

inline void TextActor::SetFillBrush(BrushPtr brush)
{
    text_layout_.SetFillBrush(brush);
}

inline void TextActor::SetOutlineBrush(BrushPtr brush)
{
    text_layout_.SetOutlineBrush(brush);
}

inline void TextActor::SetOutlineWidth(float outline_width)
{
    text_layout_.SetOutlineWidth(outline_width);
}

inline void TextActor::SetOutlineStroke(const StrokeStyle& outline_stroke)
{
    text_layout_.SetOutlineStroke(outline_stroke);
}
}  // namespace kiwano
