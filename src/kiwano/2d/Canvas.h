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
#include <kiwano/render/RenderContext.h>

namespace kiwano
{
class CanvasRenderContext;

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 画布
 * @details 用于绘制图形、图像、文字等各种类型的图元，同时可以将绘制内容导出至位图
 */
class KGE_API Canvas : public Actor
{
public:
    /// \~chinese
    /// @brief 创建画布
    /// @warning 必须调用 ResizeAndClear 以初始化画布
    Canvas();

    /// \~chinese
    /// @brief 创建画布
    /// @param size 画布大小
    Canvas(const Size& size);

    /// \~chinese
    /// @brief 获取2D绘图上下文
    RefPtr<CanvasRenderContext> GetContext2D() const;

    /// \~chinese
    /// @brief 清空画布大小并重设画布大小
    /// @warning 该函数会导致原绘图上下文失效
    void ResizeAndClear(const Size& size);

    /// \~chinese
    /// @brief 获取输出目标
    RefPtr<Bitmap> GetTarget() const;

    /// \~chinese
    /// @brief 使用渲染命令集模式
    /// @warning 该函数会导致原绘图上下文失效
    void EnableCommandListMode(bool enable);

    void OnRender(RenderContext& ctx) override;

private:
    bool                  cmd_list_mode_;
    RefPtr<Bitmap>        cached_bitmap_;
    RefPtr<RenderContext> render_ctx_;
};

/// \~chinese
/// @brief 画布渲染上下文
class KGE_API CanvasRenderContext : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 开始渲染
    void BeginDraw();

    /// \~chinese
    /// @brief 结束渲染
    void EndDraw();

    /// \~chinese
    /// @brief 画角色
    /// @param actor 角色
    void DrawActor(RefPtr<Actor> actor);

    /// \~chinese
    /// @brief 画形状轮廓
    /// @param shape 形状
    void DrawShape(RefPtr<Shape> shape);

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
    void FillShape(RefPtr<Shape> shape);

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
    /// @brief 绘制位图
    /// @param bitmap 位图
    /// @param pos 绘制的目标位置
    /// @param crop_rect 位图裁剪矩形
    void DrawBitmap(RefPtr<Bitmap> bitmap, const Point& pos, const Rect* crop_rect = nullptr);

    /// \~chinese
    /// @brief 绘制位图
    /// @param bitmap 位图
    /// @param pos 绘制的目标位置
    /// @param size 绘制的目标大小
    /// @param crop_rect 位图裁剪矩形
    void DrawBitmap(RefPtr<Bitmap> bitmap, const Point& pos, const Size& size, const Rect* crop_rect = nullptr);

    /// \~chinese
    /// @brief 绘制文字布局
    /// @param text 文字
    /// @param style 文字样式
    /// @param point 绘制文字的位置
    void DrawTextLayout(StringView text, const TextStyle& style, const Point& point);

    /// \~chinese
    /// @brief 绘制文字布局
    /// @param layout 文字布局
    /// @param point 绘制布局的位置
    void DrawTextLayout(RefPtr<TextLayout> layout, const Point& point);

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
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 获取填充画刷
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief 设置轮廓颜色
    /// @param color 轮廓颜色
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief 设置轮廓画刷
    /// @param[in] brush 轮廓画刷
    void SetStrokeBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 获取轮廓画刷
    RefPtr<Brush> GetStrokeBrush() const;

    /// \~chinese
    /// @brief 设置轮廓样式
    /// @param stroke_style 轮廓样式
    void SetStrokeStyle(RefPtr<StrokeStyle> stroke_style);

    /// \~chinese
    /// @brief 获取轮廓样式
    RefPtr<StrokeStyle> GetStrokeStyle() const;

    /// \~chinese
    /// @brief 添加一个裁剪区域
    /// @param clip_rect 裁剪矩形
    void PushClipRect(const Rect& clip_rect);

