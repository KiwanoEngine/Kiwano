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
#include <kiwano/render/Brush.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/StrokeStyle.h>

namespace kiwano
{

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 形状角色
 */
class KGE_API ShapeActor : public Actor
{
public:
    ShapeActor();

    /// \~chinese
    /// @brief 创建形状角色
    /// @param shape 形状
    ShapeActor(RefPtr<Shape> shape);

    /// \~chinese
    /// @brief 创建形状角色
    /// @param shape 形状
    /// @param fill_color 填充颜色
    /// @param stroke_color 轮廓颜色
    ShapeActor(RefPtr<Shape> shape, const Color& fill_color, const Color& stroke_color);

    /// \~chinese
    /// @brief 创建形状角色
    /// @param shape 形状
    /// @param fill_brush 填充画刷
    /// @param stroke_brush 轮廓画刷
    ShapeActor(RefPtr<Shape> shape, RefPtr<Brush> fill_brush, RefPtr<Brush> stroke_brush);

    virtual ~ShapeActor();

    /// \~chinese
    /// @brief 获取填充画刷
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief 获取轮廓画刷
    RefPtr<Brush> GetStrokeBrush() const;

    /// \~chinese
    /// @brief 获取线条样式
    RefPtr<StrokeStyle> GetStrokeStyle() const;

    /// \~chinese
    /// @brief 获取形状
    RefPtr<Shape> GetShape() const;

    /// \~chinese
    /// @brief 获取边界
    Rect GetBounds() const override;

    /// \~chinese
    /// @brief 获取外切包围盒
    Rect GetBoundingBox() const override;

    /// \~chinese
    /// @brief 判断点是否在形状内
    bool ContainsPoint(const Point& point) const override;

    /// \~chinese
    /// @brief 设置填充颜色
    /// @param color 填充颜色
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief 设置填充画刷
    /// @param[in] brush 填充画刷
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 设置轮廓颜色
    /// @param color 轮廓颜色
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief 设置轮廓画刷
    /// @param[in] brush 轮廓画刷
    void SetStrokeBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 设置线条样式
    void SetStrokeStyle(RefPtr<StrokeStyle> stroke_style);

    /// \~chinese
    /// @brief 设置形状
    void SetShape(RefPtr<Shape> shape);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    RefPtr<Brush>       fill_brush_;
    RefPtr<Brush>       stroke_brush_;
    RefPtr<StrokeStyle> stroke_style_;
    Rect           bounds_;
    RefPtr<Shape>       shape_;
};

/// \~chinese
/// @brief 线段角色
class KGE_API LineActor : public ShapeActor
{
public:
    LineActor();

    /// \~chinese
    /// @brief 创建线段角色
    /// @param begin 线段起点
    /// @param end 线段终点
    LineActor(const Point& begin, const Point& end);

    virtual ~LineActor();

    /// \~chinese
    /// @brief 获取线段起点
    const Point& GetBeginPoint() const;

    /// \~chinese
    /// @brief 获取线段终点
    const Point& GetEndPoint() const;

    /// \~chinese
    /// @brief 设置线段起点
    /// @param begin 线段起点
    void SetBeginPoint(const Point& begin);

    /// \~chinese
    /// @brief 设置线段终点
    /// @param end 线段终点
    void SetEndPoint(const Point& end);

    /// \~chinese
    /// @brief 设置线段起点和终点
    /// @param begin 线段起点
    /// @param end 线段终点
    void SetLine(const Point& begin, const Point& end);

private:
    Point begin_;
    Point end_;
};

/// \~chinese
/// @brief 矩形角色
class KGE_API RectActor : public ShapeActor
{
public:
    RectActor();

    /// \~chinese
    /// @brief 创建矩形角色
    /// @param size 矩形大小
    RectActor(const Size& size);

    virtual ~RectActor();

    /// \~chinese
    /// @brief 获取矩形大小
    const Size& GetRectSize() const;

    /// \~chinese
    /// @brief 设置矩形大小
    /// @param size 矩形大小
    void SetRectSize(const Size& size);

private:
    Size rect_size_;
};

/// \~chinese
/// @brief 圆角矩形角色
class KGE_API RoundedRectActor : public ShapeActor
{
public:
    RoundedRectActor();

    /// \~chinese
    /// @brief 创建圆角矩形角色
    /// @param size 圆角矩形大小
    /// @param radius 圆角半径
    RoundedRectActor(const Size& size, const Vec2& radius);

