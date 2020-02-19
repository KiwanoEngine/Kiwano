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
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/TextureRenderContext.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Canvas);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 画布
 * @details 用于绘制图形、图像、文字等各种类型的图元，同时可以将绘制内容导出至图像
 */
class KGE_API Canvas : public Actor
{
public:
    /// \~chinese
    /// @brief 创建画布
    /// @param size 画布大小
    static CanvasPtr Create(const Size& size);

    /// \~chinese
    /// @brief 开始绘图
    void BeginDraw();

    /// \~chinese
    /// @brief 结束绘图
    void EndDraw();

    /// \~chinese
    /// @brief 画形状轮廓
    /// @param shape 形状
    void DrawShape(ShapePtr shape);

    /// \~chinese
    /// @brief 画线段
    /// @param begin 线段起点
    /// @param end 线段终点
    void DrawLine(const Point& begin, const Point& end);

    /// \~chinese
    /// @brief 画圆形边框
    /// @param center 圆形原点
    /// @param radius 圆形半径
    void DrawCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief 画椭圆形边框
    /// @param center 椭圆原点
    /// @param radius 椭圆半径
    void DrawEllipse(const Point& center, const Vec2& radius);

    /// \~chinese
    /// @brief 画矩形边框
    /// @param rect 矩形
    void DrawRect(const Rect& rect);

    /// \~chinese
    /// @brief 画圆角矩形边框
    /// @param rect 矩形
    /// @param radius 矩形圆角半径
    void DrawRoundedRect(const Rect& rect, const Vec2& radius);

    /// \~chinese
    /// @brief 填充形状
    /// @param shape 形状
    void FillShape(ShapePtr shape);

    /// \~chinese
    /// @brief 填充圆形
    /// @param center 圆形原点
    /// @param radius 圆形半径
    void FillCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief 填充椭圆形
    /// @param center 椭圆原点
    /// @param radius 椭圆半径
    void FillEllipse(const Point& center, const Vec2& radius);

    /// \~chinese
    /// @brief 填充矩形
    /// @param rect 矩形
    void FillRect(const Rect& rect);

    /// \~chinese
    /// @brief 填充圆角矩形
    /// @param rect 矩形
    /// @param radius 矩形圆角半径
    void FillRoundedRect(const Rect& rect, const Vec2& radius);

    /// \~chinese
    /// @brief 绘制纹理
    /// @param texture 纹理
    /// @param src_rect 纹理裁剪区域
    /// @param dest_rect 绘制目标区域
    void DrawTexture(TexturePtr texture, const Rect* src_rect = nullptr, const Rect* dest_rect = nullptr);

    /// \~chinese
    /// @brief 绘制文字布局
    /// @param text 文字
    /// @param style 文字样式
    /// @param point 绘制文字的位置
    void DrawTextLayout(const String& text, const TextStyle& style, const Point& point);

    /// \~chinese
    /// @brief 绘制文字布局
    /// @param layout 文字布局
    /// @param point 绘制布局的位置
    void DrawTextLayout(TextLayoutPtr layout, const Point& point);

    /// \~chinese
    /// @brief 开始绘制路径
    /// @param begin_pos 路径起始点
    void BeginPath(const Point& begin_pos);

    /// \~chinese
    /// @brief 结束路径
    /// @param closed 路径是否闭合
    void EndPath(bool closed = false);

    /// \~chinese
    /// @brief 添加一条线段
    /// @param point 端点
    void AddLine(const Point& point);

    /// \~chinese
    /// @brief 添加多条线段
    /// @param points 端点集合
    void AddLines(const Vector<Point>& points);

    /// \~chinese
    /// @brief 添加一条三次方贝塞尔曲线
    /// @param point1 贝塞尔曲线的第一个控制点
    /// @param point2 贝塞尔曲线的第二个控制点
    /// @param point3 贝塞尔曲线的终点
    void AddBezier(const Point& point1, const Point& point2, const Point& point3);

    /// \~chinese
    /// @brief 添加弧线
    /// @param point 终点
    /// @param radius 椭圆半径
    /// @param rotation 椭圆旋转角度
    /// @param clockwise 顺时针 or 逆时针
    /// @param is_small 是否取小于 180° 的弧
    void AddArc(const Point& point, const Size& radius, float rotation, bool clockwise = true, bool is_small = true);

    /// \~chinese
    /// @brief 以描边的方式绘制路径
    void StrokePath();

    /// \~chinese
    /// @brief 以填充的方式绘制路径
    void FillPath();

    /// \~chinese
    /// @brief 清空画布
    void Clear();

    /// \~chinese
    /// @brief 清空画布
    /// @param clear_color 清空颜色
    void Clear(const Color& clear_color);

    /// \~chinese
    /// @brief 设置填充颜色
    /// @param color 填充颜色
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief 设置填充画刷
    /// @param[in] brush 填充画刷
    void SetFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置轮廓颜色
    /// @param color 轮廓颜色
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief 设置轮廓画刷
    /// @param[in] brush 轮廓画刷
    void SetStrokeBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置轮廓样式
    /// @param stroke_style 轮廓样式
    void SetStrokeStyle(StrokeStylePtr stroke_style);

    /// \~chinese
    /// @brief 设置画刷
    /// @param[in] brush 画刷
    void SetBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 设置画刷二维变换
    /// @param transform 二维变换
    void SetBrushTransform(const Transform& transform);

    /// \~chinese
    /// @brief 设置画刷二维变换矩阵
    /// @param transform 二维变换矩阵
    void SetBrushTransform(const Matrix3x2& transform);

    /// \~chinese
    /// @brief 添加一个图层
    /// @param layer 图层
    void PushLayer(LayerPtr layer);

    /// \~chinese
    /// @brief 删除最近添加的图层
    void PopLayer();

    /// \~chinese
    /// @brief 添加一个裁剪区域
    /// @param clip_rect 裁剪矩形
    void PushClipRect(const Rect& clip_rect);

    /// \~chinese
    /// @brief 删除最近添加的裁剪区域
    void PopClipRect();

    /// \~chinese
    /// @brief 获取填充画刷
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief 获取轮廓画刷
    BrushPtr GetStrokeBrush() const;

    /// \~chinese
    /// @brief 清空画布大小并重设画布大小
    void ResizeAndClear(Size size);

    /// \~chinese
    /// @brief 导出纹理
    TexturePtr ExportToTexture() const;

    void OnRender(RenderContext& ctx) override;

private:
    Canvas();

    void UpdateCache() const;

private:
    StrokeStylePtr stroke_style_;
    ShapeMaker     shape_maker_;
    BrushPtr       fill_brush_;
    BrushPtr       stroke_brush_;

    mutable bool                    cache_expired_;
    mutable TexturePtr              texture_cached_;
    mutable TextureRenderContextPtr ctx_;
};

/** @} */

inline void Canvas::SetStrokeStyle(StrokeStylePtr stroke_style)
{
    stroke_style_ = stroke_style;
}

inline void Canvas::SetStrokeColor(const Color& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ = new Brush;
    }
    stroke_brush_->SetColor(color);
}

inline void Canvas::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ = new Brush;
    }
    fill_brush_->SetColor(color);
}

inline void Canvas::SetFillBrush(BrushPtr brush)
{
    fill_brush_ = brush;
}

inline void Canvas::SetStrokeBrush(BrushPtr brush)
{
    stroke_brush_ = brush;
}

inline BrushPtr Canvas::GetFillBrush() const
{
    return fill_brush_;
}

inline BrushPtr Canvas::GetStrokeBrush() const
{
    return stroke_brush_;
}

}  // namespace kiwano
