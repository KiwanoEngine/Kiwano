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
#include <kiwano/ability/Ability.h>
#include <kiwano/render/RenderObject.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

class TextActor;

/**
 * \addtogroup Ability
 * @{
 */

/**
 * \~chinese
 * @brief 渲染能力
 */
class KGE_API RenderAbility
    : public Ability
    , public RenderObject
    , protected IntrusiveListValue<RefPtr<RenderAbility>>
{
    friend IntrusiveList<RefPtr<RenderAbility>>;

public:
    RenderAbility();

protected:
    /// \~chinese
    /// @brief 检查是否在渲染上下文的视区内
    virtual bool CheckVisibility(RenderContext& ctx);

    /// \~chinese
    /// @brief 执行渲染
    /// @details 每帧画面刷新时调用该函数，重载该函数以实现具体渲染过程
    /// @param ctx 渲染上下文
    void OnRender(RenderContext& ctx) override;

    /// \~chinese
    /// @brief 渲染完成后
    /// @param ctx 渲染上下文
    void AfterRender(RenderContext& ctx) override;

    /// \~chinese
    /// @brief 获取对象可见性
    bool IsVisible() const;

    void OnUpdate(Duration dt) override;

private:
    bool visible_;
};

/**
 * \~chinese
 * @brief 纹理渲染能力
 */
class KGE_API TextureRenderAbility : public RenderAbility
{
public:
    TextureRenderAbility();

    TextureRenderAbility(RefPtr<Texture> texture, const Rect& src_rect = Rect(), const Rect& dest_rect = Rect());

    /// \~chinese
    /// @brief 获取纹理
    RefPtr<Texture> GetTexture() const;

    /// \~chinese
    /// @brief 设置纹理
    void SetTexture(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief 获取源矩形
    const Rect& GetSourceRect() const;

    /// \~chinese
    /// @brief 设置源矩形
    /// @param src_rect 源矩形，读取纹理的矩形区域
    void SetSourceRect(const Rect& src_rect);

    /// \~chinese
    /// @brief 获取目标矩形
    const Rect& GetDestRect() const;

    /// \~chinese
    /// @brief 设置目标矩形
    /// @param dest_rect 目标矩形，将纹理渲染到矩形区域
    void SetDestRect(const Rect& dest_rect);

    /// \~chinese
    /// @brief 设置纹理变化时的回调函数
    void OnTextureChanged(const Function<void()>& callback);

public:
    void OnRender(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) override;

protected:
    RefPtr<Texture>  texture_;
    Rect             src_rect_;
    Rect             dest_rect_;
    Function<void()> on_texture_changed_;
};

/**
 * \~chinese
 * @brief 文字渲染能力
 */
class KGE_API TextRenderAbility : public RenderAbility
{
    friend class TextActor;

public:
    TextRenderAbility();

    TextRenderAbility(RefPtr<TextLayout> text_layout);

    /// \~chinese
    /// @brief 获取文本布局
    RefPtr<TextLayout> GetTextLayout() const;

    /// \~chinese
    /// @brief 设置文本布局
    void SetTextLayout(RefPtr<TextLayout> layout);

    /// \~chinese
    /// @brief 获取填充画刷
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief 设置文字填充画刷
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 获取描边画刷
    RefPtr<Brush> GetOutlineBrush() const;

    /// \~chinese
    /// @brief 设置文字描边画刷
    void SetOutlineBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 获取描边线条样式
    RefPtr<StrokeStyle> GetOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief 设置描边线条样式
    void SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke);

    /// \~chinese
    /// @brief 设置预渲染模式，在描边等情况下会有更好的性能
    void SetPreRenderEnabled(bool enable);

public:
    void OnRender(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) override;

protected:
    void UpdateCachedTexture();

protected:
    bool                  is_cache_dirty_;
    RefPtr<TextLayout>    layout_;
    RefPtr<Brush>         fill_brush_;
    RefPtr<Brush>         outline_brush_;
    RefPtr<StrokeStyle>   outline_stroke_;
    RefPtr<Texture>       texture_cached_;
    RefPtr<RenderContext> render_ctx_;
};

/**
 * \~chinese
 * @brief 形状渲染能力
 */
class KGE_API ShapeRenderAbility : public RenderAbility
{
public:
    ShapeRenderAbility();

    ShapeRenderAbility(RefPtr<Shape> shape, RefPtr<Brush> fill_brush, RefPtr<Brush> stroke_brush);

    /// \~chinese
    /// @brief 获取形状
    RefPtr<Shape> GetShape() const;

    /// \~chinese
    /// @brief 设置形状
    void SetShape(RefPtr<Shape> shape);

    /// \~chinese
    /// @brief 获取填充画刷
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief 设置填充颜色
    /// @param color 填充颜色
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief 设置填充画刷
    /// @param[in] brush 填充画刷
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 获取轮廓画刷
    RefPtr<Brush> GetStrokeBrush() const;

