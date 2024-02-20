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
#include <kiwano/render/Brush.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/StrokeStyle.h>

namespace kiwano
{

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ��״��ɫ
 */
class KGE_API ShapeActor : public Actor
{
public:
    ShapeActor();

    /// \~chinese
    /// @brief ������״��ɫ
    /// @param shape ��״
    ShapeActor(RefPtr<Shape> shape);

    /// \~chinese
    /// @brief ������״��ɫ
    /// @param shape ��״
    /// @param fill_color �����ɫ
    /// @param stroke_color ������ɫ
    ShapeActor(RefPtr<Shape> shape, const Color& fill_color, const Color& stroke_color);

    /// \~chinese
    /// @brief ������״��ɫ
    /// @param shape ��״
    /// @param fill_brush ��仭ˢ
    /// @param stroke_brush ������ˢ
    ShapeActor(RefPtr<Shape> shape, RefPtr<Brush> fill_brush, RefPtr<Brush> stroke_brush);

    virtual ~ShapeActor();

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡ������ˢ
    RefPtr<Brush> GetStrokeBrush() const;

    /// \~chinese
    /// @brief ��ȡ������ʽ
    RefPtr<StrokeStyle> GetStrokeStyle() const;

    /// \~chinese
    /// @brief ��ȡ��״
    RefPtr<Shape> GetShape() const;

    /// \~chinese
    /// @brief ��ȡ�߽�
    Rect GetBounds() const override;

    /// \~chinese
    /// @brief ��ȡ���а�Χ��
    Rect GetBoundingBox() const override;

    /// \~chinese
    /// @brief �жϵ��Ƿ�����״��
    bool ContainsPoint(const Point& point) const override;

    /// \~chinese
    /// @brief ���������ɫ
    /// @param color �����ɫ
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief ������仭ˢ
    /// @param[in] brush ��仭ˢ
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ����������ɫ
    /// @param color ������ɫ
    void SetStrokeColor(const Color& color);

    /// \~chinese
    /// @brief ����������ˢ
    /// @param[in] brush ������ˢ
    void SetStrokeBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ����������ʽ
    void SetStrokeStyle(RefPtr<StrokeStyle> stroke_style);

    /// \~chinese
    /// @brief ������״
    void SetShape(RefPtr<Shape> shape);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    RefPtr<Brush>       fill_brush_;
    RefPtr<Brush>       stroke_brush_;
    RefPtr<StrokeStyle> stroke_style_;
    Rect           bounds_;
    RefPtr<Shape>       shape_;
};

/// \~chinese
/// @brief �߶ν�ɫ
class KGE_API LineActor : public ShapeActor
{
public:
    LineActor();

    /// \~chinese
    /// @brief �����߶ν�ɫ
    /// @param begin �߶����
    /// @param end �߶��յ�
    LineActor(const Point& begin, const Point& end);

    virtual ~LineActor();

    /// \~chinese
    /// @brief ��ȡ�߶����
    const Point& GetBeginPoint() const;

    /// \~chinese
    /// @brief ��ȡ�߶��յ�
    const Point& GetEndPoint() const;

    /// \~chinese
    /// @brief �����߶����
    /// @param begin �߶����
    void SetBeginPoint(const Point& begin);

    /// \~chinese
    /// @brief �����߶��յ�
    /// @param end �߶��յ�
    void SetEndPoint(const Point& end);

    /// \~chinese
    /// @brief �����߶������յ�
    /// @param begin �߶����
    /// @param end �߶��յ�
    void SetLine(const Point& begin, const Point& end);

private:
    Point begin_;
    Point end_;
};

/// \~chinese
/// @brief ���ν�ɫ
class KGE_API RectActor : public ShapeActor
{
public:
    RectActor();

    /// \~chinese
    /// @brief �������ν�ɫ
    /// @param size ���δ�С
    RectActor(const Size& size);

    virtual ~RectActor();

    /// \~chinese
    /// @brief ��ȡ���δ�С
    const Size& GetRectSize() const;

    /// \~chinese
    /// @brief ���þ��δ�С
    /// @param size ���δ�С
    void SetRectSize(const Size& size);

private:
    Size rect_size_;
};

/// \~chinese
/// @brief Բ�Ǿ��ν�ɫ
class KGE_API RoundedRectActor : public ShapeActor
{
public:
    RoundedRectActor();

    /// \~chinese
    /// @brief ����Բ�Ǿ��ν�ɫ
    /// @param size Բ�Ǿ��δ�С
    /// @param radius Բ�ǰ뾶
    RoundedRectActor(const Size& size, const Vec2& radius);

    virtual ~RoundedRectActor();

