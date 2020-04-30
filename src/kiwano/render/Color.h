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
#include <kiwano/core/Common.h>
#include <kiwano/macros.h>

namespace kiwano
{
/**
 * \addtogroup Render
 * @{
 */

/*
 * \~chinese
 * @brief 颜色
 * @details
 * 使用枚举表示颜色: @code Color blue = Color::Blue; @endcode
 * 使用 RGB 表示一个颜色: @code Color red = Color(1.0f, 0.0f, 0.0f); @endcode
 * 使用 RGBA 表示一个带透明度的颜色: @code Color not_white = Color(1.0f, 1.0f,
 * 1.0f, 0.5f); @endcode
 * 使用一个16进制整型值表示 RGB 颜色: @code Color black(0x000000); @endcode
 */
class KGE_API Color
{
public:
    /// \~chinese
    /// @brief 构造颜色
    /// @details 默认颜色为 R: 0.0, G: 0.0, B: 0.0, A: 1.0
    Color();

    /// \~chinese
    /// @brief 构造 RGB 颜色
    /// @param r 红色值，范围 0.0 - 1.0
    /// @param g 绿色值，范围 0.0 - 1.0
    /// @param b 蓝色值，范围 0.0 - 1.0
    Color(float r, float g, float b);

    /// \~chinese
    /// @brief 构造 RGBA 颜色
    /// @param r 红色值，范围 0.0 - 1.0
    /// @param g 绿色值，范围 0.0 - 1.0
    /// @param b 蓝色值，范围 0.0 - 1.0
    /// @param alpha Alpha值，范围 0.0 - 1.0
    Color(float r, float g, float b, float alpha);

    /// \~chinese
    /// @brief 构造 RGB 颜色
    /// @param rgb 使用16进制整形值表示 RGB颜色
    Color(uint32_t rgb);

    /// \~chinese
    /// @brief 构造 RGBA 颜色
    /// @param rgb 使用16进制整形值表示 RGB 颜色
    /// @param alpha Alpha值，范围 0.0 - 1.0
    Color(uint32_t rgb, float alpha);

    /// \~chinese
    /// @brief 构造 RGB 颜色
    /// @param r 红色值，范围 0.0 - 1.0
    /// @param g 绿色值，范围 0.0 - 1.0
    /// @param b 蓝色值，范围 0.0 - 1.0
    static Color RGB(float r, float g, float b);

    /// \~chinese
    /// @brief 构造 RGB 颜色
    /// @param rgb 使用16进制整形值表示 RGB颜色
    static Color RGB(uint32_t rgb);

    /// \~chinese
    /// @brief 构造 RGBA 颜色
    /// @param r 红色值，范围 0.0 - 1.0
    /// @param g 绿色值，范围 0.0 - 1.0
    /// @param b 蓝色值，范围 0.0 - 1.0
    /// @param alpha Alpha值，范围 0.0 - 1.0
    static Color RGBA(float r, float g, float b, float alpha);

    /// \~chinese
    /// @brief 构造 RGBA 颜色
    /// @param rgb 使用16进制整形值表示 RGB颜色
    /// @param alpha Alpha值，范围 0.0 - 1.0
    static Color RGBA(uint32_t rgb, float alpha);

    bool operator==(const Color& rhs) const;

    bool operator!=(const Color& rhs) const;

public:
    /// \~chinese
    /// @brief 常见颜色枚举
    enum Value : uint32_t
    {
        Black         = 0x000000,
        Blue          = 0x0000FF,
        BlueViolet    = 0x8A2BE2,
        Brown         = 0xA52A2A,
        Chocolate     = 0xD2691E,
        DarkBlue      = 0x00008B,
        DarkGray      = 0xA9A9A9,
        DarkGreen     = 0x006400,
        DarkOrange    = 0xFF8C00,
        DarkRed       = 0x8B0000,
        DarkViolet    = 0x9400D3,
        ForestGreen   = 0x228B22,
        Gold          = 0xFFD700,
        Gray          = 0x808080,
        Green         = 0x008000,
        GreenYellow   = 0xADFF2F,
        LightBlue     = 0xADD8E6,
        LightCyan     = 0xE0FFFF,
        LightGreen    = 0x90EE90,
        LightGray     = 0xD3D3D3,
        LightPink     = 0xFFB6C1,
        LightSeaGreen = 0x20B2AA,
        LightSkyBlue  = 0x87CEFA,
        LightYellow   = 0xFFFFE0,
        Orange        = 0xFFA500,
        OrangeRed     = 0xFF4500,
        Pink          = 0xFFC0CB,
        Purple        = 0x800080,
        Red           = 0xFF0000,
        Silver        = 0xC0C0C0,
        SkyBlue       = 0x87CEEB,
        Snow          = 0xFFFAFA,
        Violet        = 0xEE82EE,
        Wheat         = 0xF5DEB3,
        White         = 0xFFFFFF,
        WhiteSmoke    = 0xF5F5F5,
        Wood          = 0xDEB887,
        Yellow        = 0xFFFF00,
        YellowGreen   = 0x9ACD32
    };

    /// \~chinese
    /// @brief 透明色
    static const Color Transparent;

public:
    float r;  ///< 红色值
    float g;  ///< 绿色值
    float b;  ///< 蓝色值
    float a;  ///< Alpha值
};

/** @} */

inline bool Color::operator==(const Color& rhs) const
{
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
}

inline bool Color::operator!=(const Color& rhs) const
{
    return !((*this) == rhs);
}
}  // namespace kiwano
