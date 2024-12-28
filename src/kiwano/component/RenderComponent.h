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
#include <kiwano/component/Component.h>
#include <kiwano/render/RenderObject.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

class TextActor;

/**
 * \addtogroup Component
 * @{
 */

/**
 * \~chinese
 * @brief ��Ⱦ���
 */
class KGE_API RenderComponent
    : public Component
    , public RenderObject
    , protected IntrusiveListValue<RefPtr<RenderComponent>>
{
    friend IntrusiveList<RefPtr<RenderComponent>>;

public:
    RenderComponent();

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
 * @brief ������Ⱦ���
 */
class KGE_API TextureRenderComponent : public RenderComponent
{
public:
    TextureRenderComponent();

    TextureRenderComponent(RefPtr<Texture> texture, const Rect& src_rect = Rect());

    /// \~chinese
    /// @brief ��ȡ����
    RefPtr<Texture> GetTexture() const;

    /// \~chinese
    /// @brief ��������
    void SetTexture(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief ��ȡԴ����
    Rect GetSourceRect() const;

    /// \~chinese
    /// @brief ����Դ����
    /// @param src_rect �ü�����
    void SetSourceRect(const Rect& src_rect);

public:
    void OnRender(RenderContext& ctx) override;

    bool CheckVisibility(RenderContext& ctx) override;

protected:
    RefPtr<Texture> texture_;
    Rect            src_rect_;
};

/**
 * \~chinese
 * @brief ������Ⱦ���
 */
class KGE_API TextRenderComponent : public RenderComponent
{
    friend class TextActor;

public:
    TextRenderComponent();

    TextRenderComponent(RefPtr<TextLayout> text_layout);

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
 * @brief ��״��Ⱦ���
 */
class KGE_API ShapeRenderComponent : public RenderComponent
{
public:
    ShapeRenderComponent();

    ShapeRenderComponent(RefPtr<Shape> shape, RefPtr<Brush> fill_brush, RefPtr<Brush> stroke_brush);

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

/** @} */

inline bool RenderComponent::IsVisible() const
{
    return visible_;
}

inline RefPtr<Texture> TextureRenderComponent::GetTexture() const
{
    return texture_;
}

inline void TextureRenderComponent::SetTexture(RefPtr<Texture> texture)
{
    texture_ = texture;
}

inline Rect TextureRenderComponent::GetSourceRect() const
{
    return src_rect_;
}

inline void TextureRenderComponent::SetSourceRect(const Rect& src_rect)
{
    src_rect_ = src_rect;
}

inline RefPtr<TextLayout> TextRenderComponent::GetTextLayout() const
{
    return layout_;
}

inline void TextRenderComponent::SetTextLayout(RefPtr<TextLayout> layout)
{
    layout_         = layout;
    is_cache_dirty_ = true;
}

inline RefPtr<Brush> TextRenderComponent::GetFillBrush() const
{
    return fill_brush_;
}

inline void TextRenderComponent::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_     = brush;
    is_cache_dirty_ = true;
}

inline RefPtr<Brush> TextRenderComponent::GetOutlineBrush() const
{
    return outline_brush_;
}

inline void TextRenderComponent::SetOutlineBrush(RefPtr<Brush> brush)
{
    outline_brush_  = brush;
    is_cache_dirty_ = true;
}

inline RefPtr<StrokeStyle> TextRenderComponent::GetOutlineStrokeStyle() const
{
    return outline_stroke_;
}

inline void TextRenderComponent::SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke)
{
    outline_stroke_ = stroke;
    is_cache_dirty_ = true;
}

inline void ShapeRenderComponent::SetStrokeColor(const Color& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ = MakePtr<Brush>();
    }
    stroke_brush_->SetColor(color);
}

inline void ShapeRenderComponent::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ = MakePtr<Brush>();
    }
    fill_brush_->SetColor(color);
}

inline void ShapeRenderComponent::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_ = brush;
}

inline void ShapeRenderComponent::SetStrokeBrush(RefPtr<Brush> brush)
{
    stroke_brush_ = brush;
}

inline RefPtr<Brush> ShapeRenderComponent::GetFillBrush() const
{
    return fill_brush_;
}

inline RefPtr<Brush> ShapeRenderComponent::GetStrokeBrush() const
{
    return stroke_brush_;
}

inline RefPtr<StrokeStyle> ShapeRenderComponent::GetStrokeStyle() const
{
    return stroke_style_;
}

inline RefPtr<Shape> ShapeRenderComponent::GetShape() const
{
    return shape_;
}

inline void ShapeRenderComponent::SetShape(RefPtr<Shape> shape)
{
    shape_ = shape;
}

inline void ShapeRenderComponent::SetStrokeStyle(RefPtr<StrokeStyle> stroke_style)
{
    stroke_style_ = stroke_style;
}

}  // namespace kiwano
