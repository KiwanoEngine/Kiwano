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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Time.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Geometry.h>
#include <kiwano/render/LayerArea.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/dx/TextRenderer.h>

namespace kiwano
{
class Renderer;

KGE_DECLARE_SMART_PTR(RenderContext);
KGE_DECLARE_SMART_PTR(TextureRenderContext);

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
/// @brief 渲染上下文
/// @details
/// 渲染上下文将完成基础图元的绘制，并将绘制结果输出到特定的目标中（如窗口或纹理）
class KGE_API RenderContext : public virtual ObjectBase
{
    friend class Renderer;

public:
    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 是否有效
    void BeginDraw();

    /// \~chinese
    /// @brief 是否有效
    void EndDraw();

    /// \~chinese
    /// @brief 是否有效
    void DrawGeometry(Geometry const& geometry, float stroke_width, const StrokeStyle& stroke = StrokeStyle());

    /// \~chinese
    /// @brief 是否有效
    void FillGeometry(Geometry const& geometry);

    /// \~chinese
    /// @brief 是否有效
    void DrawLine(Point const& point1, Point const& point2, float stroke_width,
                  const StrokeStyle& stroke = StrokeStyle());

    /// \~chinese
    /// @brief 是否有效
    void DrawRectangle(Rect const& rect, float stroke_width, const StrokeStyle& stroke = StrokeStyle());

    /// \~chinese
    /// @brief 是否有效
    void FillRectangle(Rect const& rect);

    /// \~chinese
    /// @brief 是否有效
    void DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, float stroke_width,
                              const StrokeStyle& stroke = StrokeStyle());

    /// \~chinese
    /// @brief 是否有效
    void FillRoundedRectangle(Rect const& rect, Vec2 const& radius);

    /// \~chinese
    /// @brief 是否有效
    void DrawEllipse(Point const& center, Vec2 const& radius, float stroke_width,
                     const StrokeStyle& stroke = StrokeStyle());

    /// \~chinese
    /// @brief 是否有效
    void FillEllipse(Point const& center, Vec2 const& radius);

    /// \~chinese
    /// @brief 是否有效
    void DrawTexture(Texture const& texture, Rect const& src_rect, Rect const& dest_rect);

    /// \~chinese
    /// @brief 是否有效
    void DrawTexture(Texture const& texture, const Rect* src_rect = nullptr, const Rect* dest_rect = nullptr);

    /// \~chinese
    /// @brief 是否有效
    void DrawTextLayout(TextLayout const& layout, Point const& offset = Point{});

    /// \~chinese
    /// @brief 是否有效
    void CreateTexture(Texture& texture, math::Vec2T<uint32_t> size, D2D1_PIXEL_FORMAT format);

    /// \~chinese
    /// @brief 是否有效
    void PushClipRect(Rect const& clip_rect);

    /// \~chinese
    /// @brief 是否有效
    void PopClipRect();

    /// \~chinese
    /// @brief 是否有效
    void PushLayer(LayerArea& layer);

    /// \~chinese
    /// @brief 是否有效
    void PopLayer();

    /// \~chinese
    /// @brief 是否有效
    void Clear();

    /// \~chinese
    /// @brief 是否有效
    void Clear(Color const& clear_color);

    /// \~chinese
    /// @brief 是否有效
    float GetBrushOpacity() const;

    /// \~chinese
    /// @brief 是否有效
    BrushPtr GetCurrentBrush() const;

    /// \~chinese
    /// @brief 是否有效
    Matrix3x2 GetGlobalTransform() const;

    /// \~chinese
    /// @brief 是否有效
    void SetBrushOpacity(float opacity);

    /// \~chinese
    /// @brief 是否有效
    void SetCurrentBrush(BrushPtr brush);

    /// \~chinese
    /// @brief 是否有效
    void SetTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief 是否有效
    void SetGlobalTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief 是否有效
    void SetGlobalTransform(const Matrix3x2* matrix);

