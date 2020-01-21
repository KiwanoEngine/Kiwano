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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/render/dx/D2DDeviceResources.h>

namespace kiwano
{
class RenderContext;
class Renderer;

KGE_DECLARE_SMART_PTR(Brush);

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

    LinearGradientStyle(Point const& begin, Point const& end, Vector<GradientStop> const& stops,
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

    RadialGradientStyle(Point const& center, Vec2 const& offset, Vec2 const& radius, Vector<GradientStop> const& stops,
                        GradientExtendMode extend_mode = GradientExtendMode::Clamp);
};

/**
 * \~chinese
 * @brief 画刷
 */
class KGE_API Brush : public virtual ObjectBase
{
    friend class RenderContext;
    friend class Renderer;

public:
    /// \~chinese
    /// @brief 构造默认画刷
    Brush();

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 设置纯色画刷颜色
    void SetColor(Color const& color);

    /// \~chinese
    /// @brief 设置线性渐变样式
    void SetStyle(LinearGradientStyle const& style);

    /// \~chinese
    /// @brief 设置径向渐变样式
    void SetStyle(RadialGradientStyle const& style);

public:
    /// \~chinese
    /// @brief 画刷类型
    enum class Type
    {
        Unknown,
        SolidColor,      ///< 纯色填充画刷
        LinearGradient,  ///< 线性渐变画刷
        RadialGradient   ///< 径向渐变画刷
    };

    /// \~chinese
    /// @brief 获取画刷类型
    Type GetType() const;

private:
    /// \~chinese
    /// @brief 获取透明度
    float GetOpacity() const;

    /// \~chinese
    /// @brief 设置透明度
    void SetOpacity(float opacity);

    void SetBrush(ComPtr<ID2D1Brush> brush, Type type);

    ComPtr<ID2D1Brush> GetBrush() const;

private:
    Type               type_;
    float              opacity_;
    ComPtr<ID2D1Brush> raw_;
};

/** @} */

inline Brush::Type Brush::GetType() const
{
    return type_;
}

inline ComPtr<ID2D1Brush> Brush::GetBrush() const
{
    return raw_;
}
}  // namespace kiwano
