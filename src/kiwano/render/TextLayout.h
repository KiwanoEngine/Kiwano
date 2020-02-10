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
#include <kiwano/math/Math.h>
#include <kiwano/render/TextStyle.hpp>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief 文本布局
class KGE_API TextLayout
{
public:
    /// \~chinese
    /// @brief 构造空的文本布局
    TextLayout();

    /// \~chinese
    /// @brief 文本布局是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 文本布局是否陈旧
    bool IsDirty() const;

    /// \~chinese
    /// @brief 更新文本布局
    /// @note 文本布局是懒更新的，在修改文本布局的属性后需要手动更新
    void Update();

    /// \~chinese
    /// @brief 获取文本
    const String& GetText() const;

    /// \~chinese
    /// @brief 获取文本样式
    const TextStyle& GetStyle() const;

    /// \~chinese
    /// @brief 获取文本行数
    uint32_t GetLineCount() const;

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
    /// @brief 设置文本
    void SetText(const String& text);

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
    /// @brief 设置文字斜体（默认值为 false）
    void SetItalic(bool italic);

    /// \~chinese
    /// @brief 设置文本自动换行的宽度
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief 设置行间距（默认为 0）
    void SetLineSpacing(float line_spacing);

    /// \~chinese
    /// @brief 设置对齐方式
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief 设置文字描边画刷
    void SetOutlineBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置文字描边线宽
    void SetOutlineWidth(float outline_width);

    /// \~chinese
    /// @brief 设置文字描边线相交样式
    void SetOutlineStroke(StrokeStylePtr outline_stroke);

    /// \~chinese
    /// @brief 设置下划线
    /// @param enable 是否显示下划线
    /// @param start 起始位置
    /// @param length 长度
    void SetUnderline(bool enable, uint32_t start, uint32_t length);

    /// \~chinese
    /// @brief 设置删除线
    /// @param enable 是否显示删除线
    /// @param start 起始位置
    /// @param length 长度
    void SetStrikethrough(bool enable, uint32_t start, uint32_t length);

    /// \~chinese
    /// @brief 脏数据标志
    enum DirtyFlag : uint8_t
    {
        Clean       = 0,       ///< 干净数据
        DirtyFormat = 1,       ///< 文字格式待更新
        DirtyLayout = 1 << 1,  ///< 文字布局待更新
        Updated     = 1 << 2,  ///< 数据已更新
    };

    uint8_t GetDirtyFlag() const;

    void SetDirtyFlag(uint8_t flag);

private:
    uint8_t   dirty_flag_;
    String    text_;
    TextStyle style_;

#if defined(KGE_WIN32)
public:
    ComPtr<IDWriteTextFormat> GetTextFormat() const;

    void SetTextFormat(ComPtr<IDWriteTextFormat> format);

    ComPtr<IDWriteTextLayout> GetTextLayout() const;

    void SetTextLayout(ComPtr<IDWriteTextLayout> layout);

private:
    ComPtr<IDWriteTextFormat> text_format_;
    ComPtr<IDWriteTextLayout> text_layout_;
#endif
};

/** @} */

inline bool TextLayout::IsValid() const
{
    return text_layout_ != nullptr;
}

inline bool TextLayout::IsDirty() const
{
    return dirty_flag_ != DirtyFlag::Clean;
}

inline const String& TextLayout::GetText() const
{
    return text_;
}

inline const TextStyle& TextLayout::GetStyle() const
{
    return style_;
}

inline uint8_t TextLayout::GetDirtyFlag() const
{
    return dirty_flag_;
}

inline void TextLayout::SetDirtyFlag(uint8_t flag)
{
    dirty_flag_ = flag;
}

inline BrushPtr TextLayout::GetFillBrush() const
{
    return style_.fill_brush;
}

inline BrushPtr TextLayout::GetOutlineBrush() const
{
    return style_.outline_brush;
}

inline void TextLayout::SetFillBrush(BrushPtr brush)
{
    style_.fill_brush = brush;
}

inline void TextLayout::SetOutlineBrush(BrushPtr brush)
{
    style_.outline_brush = brush;
}

inline void TextLayout::SetOutlineWidth(float outline_width)
{
    style_.outline_width = outline_width;
}

inline void TextLayout::SetOutlineStroke(StrokeStylePtr outline_stroke)
{
    style_.outline_stroke = outline_stroke;
}

#if defined(KGE_WIN32)
inline ComPtr<IDWriteTextFormat> TextLayout::GetTextFormat() const
{
    return text_format_;
}

inline ComPtr<IDWriteTextLayout> TextLayout::GetTextLayout() const
{
    return text_layout_;
}

inline void TextLayout::SetTextFormat(ComPtr<IDWriteTextFormat> format)
{
    text_format_ = format;
}

inline void TextLayout::SetTextLayout(ComPtr<IDWriteTextLayout> layout)
{
    text_layout_ = layout;
}
#endif

}  // namespace kiwano