    virtual ~RoundedRectActor();

    /// \~chinese
    /// @brief 获取圆角半径
    Vec2 GetRadius() const;

    /// \~chinese
    /// @brief 获取圆角矩形大小
    Size GetRectSize() const;

    /// \~chinese
    /// @brief 设置圆角半径
    /// @param radius 圆角半径
    void SetRadius(const Vec2& radius);

    /// \~chinese
    /// @brief 设置圆角矩形大小
    /// @param size 圆角矩形大小
    void SetRectSize(const Size& size);

    /// \~chinese
    /// @brief 设置圆角矩形
    /// @param size 圆角矩形大小
    /// @param radius 圆角半径
    void SetRoundedRect(const Size& size, const Vec2& radius);

private:
    Size rect_size_;
    Vec2 radius_;
};

/// \~chinese
/// @brief 圆形角色
class KGE_API CircleActor : public ShapeActor
{
public:
    CircleActor();

    /// \~chinese
    /// @brief 创建圆形角色
    /// @param radius 圆形半径
    CircleActor(float radius);

    virtual ~CircleActor();

    /// \~chinese
    /// @brief 获取圆形半径
    float GetRadius() const;

    /// \~chinese
    /// @brief 设置圆形半径
    /// @param radius 圆形半径
    void SetRadius(float radius);

private:
    float radius_;
};

/// \~chinese
/// @brief 椭圆角色
class KGE_API EllipseActor : public ShapeActor
{
public:
    EllipseActor();

    /// \~chinese
    /// @brief 创建椭圆角色
    /// @param radius 椭圆半径
    EllipseActor(const Vec2& radius);

    virtual ~EllipseActor();

    /// \~chinese
    /// @brief 获取椭圆半径
    Vec2 GetRadius() const;

    /// \~chinese
    /// @brief 设置椭圆半径
    /// @param radius 椭圆半径
    void SetRadius(const Vec2& radius);

private:
    Vec2 radius_;
};

/// \~chinese
/// @brief 多边形角色
class KGE_API PolygonActor : public ShapeActor
{
public:
    PolygonActor();

    /// \~chinese
    /// @brief 创建多边形角色
    /// @param vertices 多边形端点集合
    PolygonActor(const Vector<Point>& vertices);

    virtual ~PolygonActor();

    /// \~chinese
    /// @brief 设置多边形端点
    /// @param vertices 多边形端点集合
    void SetVertices(const Vector<Point>& vertices);
};


/** @} */

inline void ShapeActor::SetStrokeColor(const Color& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ =  MakePtr<Brush>();
    }
    stroke_brush_->SetColor(color);
}

inline void ShapeActor::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ =  MakePtr<Brush>();
    }
    fill_brush_->SetColor(color);
}

inline void ShapeActor::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_ = brush;
}

inline void ShapeActor::SetStrokeBrush(RefPtr<Brush> brush)
{
    stroke_brush_ = brush;
}

inline RefPtr<Brush> ShapeActor::GetFillBrush() const
{
    return fill_brush_;
}

inline RefPtr<Brush> ShapeActor::GetStrokeBrush() const
{
    return stroke_brush_;
}

inline RefPtr<StrokeStyle> ShapeActor::GetStrokeStyle() const
{
    return stroke_style_;
}

inline RefPtr<Shape> ShapeActor::GetShape() const
{
    return shape_;
}

inline void ShapeActor::SetStrokeStyle(RefPtr<StrokeStyle> stroke_style)
{
    stroke_style_ = stroke_style;
}

inline const Point& LineActor::GetBeginPoint() const
{
    return begin_;
}

inline const Point& LineActor::GetEndPoint() const
{
    return end_;
}

inline void LineActor::SetBeginPoint(const Point& begin)
{
    SetLine(begin, end_);
}

inline void LineActor::SetEndPoint(const Point& end)
{
    SetLine(begin_, end);
}

inline const Size& RectActor::GetRectSize() const
{
    return rect_size_;
}

inline Vec2 RoundedRectActor::GetRadius() const
{
    return radius_;
}

inline Size RoundedRectActor::GetRectSize() const
{
    return GetSize();
}

inline float CircleActor::GetRadius() const
{
    return radius_;
}

inline Vec2 EllipseActor::GetRadius() const
{
    return radius_;
}

}  // namespace kiwano