    /// \~chinese
    /// @brief 设置轮廓颜色
    /// @param color 轮廓颜色
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief 设置轮廓画刷
    /// @param[in] brush 轮廓画刷
    void SetStrokeBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 获取线条样式
    RefPtr<StrokeStyle> GetStrokeStyle() const;

    /// \~chinese
    /// @brief 设置线条样式
    void SetStrokeStyle(RefPtr<StrokeStyle> stroke_style);

public:
    void OnRender(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) override;

protected:
    RefPtr<Brush>       fill_brush_;
    RefPtr<Brush>       stroke_brush_;
    RefPtr<StrokeStyle> stroke_style_;
    RefPtr<Shape>       shape_;
};

/**
 * \~chinese
 * @brief 图层渲染能力
 */
class KGE_API LayerRenderAbility : public RenderAbility
{
public:
    LayerRenderAbility(const Layer& layer);

    /// \~chinese
    /// @brief 获取图层
    const Layer& GetLayer() const;

    /// \~chinese
    /// @brief 获取图层
    Layer& GetLayer();

    /// \~chinese
    /// @brief 设置图层
    void SetLayer(const Layer& layer);

public:
    void OnRender(RenderContext& ctx) override;

    void AfterRender(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) override;

protected:
    Layer layer_;
};

/** @} */

inline bool RenderAbility::IsVisible() const
{
    return visible_;
}

inline RefPtr<Texture> TextureRenderAbility::GetTexture() const
{
    return texture_;
}

inline void TextureRenderAbility::SetTexture(RefPtr<Texture> texture)
{
    texture_ = texture;
}

inline const Rect& TextureRenderAbility::GetSourceRect() const
{
    return src_rect_;
}

inline void TextureRenderAbility::SetSourceRect(const Rect& src_rect)
{
    src_rect_ = src_rect;
}

inline const Rect& TextureRenderAbility::GetDestRect() const
{
    return dest_rect_;
}

inline void TextureRenderAbility::SetDestRect(const Rect& dest_rect)
{
    dest_rect_ = dest_rect;
}

inline RefPtr<TextLayout> TextRenderAbility::GetTextLayout() const
{
    return layout_;
}

inline void TextRenderAbility::SetTextLayout(RefPtr<TextLayout> layout)
{
    layout_         = layout;
    is_cache_dirty_ = true;
}

inline RefPtr<Brush> TextRenderAbility::GetFillBrush() const
{
    return fill_brush_;
}

inline void TextRenderAbility::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_     = brush;
    is_cache_dirty_ = true;
}

inline RefPtr<Brush> TextRenderAbility::GetOutlineBrush() const
{
    return outline_brush_;
}

inline void TextRenderAbility::SetOutlineBrush(RefPtr<Brush> brush)
{
    outline_brush_  = brush;
    is_cache_dirty_ = true;
}

inline RefPtr<StrokeStyle> TextRenderAbility::GetOutlineStrokeStyle() const
{
    return outline_stroke_;
}

inline void TextRenderAbility::SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke)
{
    outline_stroke_ = stroke;
    is_cache_dirty_ = true;
}

inline void ShapeRenderAbility::SetStrokeColor(const Color& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ = MakePtr<Brush>();
    }
    stroke_brush_->SetColor(color);
}

inline void ShapeRenderAbility::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ = MakePtr<Brush>();
    }
    fill_brush_->SetColor(color);
}

inline void ShapeRenderAbility::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_ = brush;
}

inline void ShapeRenderAbility::SetStrokeBrush(RefPtr<Brush> brush)
{
    stroke_brush_ = brush;
}

inline RefPtr<Brush> ShapeRenderAbility::GetFillBrush() const
{
    return fill_brush_;
}

inline RefPtr<Brush> ShapeRenderAbility::GetStrokeBrush() const
{
    return stroke_brush_;
}

inline RefPtr<StrokeStyle> ShapeRenderAbility::GetStrokeStyle() const
{
    return stroke_style_;
}

inline RefPtr<Shape> ShapeRenderAbility::GetShape() const
{
    return shape_;
}

inline void ShapeRenderAbility::SetShape(RefPtr<Shape> shape)
{
    shape_ = shape;
}

inline void ShapeRenderAbility::SetStrokeStyle(RefPtr<StrokeStyle> stroke_style)
{
    stroke_style_ = stroke_style;
}

inline const Layer& LayerRenderAbility::GetLayer() const
{
    return layer_;
}

inline Layer& LayerRenderAbility::GetLayer()
{
    return layer_;
}

inline void LayerRenderAbility::SetLayer(const Layer& layer)
{
    layer_ = layer;
}

}  // namespace kiwano
