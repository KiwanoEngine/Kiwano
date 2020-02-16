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
 * @brief 字体粗细值
 */
struct FontWeight
{
    enum Value : uint32_t
    {
        Thin       = 100U,
        ExtraLight = 200U,
        Light      = 300U,
        Normal     = 400U,  ///< 正常
        Medium     = 500U,
        Bold       = 700U,
        ExtraBold  = 800U,
        Black      = 900U,
        ExtraBlack = 950U
    };
};

/**
 * \~chinese
 * @brief 文本样式
 */
class KGE_API TextStyle
{
public:
    FontPtr        font;                ///< 字体
    String         font_family;         ///< 字体族
    float          font_size;           ///< 字号
    uint32_t       font_weight;         ///< 粗细值
    bool           italic;              ///< 是否斜体
    TextAlign      alignment;           ///< 对齐方式
    BrushPtr       fill_brush;          ///< 填充画刷
    BrushPtr       outline_brush;       ///< 描边画刷
    StrokeStylePtr outline_stroke;      ///< 描边样式
    float          wrap_width;          ///< 自动换行宽度
    float          line_spacing;        ///< 行间距
    bool           show_underline;      ///< 显示下划线
    bool           show_strikethrough;  ///< 显示删除线

public:
    /**
     * \~chinese
     * @brief 构建默认文本样式
     */
    TextStyle();

    /**
     * \~chinese
     * @brief 构建文本样式
     * @param font_family 字体族
     * @param font_size 字体大小
     * @param font_weight 字体粗细
     */
    TextStyle(const String& font_family, float font_size, uint32_t font_weight = FontWeight::Normal);
};

/** @} */

inline TextStyle::TextStyle()
    : TextStyle(String(), 18, FontWeight::Normal)
{
}

inline TextStyle::TextStyle(const String& font_family, float font_size, uint32_t font_weight)
    : font(nullptr)
    , font_family(font_family)
    , font_size(font_size)
    , font_weight(font_weight)
    , italic(false)
    , alignment(TextAlign::Left)
    , wrap_width(0)
    , line_spacing(0)
    , show_underline(false)
    , show_strikethrough(false)
{
}

}  // namespace kiwano
