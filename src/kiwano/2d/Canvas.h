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
 * @brief ����
 * @details ���ڻ���ͼ�Ρ�ͼ�����ֵȸ������͵�ͼԪ��ͬʱ���Խ��������ݵ�����ͼ��
 */
class KGE_API Canvas : public Actor
{
public:
    Canvas();

    /// \~chinese
    /// @brief ��������
    /// @param size ������С
    Canvas(const Size& size);

    /// \~chinese
    /// @brief ��ȡ2D��ͼ������
    CanvasRenderContextPtr GetContext2D() const;

    /// \~chinese
    /// @brief ��ջ�����С�����軭����С
    /// @warning �ú����ᵼ��ԭ��ͼ������ʧЧ
    void ResizeAndClear(Size size);

    /// \~chinese
    /// @brief ��������
    TexturePtr ExportToTexture() const;

    void OnRender(RenderContext& ctx) override;

private:
    void RecreateContext(Size* size);

private:
    TexturePtr       texture_cached_;
    RenderContextPtr render_ctx_;
};


/// \~chinese
/// @brief ������Ⱦ������
class KGE_API CanvasRenderContext : public ObjectBase
{
public:
    /// \~chinese
    /// @brief ��ʼ��Ⱦ
    void BeginDraw();

    /// \~chinese
    /// @brief ������Ⱦ
    void EndDraw();

    /// \~chinese
    /// @brief ����״����
    /// @param shape ��״
    void DrawShape(ShapePtr shape);

    /// \~chinese
    /// @brief ���߶�
    /// @param begin �߶����
    /// @param end �߶��յ�
    void DrawLine(const Point& begin, const Point& end);

    /// \~chinese
    /// @brief ��Բ�α߿�
    /// @param center Բ��ԭ��
    /// @param radius Բ�ΰ뾶
    void DrawCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief ����Բ�α߿�
    /// @param center ��Բԭ��
    /// @param radius ��Բ�뾶
    void DrawEllipse(const Point& center, const Vec2& radius);

    /// \~chinese
    /// @brief �����α߿�
    /// @param rect ����
    void DrawRect(const Rect& rect);

    /// \~chinese
    /// @brief ��Բ�Ǿ��α߿�
    /// @param rect ����
    /// @param radius ����Բ�ǰ뾶
    void DrawRoundedRect(const Rect& rect, const Vec2& radius);

    /// \~chinese
    /// @brief �����״
    /// @param shape ��״
    void FillShape(ShapePtr shape);

    /// \~chinese
    /// @brief ���Բ��
    /// @param center Բ��ԭ��
    /// @param radius Բ�ΰ뾶
    void FillCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief �����Բ��
    /// @param center ��Բԭ��
    /// @param radius ��Բ�뾶
    void FillEllipse(const Point& center, const Vec2& radius);

    /// \~chinese
    /// @brief ������
    /// @param rect ����
    void FillRect(const Rect& rect);

    /// \~chinese
    /// @brief ���Բ�Ǿ���
    /// @param rect ����
    /// @param radius ����Բ�ǰ뾶
    void FillRoundedRect(const Rect& rect, const Vec2& radius);

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param pos ���Ƶ�Ŀ��λ��
    /// @param crop_rect ����ü�����
    void DrawTexture(TexturePtr texture, const Point& pos, const Rect* crop_rect = nullptr);

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param pos ���Ƶ�Ŀ��λ��
    /// @param size ���Ƶ�Ŀ���С
    /// @param crop_rect ����ü�����
    void DrawTexture(TexturePtr texture, const Point& pos, const Size& size, const Rect* crop_rect = nullptr);

    /// \~chinese
    /// @brief �������ֲ���
    /// @param text ����
    /// @param style ������ʽ
    /// @param point �������ֵ�λ��
    void DrawTextLayout(const String& text, const TextStyle& style, const Point& point);

    /// \~chinese
    /// @brief �������ֲ���
    /// @param layout ���ֲ���
    /// @param point ���Ʋ��ֵ�λ��
    void DrawTextLayout(TextLayoutPtr layout, const Point& point);

    /// \~chinese
    /// @brief ��ջ���
    void Clear();

    /// \~chinese
    /// @brief ��ջ���
    /// @param clear_color �����ɫ
    void Clear(const Color& clear_color);

    /// \~chinese
    /// @brief ���������ɫ
    /// @param color �����ɫ
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief ������仭ˢ
    /// @param[in] brush ��仭ˢ
    void SetFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief ����������ɫ
    /// @param color ������ɫ
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief ����������ˢ
    /// @param[in] brush ������ˢ
    void SetStrokeBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ��ȡ������ˢ
    BrushPtr GetStrokeBrush() const;

    /// \~chinese
    /// @brief ����������ʽ
    /// @param stroke_style ������ʽ
    void SetStrokeStyle(StrokeStylePtr stroke_style);

    /// \~chinese
    /// @brief ��ȡ������ʽ
    StrokeStylePtr GetStrokeStyle() const;

    /// \~chinese
    /// @brief ���һ���ü�����
    /// @param clip_rect �ü�����
    void PushClipRect(const Rect& clip_rect);

    /// \~chinese
    /// @brief ɾ�������ӵĲü�����
    void PopClipRect();

    /// \~chinese
    /// @brief ���һ��ͼ��
    /// @param layer ͼ��
    void PushLayer(LayerPtr layer);

    /// \~chinese
    /// @brief ɾ�������ӵ�ͼ��
    void PopLayer();

    /// \~chinese
    /// @brief ��ȡ������С
    Size GetSize() const;

    /// \~chinese
    /// @brief ������Ⱦ�����ĵĶ�ά�任����
    void SetTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief ���ÿ����ģʽ
    void SetAntialiasMode(bool enabled);

    /// \~chinese
    /// @brief �������ֿ����ģʽ
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