    /// \~chinese
    /// @brief ��ȡԲ�ǰ뾶
    Vec2 GetRadius() const;

    /// \~chinese
    /// @brief ��ȡԲ�Ǿ��δ�С
    Size GetRectSize() const;

    /// \~chinese
    /// @brief ����Բ�ǰ뾶
    /// @param radius Բ�ǰ뾶
    void SetRadius(const Vec2& radius);

    /// \~chinese
    /// @brief ����Բ�Ǿ��δ�С
    /// @param size Բ�Ǿ��δ�С
    void SetRectSize(const Size& size);

    /// \~chinese
    /// @brief ����Բ�Ǿ���
    /// @param size Բ�Ǿ��δ�С
    /// @param radius Բ�ǰ뾶
    void SetRoundedRect(const Size& size, const Vec2& radius);

private:
    Size rect_size_;
    Vec2 radius_;
};

/// \~chinese
/// @brief Բ�ν�ɫ
class KGE_API CircleActor : public ShapeActor
{
public:
    CircleActor();

    /// \~chinese
    /// @brief ����Բ�ν�ɫ
    /// @param radius Բ�ΰ뾶
    CircleActor(float radius);

    virtual ~CircleActor();

    /// \~chinese
    /// @brief ��ȡԲ�ΰ뾶
    float GetRadius() const;

    /// \~chinese
    /// @brief ����Բ�ΰ뾶
    /// @param radius Բ�ΰ뾶
    void SetRadius(float radius);

private:
    float radius_;
};

/// \~chinese
/// @brief ��Բ��ɫ
class KGE_API EllipseActor : public ShapeActor
{
public:
    EllipseActor();

    /// \~chinese
    /// @brief ������Բ��ɫ
    /// @param radius ��Բ�뾶
    EllipseActor(const Vec2& radius);

    virtual ~EllipseActor();

    /// \~chinese
    /// @brief ��ȡ��Բ�뾶
    Vec2 GetRadius() const;

    /// \~chinese
    /// @brief ������Բ�뾶
    /// @param radius ��Բ�뾶
    void SetRadius(const Vec2& radius);

private:
    Vec2 radius_;
};

/// \~chinese
/// @brief ����ν�ɫ
class KGE_API PolygonActor : public ShapeActor
{
public:
    PolygonActor();

    /// \~chinese
    /// @brief ��������ν�ɫ
    /// @param vertices ����ζ˵㼯��
    PolygonActor(const Vector<Point>& vertices);

    virtual ~PolygonActor();

    /// \~chinese
    /// @brief ���ö���ζ˵�
    /// @param vertices ����ζ˵㼯��
    void SetVertices(const Vector<Point>& vertices);
};


/** @} */

inline void ShapeActor::SetStrokeColor(const Color& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ =  MakePtr<Brush>();
    }
    stroke_brush_->SetColor(color);
}

inline void ShapeActor::SetFillColor(const Color& color)
{
    if (!fill_brush_)
    {
        fill_brush_ =  MakePtr<Brush>();
    }
    fill_brush_->SetColor(color);
}

inline void ShapeActor::SetFillBrush(RefPtr<Brush> brush)
{
    fill_brush_ = brush;
}

inline void ShapeActor::SetStrokeBrush(RefPtr<Brush> brush)
{
    stroke_brush_ = brush;
}

inline RefPtr<Brush> ShapeActor::GetFillBrush() const
{
    return fill_brush_;
}

inline RefPtr<Brush> ShapeActor::GetStrokeBrush() const
{
    return stroke_brush_;
}

inline RefPtr<StrokeStyle> ShapeActor::GetStrokeStyle() const
{
    return stroke_style_;
}

inline RefPtr<Shape> ShapeActor::GetShape() const
{
    return shape_;
}

inline void ShapeActor::SetStrokeStyle(RefPtr<StrokeStyle> stroke_style)
{
    stroke_style_ = stroke_style;
}

inline const Point& LineActor::GetBeginPoint() const
{
    return begin_;
}

inline const Point& LineActor::GetEndPoint() const
{
    return end_;
}

inline void LineActor::SetBeginPoint(const Point& begin)
{
    SetLine(begin, end_);
}

inline void LineActor::SetEndPoint(const Point& end)
{
    SetLine(begin_, end);
}

inline const Size& RectActor::GetRectSize() const
{
    return rect_size_;
}

inline Vec2 RoundedRectActor::GetRadius() const
{
    return radius_;
}

inline Size RoundedRectActor::GetRectSize() const
{
    return GetSize();
}

inline float CircleActor::GetRadius() const
{
    return radius_;
}

inline Vec2 EllipseActor::GetRadius() const
{
    return radius_;
}

}  // namespace kiwano
