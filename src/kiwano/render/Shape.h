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
#include <kiwano/render/DirectX/D2DDeviceResources.h>

namespace kiwano
{
class ShapeSink;

KGE_DECLARE_SMART_PTR(Shape);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 形状
 */
class KGE_API Shape : public virtual ObjectBase
{
    friend class ShapeSink;

public:
    /// \~chinese
    /// @brief 创建线段形状
    /// @param begin 线段起点
    /// @param end 线段终点
    static ShapePtr CreateLine(Point const& begin, Point const& end);

    /// \~chinese
    /// @brief 创建矩形
    /// @param rect 矩形
    static ShapePtr CreateRect(Rect const& rect);

    /// \~chinese
    /// @brief 创建圆角矩形
    /// @param rect 矩形
    /// @param radius 矩形圆角半径
    static ShapePtr CreateRoundedRect(Rect const& rect, Vec2 const& radius);

    /// \~chinese
    /// @brief 创建圆形
    /// @param center 圆形原点
    /// @param radius 圆形半径
    static ShapePtr CreateCircle(Point const& center, float radius);

    /// \~chinese
    /// @brief 创建椭圆形
    /// @param center 椭圆原点
    /// @param radius 椭圆半径
    static ShapePtr CreateEllipse(Point const& center, Vec2 const& radius);

    Shape();

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 获取外切包围盒
    Rect GetBoundingBox() const;

    /// \~chinese
    /// @brief 获取外切包围盒
    /// @param transform 二维变换
    Rect GetBoundingBox(Matrix3x2 const& transform) const;

    /// \~chinese
    /// @brief 判断图形是否包含点
    /// @param point 点
    /// @param transform 应用到点上的二维变换
    bool ContainsPoint(Point const& point, const Matrix3x2* transform = nullptr) const;

    /// \~chinese
    /// @brief 获取图形展开成一条直线的长度
    float GetLength() const;

    /// \~chinese
    /// @brief 计算图形面积
    float ComputeArea() const;

    /// \~chinese
    /// @brief 计算图形上点的位置和切线向量
    /// @param[in] length 点在图形上的位置，范围 [0.0 - 1.0]
    /// @param[out] point 点的位置
    /// @param[out] tangent 点的切线向量
    bool ComputePointAtLength(float length, Point& point, Vec2& tangent) const;

    /// \~chinese
    /// @brief 清除形状
    void Clear();

#if defined(KGE_WIN32)
public:
    ComPtr<ID2D1Geometry> GetGeometry() const;

    void SetGeometry(ComPtr<ID2D1Geometry> shape);

private:
    ComPtr<ID2D1Geometry> geo_;
#endif
};

/** @} */

#if defined(KGE_WIN32)
inline ComPtr<ID2D1Geometry> Shape::GetGeometry() const
{
    return geo_;
}

inline void Shape::SetGeometry(ComPtr<ID2D1Geometry> shape)
{
    geo_ = shape;
}
#endif

}  // namespace kiwano