    /// \~chinese
    /// @brief 删除最近添加的裁剪区域
    void PopClipRect();

    /// \~chinese
    /// @brief 添加一个图层
    /// @param layer 图层
    void PushLayer(Layer& layer);

    /// \~chinese
    /// @brief 删除最近添加的图层
    void PopLayer();

    /// \~chinese
    /// @brief 获取画布大小
    Size GetSize() const;

    /// \~chinese
    /// @brief 设置渲染上下文的二维变换矩阵
    void SetTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief 设置抗锯齿模式
    void SetAntialiasMode(bool enabled);

    /// \~chinese
    /// @brief 设置文字抗锯齿模式
    void SetTextAntialiasMode(TextAntialiasMode mode);

    virtual ~CanvasRenderContext();

private:
    friend class Canvas;

    CanvasRenderContext(RefPtr<RenderContext> ctx);

private:
    RefPtr<RenderContext> ctx_;
    RefPtr<Brush>         fill_brush_;
    RefPtr<Brush>         stroke_brush_;
    RefPtr<StrokeStyle>   stroke_style_;
};

/** @} */

inline RefPtr<Bitmap> Canvas::GetTarget() const
{
    return cached_bitmap_;
}

inline void CanvasRenderContext::BeginDraw()
{
    KGE_ASSERT(ctx_);
    ctx_->BeginDraw();
}

inline void CanvasRenderContext::EndDraw()
{
    KGE_ASSERT(ctx_);
    ctx_->EndDraw();
}

inline void CanvasRenderContext::DrawActor(RefPtr<Actor> actor)
{
    KGE_ASSERT(ctx_);
    if (actor)
    {
        actor->OnRender(*ctx_);
    }
}

inline void CanvasRenderContext::DrawShape(RefPtr<Shape> shape)
{
    KGE_ASSERT(ctx_);
    if (shape)
    {
        ctx_->SetCurrentBrush(stroke_brush_);
        ctx_->SetCurrentStrokeStyle(stroke_style_);
        ctx_->DrawShape(*shape);
    }
}

inline void CanvasRenderContext::DrawLine(const Point& begin, const Point& end)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->SetCurrentStrokeStyle(stroke_style_);
    ctx_->DrawLine(begin, end);
}

inline void CanvasRenderContext::DrawCircle(const Point& center, float radius)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->SetCurrentStrokeStyle(stroke_style_);
    ctx_->DrawCircle(center, radius);
}

inline void CanvasRenderContext::DrawEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->SetCurrentStrokeStyle(stroke_style_);
    ctx_->DrawEllipse(center, radius);
}

inline void CanvasRenderContext::DrawRect(const Rect& rect)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->SetCurrentStrokeStyle(stroke_style_);
    ctx_->DrawRectangle(rect);
}

inline void CanvasRenderContext::DrawRoundedRect(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(stroke_brush_);
    ctx_->SetCurrentStrokeStyle(stroke_style_);
    ctx_->DrawRoundedRectangle(rect, radius);
}

inline void CanvasRenderContext::FillShape(RefPtr<Shape> shape)
{
    KGE_ASSERT(ctx_);
    if (shape)
    {
        ctx_->SetCurrentBrush(fill_brush_);
        ctx_->FillShape(*shape);
    }
}

inline void CanvasRenderContext::FillCircle(const Point& center, float radius)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillCircle(center, radius);
}

inline void CanvasRenderContext::FillEllipse(const Point& center, const Vec2& radius)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillEllipse(center, radius);
}

inline void CanvasRenderContext::FillRect(const Rect& rect)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillRectangle(rect);
}

inline void CanvasRenderContext::FillRoundedRect(const Rect& rect, const Vec2& radius)
{
    KGE_ASSERT(ctx_);

    ctx_->SetCurrentBrush(fill_brush_);
    ctx_->FillRoundedRectangle(rect, radius);
}

