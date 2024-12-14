// Copyright (c) 2016-2019 Kiwano - Nomango
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
#include <kiwano/core/Time.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/Layer.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief 文字抗锯齿模式
enum class TextAntialiasMode
{
    Default,    ///< 系统默认
    ClearType,  ///< ClearType 抗锯齿
    GrayScale,  ///< 灰度抗锯齿
    None        ///< 不启用抗锯齿
};

/// \~chinese
/// @brief 混合模式
enum class BlendMode
{
    SourceOver = 0,
    Copy       = 1,
    Min        = 2,
    Add        = 3,
    Max        = 4,
};

/// \~chinese
/// @brief 渲染上下文
/// @details 渲染上下文将完成基础图元的绘制，并将绘制结果输出到特定的平面中
class KGE_API RenderContext : public NativeObject
{
public:
    /// \~chinese
    /// @brief 创建纹理渲染上下文，将绘制结果输出到纹理中
    /// @param texture 保存绘制结果的纹理
    /// @param size 渲染输出大小
    static RefPtr<RenderContext> Create(RefPtr<Texture> texture, const PixelSize& size);

    /// \~chinese
    /// @brief 开始渲染
    virtual void BeginDraw();

    /// \~chinese
    /// @brief 结束渲染
    virtual void EndDraw();

    /// \~chinese
    /// @brief 创建空纹理
    /// @param[out] texture 输出纹理
    /// @param[in] size 纹理像素大小
    virtual void CreateTexture(Texture& texture, const PixelSize& size) = 0;

    /// \~chinese
    /// @brief 绘制纹理
    /// @param texture 纹理
    /// @param src_rect 源纹理裁剪矩形
    /// @param dest_rect 绘制的目标区域
    virtual void DrawTexture(const Texture& texture, const Rect* src_rect = nullptr,
                             const Rect* dest_rect = nullptr) = 0;

    /// \~chinese
    /// @brief 绘制文本布局
    /// @param layout 文本布局
    /// @param offset 偏移量
    /// @param outline_brush 描边画刷
    virtual void DrawTextLayout(const TextLayout& layout, const Point& offset, RefPtr<Brush> outline_brush) = 0;

    /// \~chinese
    /// @brief 绘制形状轮廓
    /// @param shape 形状
    virtual void DrawShape(const Shape& shape) = 0;

    /// \~chinese
    /// @brief 绘制线段
    /// @param point1 线段起点
    /// @param point2 线段终点
    virtual void DrawLine(const Point& point1, const Point& point2) = 0;

    /// \~chinese
    /// @brief 绘制矩形边框
    /// @param rect 矩形
    virtual void DrawRectangle(const Rect& rect) = 0;

