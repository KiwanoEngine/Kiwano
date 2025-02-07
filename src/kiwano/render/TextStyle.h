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
#include <kiwano/render/Brush.h>
#include <kiwano/render/Color.h>
#include <kiwano/render/Font.h>
#include <kiwano/render/StrokeStyle.h>

namespace kiwano
{
/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 文本对齐方式
 */
enum class TextAlign
{
    Left,      ///< 左对齐
    Right,     ///< 右对齐
    Center,    ///< 居中对齐
    Justified  ///< 两端对齐
};
/**
 * \~chinese
 * @brief 换行模式
 */
enum class TextWordWrapping
{
    WRAPPING_WRAP,              ///< 在单词边界处换行，这是实现自动换行的常用模式
    WRAPPING_NO_WRAP,           ///< 不进行换行，文本将超出布局边界
    WRAPPING_EMERGENCY_BREAK,   ///< 在必要时在任意字符处换行，即使不在单词边界
    WRAPPING_WHOLE_WORD,        ///< 仅在完整的单词边界处进行换行
    WRAPPING_CHARACTER,         ///< 可以在任意字符处进行换行
};

/**
 * \~chinese
 * @brief 文本样式
 */
class KGE_API TextStyle
{
public:
    bool      show_underline;      ///< 显示下划线
    bool      show_strikethrough;  ///< 显示删除线
    float     wrap_width;          ///< 自动换行宽度
    float     line_spacing;        ///< 行间距
    TextWordWrapping word_wrapping;///< 换行模式
    TextAlign alignment;           ///< 对齐方式
    Font      font;                ///< 字体

public:
    /**
     * \~chinese
     * @brief 构建默认文本样式
     */
    TextStyle();

    /**
     * \~chinese
     * @brief 构建文本样式
     * @param font 字体
     */
    TextStyle(Font font);

    /**
     * \~chinese
     * @brief 构建文本样式
     * @param font_family 字体族
     * @param font_size 字体大小
     * @param font_weight 字体粗细
     */
    TextStyle(StringView font_family, float font_size, uint32_t font_weight = FontWeight::Normal);
};

/** @} */

}  // namespace kiwano
