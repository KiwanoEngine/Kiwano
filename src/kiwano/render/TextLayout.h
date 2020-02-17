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
#include <kiwano/render/NativeObject.h>
#include <kiwano/render/TextStyle.hpp>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(TextLayout);

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief 文本布局
class KGE_API TextLayout : public NativeObject
{
public:
    /// \~chinese
    /// @brief 创建文本布局
    static TextLayoutPtr Create();

    /// \~chinese
    /// @brief 创建文本布局
    /// @param content 文字内容
    /// @param style 文本样式
    static TextLayoutPtr Create(const String& content, const TextStyle& style);

    /// \~chinese
    /// @brief 构造空的文本布局
    TextLayout();

    /// \~chinese
    /// @brief 文本布局是否陈旧
    bool IsDirty() const;

    /// \~chinese
    /// @brief 清空文本布局
    void Clear();

    /// \~chinese
    /// @brief 重设文本布局
    /// @param content 文字内容
    /// @param style 文本样式
    void Reset(const String& content, const TextStyle& style);

    /// \~chinese
    /// @brief 获取文本布局大小
    Size GetSize() const;

    /// \~chinese
    /// @brief 获取文本行数
    uint32_t GetLineCount() const;

    /// \~chinese
    /// @brief 获取默认填充画刷
    BrushPtr GetDefaultFillBrush() const;

    /// \~chinese
    /// @brief 获取默认描边画刷
    BrushPtr GetDefaultOutlineBrush() const;

    /// \~chinese
    /// @brief 获取默认描边线条样式
    StrokeStylePtr GetDefaultOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief 文字范围
    struct TextRange
    {
        uint32_t start;   ///< 起始位置
        uint32_t length;  ///< 长度
    };

    /// \~chinese
    /// @brief 设置字体
    /// @param font 字体
    /// @param range 文字范围
    void SetFont(FontPtr font, TextRange range);

    /// \~chinese
    /// @brief 设置字体族
    /// @param family 字体族
    /// @param range 文字范围
    void SetFontFamily(String const& family, TextRange range);

    /// \~chinese
    /// @brief 设置字号（默认值为 18）
    /// @param size 字号
    /// @param range 文字范围
    void SetFontSize(float size, TextRange range);

    /// \~chinese
    /// @brief 设置字体粗细值（默认值为 FontWeight::Normal）
    /// @param weight 粗细值
    /// @param range 文字范围
    void SetFontWeight(uint32_t weight, TextRange range);

    /// \~chinese
    /// @brief 设置文字斜体（默认值为 false）
    /// @param italic 是否是斜体
    /// @param range 文字范围
    void SetItalic(bool italic, TextRange range);

    /// \~chinese
    /// @brief 设置下划线
    /// @param enable 是否显示下划线
    /// @param range 文字范围
    void SetUnderline(bool enable, TextRange range);

    /// \~chinese
    /// @brief 设置删除线
    /// @param enable 是否显示删除线
    /// @param range 文字范围
    void SetStrikethrough(bool enable, TextRange range);

    /// \~chinese
    /// @brief 设置文字填充画刷，描边画刷和描边线宽
    /// @param brush 画刷
    /// @param range 文字范围
    void SetFillBrush(BrushPtr brush, TextRange range);

    /// \~chinese
    /// @brief 设置文字描边画刷
    /// @param brush 画刷
    /// @param range 文字范围
    void SetOutlineBrush(BrushPtr brush, TextRange range);

    /// \~chinese
    /// @brief 设置描边线条样式
    /// @param stroke 线条样式
    /// @param range 文字范围
    void SetOutlineStrokeStyle(StrokeStylePtr stroke, TextRange range);

    /// \~chinese
    /// @brief 设置对齐方式
    /// @param align 对齐方式
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief 设置文本自动换行的宽度
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief 设置行间距（默认为 0）
    void SetLineSpacing(float line_spacing);

    /// \~chinese
    /// @brief 设置默认文字填充画刷
    /// @param brush 画刷
    void SetDefaultFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置默认文字描边画刷
    /// @param brush 画刷
    void SetDefaultOutlineBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置默认描边线条样式
    /// @param stroke 线条样式
    void SetDefaultOutlineStrokeStyle(StrokeStylePtr stroke);

    /// \~chinese
    /// @brief 脏布局标志
    enum class DirtyFlag : uint8_t
    {
        Clean = 0,      ///< 干净布局
        Dirty = 1 << 0  ///< 脏布局
    };

    /// \~chinese
    /// @brief 获取脏布局标志
    DirtyFlag GetDirtyFlag() const;

    /// \~chinese
    /// @brief 设置脏布局标志
    void SetDirtyFlag(DirtyFlag flag);

    /// \~chinese
    /// @brief 更新脏布局
    /// @return 是否需要更新
    bool UpdateWhenDirty();

private:
    DirtyFlag      dirty_flag_;
    uint32_t       line_count_;
    Size           size_;
    BrushPtr       default_fill_brush_;
    BrushPtr       default_outline_brush_;
    StrokeStylePtr default_outline_stroke_;
    String         content_;
};

/** @} */

inline bool TextLayout::IsDirty() const
{
    return dirty_flag_ != DirtyFlag::Clean;
}

inline void TextLayout::Clear()
{
    ResetNativePointer();
}

inline TextLayout::DirtyFlag TextLayout::GetDirtyFlag() const
{
    return dirty_flag_;
}

inline void TextLayout::SetDirtyFlag(TextLayout::DirtyFlag flag)
{
    dirty_flag_ = flag;
}

inline BrushPtr TextLayout::GetDefaultFillBrush() const
{
    return default_fill_brush_;
}

inline BrushPtr TextLayout::GetDefaultOutlineBrush() const
{
    return default_outline_brush_;
}

inline StrokeStylePtr TextLayout::GetDefaultOutlineStrokeStyle() const
{
    return default_outline_stroke_;
}

inline void TextLayout::SetDefaultFillBrush(BrushPtr brush)
{
    default_fill_brush_ = brush;
}

inline void TextLayout::SetDefaultOutlineBrush(BrushPtr brush)
{
    default_outline_brush_ = brush;
}

inline void TextLayout::SetDefaultOutlineStrokeStyle(StrokeStylePtr stroke)
{
    default_outline_stroke_ = stroke;
}

}  // namespace kiwano