    /// \~chinese
    /// @brief 设置抗锯齿模式
    void SetAntialiasMode(bool enabled);

    /// \~chinese
    /// @brief 设置文字抗锯齿模式
    void SetTextAntialiasMode(TextAntialiasMode mode);

    /// \~chinese
    /// @brief 检查边界是否在视区内
    bool CheckVisibility(Rect const& bounds, Matrix3x2 const& transform);

    /// \~chinese
    /// @brief 重设渲染上下文大小
    void Resize(Size const& size);

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
    Status const& GetStatus() const;

protected:
    RenderContext();

    ComPtr<ID2D1RenderTarget> GetRenderTarget() const;

    ComPtr<ITextRenderer> GetTextRenderer() const;

private:
    /// \~chinese
    /// @brief 创建设备依赖资源
    HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> ctx);

    /// \~chinese
    /// @brief 销毁设备依赖资源
    void DiscardDeviceResources();

    /// \~chinese
    /// @brief 增加渲染图元数量
    void IncreasePrimitivesCount(uint32_t increase = 1) const;

private:
    bool                      antialias_;
    bool                      fast_global_transform_;
    float                     brush_opacity_;
    TextAntialiasMode         text_antialias_;
    ComPtr<ITextRenderer>     text_renderer_;
    ComPtr<ID2D1RenderTarget> render_ctx_;
    BrushPtr                  current_brush_;
    Rect                      visible_size_;
    Matrix3x2                 global_transform_;

    mutable bool   collecting_status_;
    mutable Status status_;
};

/// \~chinese
/// @brief 纹理渲染上下文
/// @details 纹理渲染上下文将渲染输出到一个纹理对象中
class KGE_API TextureRenderContext : public RenderContext
{
    friend class Renderer;

public:
    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 获取渲染输出
    /// @param[out] texture 纹理输出
    /// @return 操作是否成功
    bool GetOutput(Texture& texture);

private:
    TextureRenderContext();

    ComPtr<ID2D1BitmapRenderTarget> GetBitmapRenderTarget() const;

    void SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> ctx);

private:
    ComPtr<ID2D1BitmapRenderTarget> bitmap_rt_;
};

/** @} */

inline RenderContext::Status::Status()
    : primitives(0)
{
}

inline RenderContext::Status const& RenderContext::GetStatus() const
{
    return status_;
}

inline ComPtr<ID2D1RenderTarget> RenderContext::GetRenderTarget() const
{
    KGE_ASSERT(render_ctx_);
    return render_ctx_;
}

inline ComPtr<ITextRenderer> RenderContext::GetTextRenderer() const
{
    KGE_ASSERT(text_renderer_);
    return text_renderer_;
}

inline float RenderContext::GetBrushOpacity() const
{
    return brush_opacity_;
}

inline BrushPtr RenderContext::GetCurrentBrush() const
{
    return current_brush_;
}

inline Matrix3x2 RenderContext::GetGlobalTransform() const
{
    return global_transform_;
}

inline void RenderContext::SetBrushOpacity(float opacity)
{
    brush_opacity_ = opacity;
}

inline void RenderContext::SetGlobalTransform(const Matrix3x2& matrix)
{
    SetGlobalTransform(&matrix);
}

inline void RenderContext::SetCurrentBrush(BrushPtr brush)
{
    current_brush_ = brush;
    if (current_brush_)
    {
        current_brush_->SetOpacity(brush_opacity_);
    }
}

inline bool TextureRenderContext::IsValid() const
{
    return bitmap_rt_ != nullptr;
}

inline ComPtr<ID2D1BitmapRenderTarget> TextureRenderContext::GetBitmapRenderTarget() const
{
    return bitmap_rt_;
}

inline void TextureRenderContext::SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> ctx)
{
    bitmap_rt_ = ctx;
}
}  // namespace kiwano
