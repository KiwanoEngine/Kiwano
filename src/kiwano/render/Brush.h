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
#include <kiwano/platform/NativeObject.hpp>
#include <kiwano/render/Color.h>
#include <kiwano/render/Bitmap.h>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief 渐变转换点
struct GradientStop
{
    float offset;  ///< 偏移距离
    Color color;   ///< 渐变点颜色

    GradientStop();

    GradientStop(float offset, Color color);
};

/// \~chinese
/// @brief 渐变扩充模式
/// @details 该模式用于指定画笔如何绘制正常区域外的部分
enum class GradientExtendMode
{
    Clamp,  ///< 夹模式，重复绘制边界颜色
    Wrap,   ///< 包裹模式，重复画笔内容
    Mirror  ///< 镜像模式，反转画笔内容
};

/// \~chinese
/// @brief 线性渐变样式
struct LinearGradientStyle
{
    Point                begin;        ///< 渐变起始点
    Point                end;          ///< 渐变终止点
    Vector<GradientStop> stops;        ///< 渐变转换点集合
    GradientExtendMode   extend_mode;  ///< 渐变扩充模式

    LinearGradientStyle();

    LinearGradientStyle(const Point& begin, const Point& end, const Vector<GradientStop>& stops,
                        GradientExtendMode extend_mode = GradientExtendMode::Clamp);
};

/// \~chinese
/// @brief 径向渐变样式
struct RadialGradientStyle
{
    Point                center;       ///< 径向渐变圆心
    Vec2                 offset;       ///< 径向渐变偏移
    Vec2                 radius;       ///< 径向渐变半径
    Vector<GradientStop> stops;        ///< 渐变转换点集合
    GradientExtendMode   extend_mode;  ///< 渐变扩充模式

    RadialGradientStyle();

    RadialGradientStyle(const Point& center, const Vec2& offset, const Vec2& radius, const Vector<GradientStop>& stops,
                        GradientExtendMode extend_mode = GradientExtendMode::Clamp);
};

/**
 * \~chinese
 * @brief 画刷
 */
class KGE_API Brush : public NativeObject
{
public:
    /// \~chinese
    /// @brief 创建纯色画刷
    /// @param color 画刷颜色
    Brush(const Color& color);

    /// \~chinese
    /// @brief 创建线性渐变样式
    /// @param style 线性渐变样式
    Brush(const LinearGradientStyle& style);

    /// \~chinese
    /// @brief 创建径向渐变样式
    /// @param style 径向渐变样式
    Brush(const RadialGradientStyle& style);

    Brush();

    /// \~chinese
    /// @brief 设置纯色画刷颜色
    void SetColor(const Color& color);

    /// \~chinese
    /// @brief 设置线性渐变样式
    void SetStyle(const LinearGradientStyle& style);

    /// \~chinese
    /// @brief 设置径向渐变样式
    void SetStyle(const RadialGradientStyle& style);

    /// \~chinese
    /// @brief 设置图像
    void SetImage(RefPtr<Image> image, const Rect& src_rect);

    /// \~chinese
    /// @brief 设置位图
    void SetBitmap(RefPtr<Bitmap> bitmap);

    /// \~chinese
    /// @brief 设置二维变换
    void SetTransform(const Transform& transform);

    /// \~chinese
    /// @brief 设置二维变换
    void SetTransform(const Matrix3x2& transform);

public:
    /// \~chinese
    /// @brief 画刷类型
    enum class Type
    {
        None,
        SolidColor,      ///< 纯色填充画刷
        LinearGradient,  ///< 线性渐变画刷
        RadialGradient,  ///< 径向渐变画刷
        Image            ///< 图像画刷
    };

    /// \~chinese
    /// @brief 获取画刷类型
    Type GetType() const;

private:
    Type type_;
};

/** @} */

inline Brush::Type Brush::GetType() const
{
    return type_;
}

}  // namespace kiwano