    /// \~chinese
    /// @brief 绘制圆角矩形边框
    /// @param rect 矩形
    /// @param radius 圆角半径
    virtual void DrawRoundedRectangle(const Rect& rect, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief 绘制圆形边框
    /// @param center 圆心
    /// @param radius 椭圆半径
    virtual void DrawCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief 绘制椭圆边框
    /// @param center 圆心
    /// @param radius 椭圆半径
    virtual void DrawEllipse(const Point& center, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief 填充形状
    /// @param shape 形状
    virtual void FillShape(const Shape& shape) = 0;

    /// \~chinese
    /// @brief 填充矩形
    /// @param rect 矩形
    virtual void FillRectangle(const Rect& rect) = 0;

    /// \~chinese
    /// @brief 填充圆角矩形
    /// @param rect 矩形
    /// @param radius 圆角半径
    virtual void FillRoundedRectangle(const Rect& rect, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief 填充圆形
    /// @param center 圆心
    /// @param radius 椭圆半径
    virtual void FillCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief 填充椭圆
    /// @param center 圆心
    /// @param radius 椭圆半径
    virtual void FillEllipse(const Point& center, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief 设置绘制的裁剪区域
    /// @param clip_rect 裁剪矩形
    virtual void PushClipRect(const Rect& clip_rect) = 0;

    /// \~chinese
    /// @brief 取消上一次设置的绘制裁剪区域
    virtual void PopClipRect() = 0;

    /// \~chinese
    /// @brief 设置图层区域
    /// @param layer 图层
    virtual void PushLayer(Layer& layer) = 0;

    /// \~chinese
    /// @brief 取消上一次设置的图层区域
    virtual void PopLayer() = 0;

    /// \~chinese
    /// @brief 清空渲染内容
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief 使用纯色清空渲染内容
    /// @param clear_color 清屏颜色
    virtual void Clear(const Color& clear_color) = 0;

    /// \~chinese
    /// @brief 获取渲染区域大小
    virtual Size GetSize() const = 0;

    /// \~chinese
    /// @brief 获取画刷透明度
    virtual float GetBrushOpacity() const;

    /// \~chinese
    /// @brief 获取当前画刷
    virtual RefPtr<Brush> GetCurrentBrush() const;

    /// \~chinese
    /// @brief 获取全局二维变换
    virtual const Matrix3x2& GetGlobalTransform() const;

    /// \~chinese
    /// @brief 设置画刷透明度
    virtual void SetBrushOpacity(float opacity);

    /// \~chinese
    /// @brief 设置当前使用的画刷
    virtual void SetCurrentBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 设置当前使用的线条样式
    virtual void SetCurrentStrokeStyle(RefPtr<StrokeStyle> stroke);

    /// \~chinese
    /// @brief 设置混合模式
    virtual void SetBlendMode(BlendMode blend) = 0;

    /// \~chinese
    /// @brief 设置抗锯齿模式
    virtual void SetAntialiasMode(bool enabled) = 0;

    /// \~chinese
    /// @brief 设置文字抗锯齿模式
    virtual void SetTextAntialiasMode(TextAntialiasMode mode) = 0;

    /// \~chinese
    /// @brief 检查边界是否在视区内
    virtual bool CheckVisibility(const Rect& bounds, const Matrix3x2& transform) = 0;

    /// \~chinese
    /// @brief 重设渲染上下文大小
    virtual void Resize(const Size& size) = 0;

    /// \~chinese
    /// @brief 设置上下文的二维变换
    virtual void SetTransform(const Matrix3x2& matrix) = 0;

    /// \~chinese
    /// @brief 设置全局二维变换
    virtual void SetGlobalTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief 设置全局二维变换
    virtual void SetGlobalTransform(const Matrix3x2* matrix);

    /// \~chinese
    /// @brief 获取渲染目标
    virtual RefPtr<Texture> GetTarget() const = 0;

public:
    /// \~chinese
    /// @brief 渲染上下文状态
    struct Status
    {
        uint32_t primitives;  ///< 渲染图元数量
        Time     start;       ///< 渲染起始时间
        Duration duration;    ///< 渲染时长

        Status();
    };

    /// \~chinese
    /// @brief 启用或禁用状态收集功能
    void SetCollectingStatus(bool enable);

    /// \~chinese
    /// @brief 获取渲染上下文状态
    const Status& GetStatus() const;

protected:
    RenderContext();

    /// \~chinese
    /// @brief 增加渲染图元数量
    void IncreasePrimitivesCount(uint32_t increase = 1) const;

protected:
    bool                antialias_;
    bool                fast_global_transform_;
    mutable bool        collecting_status_;
    float               brush_opacity_;
    TextAntialiasMode   text_antialias_;
    RefPtr<Brush>       current_brush_;
    RefPtr<StrokeStyle> current_stroke_;
    Rect                visible_size_;
    Matrix3x2           global_transform_;
    mutable Status      status_;
};

/** @} */

inline RenderContext::Status::Status()
    : primitives(0)
{
}

inline const RenderContext::Status& RenderContext::GetStatus() const
{
    return status_;
}

}  // namespace kiwano
