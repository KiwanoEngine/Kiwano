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
#include <kiwano/2d/Frame.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Canvas);

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
    /// \~chinese
    /// @brief ��������
    /// @param size ������С
    static CanvasPtr Create(const Size& size);

    /// \~chinese
    /// @brief ��ʼ��ͼ
    void BeginDraw();

    /// \~chinese
    /// @brief ������ͼ
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
    /// @brief ����ͼ��֡
    /// @param frame ͼ��֡
    /// @param pos ����ͼ���λ��
    void DrawFrame(FramePtr frame, const Point& pos);

    /// \~chinese
    /// @brief ����ͼ��֡
    /// @param frame ͼ��֡
    /// @param pos ����ͼ���λ��
    /// @param size ��Ⱦ��ͼ���С
    void DrawFrame(FramePtr frame, const Point& pos, const Size& size);

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
    /// @brief ��ʼ����·��
    /// @param begin_pos ·����ʼ��
    void BeginPath(const Point& begin_pos);

    /// \~chinese
    /// @brief ����·��
    /// @param closed ·���Ƿ�պ�
    void EndPath(bool closed = false);

    /// \~chinese
    /// @brief ���һ���߶�
    /// @param point �˵�
    void AddLine(const Point& point);

    /// \~chinese
    /// @brief ��Ӷ����߶�
    /// @param points �˵㼯��
    void AddLines(const Vector<Point>& points);

    /// \~chinese
    /// @brief ���һ�����η�����������
    /// @param point1 ���������ߵĵ�һ�����Ƶ�
    /// @param point2 ���������ߵĵڶ������Ƶ�
    /// @param point3 ���������ߵ��յ�
    void AddBezier(const Point& point1, const Point& point2, const Point& point3);

    /// \~chinese
    /// @brief ��ӻ���
    /// @param point �յ�
    /// @param radius ��Բ�뾶
    /// @param rotation ��Բ��ת�Ƕ�
    /// @param clockwise ˳ʱ�� or ��ʱ��
    /// @param is_small �Ƿ�ȡС�� 180�� �Ļ�
    void AddArc(const Point& point, const Size& radius, float rotation, bool clockwise = true, bool is_small = true);

    /// \~chinese
    /// @brief ����ߵķ�ʽ����·��
    void StrokePath();

    /// \~chinese
    /// @brief �����ķ�ʽ����·��
    void FillPath();

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
    /// @brief ����������ɫ
    /// @param color ������ɫ
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief ����������ˢ
    /// @param[in] brush ������ˢ
    void SetStrokeBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param stroke_style ������ʽ
    void SetStrokeStyle(StrokeStylePtr stroke_style);

    /// \~chinese
    /// @brief ���û�ˢ
    /// @param[in] brush ��ˢ
    void SetBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ���û�ˢ��ά�任
    /// @param transform ��ά�任
    void SetBrushTransform(const Transform& transform);

    /// \~chinese
    /// @brief ���û�ˢ��ά�任����
    /// @param transform ��ά�任����
    void SetBrushTransform(const Matrix3x2& transform);

    /// \~chinese
    /// @brief ���һ��ͼ��
    /// @param layer ͼ��
    void PushLayer(LayerPtr layer);

    /// \~chinese
    /// @brief ɾ�������ӵ�ͼ��
    void PopLayer();

    /// \~chinese
    /// @brief ���һ���ü�����
    /// @param clip_rect �ü�����
    void PushClipRect(const Rect& clip_rect);

    /// \~chinese
    /// @brief ɾ�������ӵĲü�����
    void PopClipRect();

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡ������ˢ
    BrushPtr GetStrokeBrush() const;

    /// \~chinese
    /// @brief ��ջ�����С�����軭����С
    void ResizeAndClear(Size size);

    /// \~chinese
    /// @brief ��������
    TexturePtr ExportToTexture() const;

    void OnRender(RenderContext& ctx) override;

private:
    Canvas();

private:
    StrokeStylePtr   stroke_style_;
    ShapeMaker       shape_maker_;
    BrushPtr         fill_brush_;
    BrushPtr         stroke_brush_;
    TexturePtr       texture_cached_;
    RenderContextPtr render_ctx_;
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
        stroke_brush_ = memory::New<Brush>();
    }
    stroke_brush_->SetColor(color);
}

inline void Canvas::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ = memory::New<Brush>();
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