inline void CanvasRenderContext::DrawBitmap(RefPtr<Bitmap> bitmap, const Point& pos, const Rect* crop_rect)
{
    if (bitmap)
    {
        this->DrawBitmap(bitmap, pos, bitmap->GetSize(), crop_rect);
    }
}

inline void CanvasRenderContext::DrawBitmap(RefPtr<Bitmap> bitmap, const Point& pos, const Size& size,
                                             const Rect* crop_rect)
{
    KGE_ASSERT(ctx_);

    if (bitmap)
    {
        Rect dest_rect(pos, pos + size);
        ctx_->DrawBitmap(*bitmap, crop_rect, &dest_rect);
    }
}

inline void CanvasRenderContext::DrawTextLayout(StringView text, const TextStyle& style, const Point& point)
{
    RefPtr<TextLayout> layout = MakePtr<TextLayout>(text, style);
    this->DrawTextLayout(layout, point);
}

inline void CanvasRenderContext::DrawTextLayout(RefPtr<TextLayout> layout, const Point& point)
{
    KGE_ASSERT(ctx_);
    if (layout)
    {
        ctx_->SetCurrentBrush(fill_brush_);
        ctx_->SetCurrentStrokeStyle(stroke_style_);
        ctx_->DrawTextLayout(*layout, point, stroke_brush_);
    }
}

inline void CanvasRenderContext::Clear()
{
    KGE_ASSERT(ctx_);
    ctx_->Clear();
}

inline void CanvasRenderContext::Clear(const Color& clear_color)
{
    KGE_ASSERT(ctx_);
    ctx_->Clear(clear_color);
}

inline void CanvasRenderContext::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ = MakePtr<Brush>();
    }
    fill_brush_->SetColor(color);
}

inline void CanvasRenderContext::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_ = brush;
}

inline RefPtr<Brush> CanvasRenderContext::GetFillBrush() const
{
    return fill_brush_;
}

inline void CanvasRenderContext::SetStrokeBrush(RefPtr<Brush> brush)
{
    stroke_brush_ = brush;
}

inline void CanvasRenderContext::SetStrokeColor(const Color& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ = MakePtr<Brush>();
    }
    stroke_brush_->SetColor(color);
}

inline RefPtr<Brush> CanvasRenderContext::GetStrokeBrush() const
{
    return stroke_brush_;
}

inline void CanvasRenderContext::SetStrokeStyle(RefPtr<StrokeStyle> stroke_style)
{
    stroke_style_ = stroke_style;
}

inline RefPtr<StrokeStyle> CanvasRenderContext::GetStrokeStyle() const
{
    return stroke_style_;
}

inline void CanvasRenderContext::PushClipRect(const Rect& clip_rect)
{
    KGE_ASSERT(ctx_);
    ctx_->PushClipRect(clip_rect);
}

inline void CanvasRenderContext::PopClipRect()
{
    KGE_ASSERT(ctx_);
    ctx_->PopClipRect();
}

inline void CanvasRenderContext::PushLayer(Layer& layer)
{
    KGE_ASSERT(ctx_);
    ctx_->PushLayer(layer);
}

inline void CanvasRenderContext::PopLayer()
{
    KGE_ASSERT(ctx_);
    ctx_->PopLayer();
}

inline Size CanvasRenderContext::GetSize() const
{
    KGE_ASSERT(ctx_);
    return ctx_->GetSize();
}

inline void CanvasRenderContext::SetTransform(const Matrix3x2& matrix)
{
    KGE_ASSERT(ctx_);
    ctx_->SetTransform(matrix);
}

inline void CanvasRenderContext::SetAntialiasMode(bool enabled)
{
    KGE_ASSERT(ctx_);
    ctx_->SetAntialiasMode(enabled);
}

inline void CanvasRenderContext::SetTextAntialiasMode(TextAntialiasMode mode)
{
    KGE_ASSERT(ctx_);
    ctx_->SetTextAntialiasMode(mode);
}

}  // namespace kiwano
