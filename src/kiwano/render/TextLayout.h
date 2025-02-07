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
#include <kiwano/platform/NativeObject.hpp>
#include <kiwano/render/TextStyle.h>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief 文本布局
class KGE_API TextLayout : public NativeObject
{
public:
    TextLayout();

    /// \~chinese
    /// @brief 创建文本布局
    /// @param content 文字内容
    /// @param style 文本样式
    TextLayout(StringView content, const TextStyle& style);

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
    void Reset(StringView content, const TextStyle& style);

    /// \~chinese
    /// @brief 获取文本布局大小
    Size GetSize() const;

    /// \~chinese
    /// @brief 获取文本行数
    uint32_t GetLineCount() const;

    /// \~chinese
    /// @brief 获取文本长度
    uint32_t GetContentLength() const;

    /// \~chinese
    /// @brief 设置字体
    /// @param font 字体
    void SetFont(const Font& font);

    /// \~chinese
    /// @brief 设置下划线
    /// @param enable 是否显示下划线
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief 设置删除线
    /// @param enable 是否显示删除线
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief 设置对齐方式
    /// @param align 对齐方式
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief 设置文本自动换行的宽度
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief 设置文本自动换行的模式
    void SetWordWrapping(TextWordWrapping wrap_width);

    /// \~chinese
    /// @brief 设置行间距（默认为 0）
    void SetLineSpacing(float line_spacing);

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
    bool UpdateIfDirty();

private:
    DirtyFlag dirty_flag_;
    uint32_t  line_count_;
    uint32_t  content_length_;
    Size      size_;
};

/** @} */

inline bool TextLayout::IsDirty() const
{
    return dirty_flag_ != DirtyFlag::Clean;
}

inline uint32_t TextLayout::GetContentLength() const
{
    return content_length_;
}

inline TextLayout::DirtyFlag TextLayout::GetDirtyFlag() const
{
    return dirty_flag_;
}

inline void TextLayout::SetDirtyFlag(TextLayout::DirtyFlag flag)
{
    dirty_flag_ = flag;
}

}  // namespace kiwano
