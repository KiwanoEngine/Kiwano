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
 * @brief ��Ⱦ����
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
    /// @brief ����Ƿ�����Ⱦ�����ĵ�������
    virtual bool CheckVisibility(RenderContext& ctx);

    /// \~chinese
    /// @brief ִ����Ⱦ
    /// @details ÿ֡����ˢ��ʱ���øú��������ظú�����ʵ�־�����Ⱦ����
    /// @param ctx ��Ⱦ������
    void OnRender(RenderContext& ctx) override;

    /// \~chinese
    /// @brief ��Ⱦ��ɺ�
    /// @param ctx ��Ⱦ������
    void AfterRender(RenderContext& ctx) override;

    /// \~chinese
    /// @brief ��ȡ����ɼ���
    bool IsVisible() const;

    void OnUpdate(Duration dt) override;

private:
    bool visible_;
};

/**
 * \~chinese
 * @brief ������Ⱦ����
 */
class KGE_API TextureRenderAbility : public RenderAbility
{
public:
    TextureRenderAbility();

    TextureRenderAbility(RefPtr<Texture> texture, const Rect& src_rect = Rect(), const Rect& dest_rect = Rect());

    /// \~chinese
    /// @brief ��ȡ����
    RefPtr<Texture> GetTexture() const;

    /// \~chinese
    /// @brief ��������
    void SetTexture(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief ��ȡԴ����
    const Rect& GetSourceRect() const;

    /// \~chinese
    /// @brief ����Դ����
    /// @param src_rect Դ���Σ���ȡ����ľ�������
    void SetSourceRect(const Rect& src_rect);

    /// \~chinese
    /// @brief ��ȡĿ�����
    const Rect& GetDestRect() const;

    /// \~chinese
    /// @brief ����Ŀ�����
    /// @param dest_rect Ŀ����Σ���������Ⱦ����������
    void SetDestRect(const Rect& dest_rect);

    /// \~chinese
    /// @brief ��������仯ʱ�Ļص�����
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
 * @brief ������Ⱦ����
 */
class KGE_API TextRenderAbility : public RenderAbility
{
    friend class TextActor;

public:
    TextRenderAbility();

    TextRenderAbility(RefPtr<TextLayout> text_layout);

    /// \~chinese
    /// @brief ��ȡ�ı�����
    RefPtr<TextLayout> GetTextLayout() const;

    /// \~chinese
    /// @brief �����ı�����
    void SetTextLayout(RefPtr<TextLayout> layout);

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief ����������仭ˢ
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ��ȡ��߻�ˢ
    RefPtr<Brush> GetOutlineBrush() const;

    /// \~chinese
    /// @brief ����������߻�ˢ
    void SetOutlineBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ��ȡ���������ʽ
    RefPtr<StrokeStyle> GetOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief �������������ʽ
    void SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke);

    /// \~chinese
    /// @brief ����Ԥ��Ⱦģʽ������ߵ�����»��и��õ�����
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
 * @brief ��״��Ⱦ����
 */
class KGE_API ShapeRenderAbility : public RenderAbility
{
public:
    ShapeRenderAbility();

    ShapeRenderAbility(RefPtr<Shape> shape, RefPtr<Brush> fill_brush, RefPtr<Brush> stroke_brush);

    /// \~chinese
    /// @brief ��ȡ��״
    RefPtr<Shape> GetShape() const;

    /// \~chinese
    /// @brief ������״
    void SetShape(RefPtr<Shape> shape);

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief ���������ɫ
    /// @param color �����ɫ
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief ������仭ˢ
    /// @param[in] brush ��仭ˢ
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ��ȡ������ˢ
    RefPtr<Brush> GetStrokeBrush() const;

    /// \~chinese
    /// @brief ����������ɫ
    /// @param color ������ɫ
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief ����������ˢ
    /// @param[in] brush ������ˢ
    void SetStrokeBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ��ȡ������ʽ
    RefPtr<StrokeStyle> GetStrokeStyle() const;

    /// \~chinese
    /// @brief ����������ʽ
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
 * @brief ͼ����Ⱦ����
 */
class KGE_API LayerRenderAbility : public RenderAbility
{
public:
    LayerRenderAbility(const Layer& layer);

    /// \~chinese
    /// @brief ��ȡͼ��
    const Layer& GetLayer() const;

    /// \~chinese
    /// @brief ��ȡͼ��
    Layer& GetLayer();

    /// \~chinese
    /// @brief ����ͼ��
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
