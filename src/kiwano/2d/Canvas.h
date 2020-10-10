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
#include <kiwano/2d/animation/KeyFrame.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Canvas);
KGE_DECLARE_SMART_PTR(CanvasRenderContext);

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
    Canvas();

    /// \~chinese
    /// @brief 创建画布
    /// @param size 画布大小
    Canvas(const Size& size);

    /// \~chinese
    /// @brief 获取2D绘图上下文
    CanvasRenderContextPtr GetContext2D() const;

    /// \~chinese
    /// @brief 清空画布大小并重设画布大小
    /// @warning 该函数会导致原绘图上下文失效
    void ResizeAndClear(Size size);

    /// \~chinese
    /// @brief 导出纹理
    TexturePtr ExportToTexture() const;

    void OnRender(RenderContext& ctx) override;

private:
    void RecreateContext(Size* size);

private:
    TexturePtr       texture_cached_;
    RenderContextPtr render_ctx_;
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
    /// @param pos 绘制的目标位置
    /// @param crop_rect 纹理裁剪矩形
    void DrawTexture(TexturePtr texture, const Point& pos, const Rect* crop_rect = nullptr);

    /// \~chinese
    /// @brief 绘制纹理
    /// @param texture 纹理
    /// @param pos 绘制的目标位置
    /// @param size 绘制的目标大小
    /// @param crop_rect 纹理裁剪矩形
    void DrawTexture(TexturePtr texture, const Point& pos, const Size& size, const Rect* crop_rect = nullptr);

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
    /// @brief 获取填充画刷
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief 设置轮廓颜色
    /// @param color 轮廓颜色
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief 设置轮廓画刷
    /// @param[in] brush 轮廓画刷
    void SetStrokeBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 获取轮廓画刷
    BrushPtr GetStrokeBrush() const;

    /// \~chinese
    /// @brief 设置轮廓样式
    /// @param stroke_style 轮廓样式
    void SetStrokeStyle(StrokeStylePtr stroke_style);

    /// \~chinese
    /// @brief 获取轮廓样式
    StrokeStylePtr GetStrokeStyle() const;

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
    void PushLayer(LayerPtr layer);

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

    CanvasRenderContext(RenderContextPtr ctx);

private:
    RenderContextPtr ctx_;
    BrushPtr         fill_brush_;
    BrushPtr         stroke_brush_;
    StrokeStylePtr   stroke_style_;
};

/** @} */

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

inline void CanvasRenderContext::DrawShape(ShapePtr shape)
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

inline void CanvasRenderContext::FillShape(ShapePtr shape)
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

inline void CanvasRenderContext::DrawTexture(TexturePtr texture, const Point& pos, const Rect* crop_rect)
{
    if (texture)
    {
        this->DrawTexture(texture, pos, texture->GetSize(), crop_rect);
    }
}

inline void CanvasRenderContext::DrawTexture(TexturePtr texture, const Point& pos, const Size& size,
                                             const Rect* crop_rect)
{
    KGE_ASSERT(ctx_);

    if (texture)
    {
        ctx_->DrawTexture(*texture, crop_rect, &Rect(pos, size));
    }
}

inline void CanvasRenderContext::DrawTextLayout(const String& text, const TextStyle& style, const Point& point)
{
    TextLayoutPtr layout = MakePtr<TextLayout>(text, style);
    this->DrawTextLayout(layout, point);
}

inline void CanvasRenderContext::DrawTextLayout(TextLayoutPtr layout, const Point& point)
{
    KGE_ASSERT(ctx_);
    if (layout)
    {
        ctx_->DrawTextLayout(*layout, point);
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

inline void CanvasRenderContext::SetFillBrush(BrushPtr brush)
{
    fill_brush_ = brush;
}

inline BrushPtr CanvasRenderContext::GetFillBrush() const
{
    return fill_brush_;
}

inline void CanvasRenderContext::SetStrokeBrush(BrushPtr brush)
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

inline BrushPtr CanvasRenderContext::GetStrokeBrush() const
{
    return stroke_brush_;
}

inline void CanvasRenderContext::SetStrokeStyle(StrokeStylePtr stroke_style)
{
    stroke_style_ = stroke_style;
}

inline StrokeStylePtr CanvasRenderContext::GetStrokeStyle() const
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

inline void CanvasRenderContext::PushLayer(LayerPtr layer)
{
    KGE_ASSERT(ctx_);
    if (layer)
    {
        ctx_->PushLayer(*layer);
    }
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
