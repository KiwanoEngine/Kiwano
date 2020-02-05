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
#include <kiwano/render/ShapeSink.h>
#include <kiwano/render/StrokeStyle.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ShapeActor);
KGE_DECLARE_SMART_PTR(LineActor);
KGE_DECLARE_SMART_PTR(RectActor);
KGE_DECLARE_SMART_PTR(RoundRectActor);
KGE_DECLARE_SMART_PTR(CircleActor);
KGE_DECLARE_SMART_PTR(EllipseActor);
KGE_DECLARE_SMART_PTR(PolygonActor);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ��ά��״��ɫ
 */
class KGE_API ShapeActor : public Actor
{
public:
    /// \~chinese
    /// @brief �����ά��״��ɫ
    ShapeActor();

    virtual ~ShapeActor();

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡ������ˢ
    BrushPtr GetStrokeBrush() const;

    /// \~chinese
    /// @brief ��ȡ�������
    float GetStrokeWidth() const;

    /// \~chinese
    /// @brief ��ȡ������ʽ
    const StrokeStyle& GetStrokeStyle() const;

    /// \~chinese
    /// @brief ��ȡ��״
    ShapePtr GetShape() const;

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
    void SetFillColor(Color const& color);

    /// \~chinese
    /// @brief ������仭ˢ
    /// @param[in] brush ��仭ˢ
    void SetFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ����������ɫ
    /// @param color ������ɫ
    void SetStrokeColor(Color const& color);

    /// \~chinese
    /// @brief ����������ˢ
    /// @param[in] brush ������ˢ
    void SetStrokeBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ����������ȣ�Ĭ��Ϊ 1.0
    void SetStrokeWidth(float width);

    /// \~chinese
    /// @brief ����������ʽ
    void SetStrokeStyle(const StrokeStyle& stroke_style);

    /// \~chinese
    /// @brief ���ü�����״
    void SetShape(ShapePtr shape);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    BrushPtr    fill_brush_;
    BrushPtr    stroke_brush_;
    float       stroke_width_;
    StrokeStyle stroke_style_;
    Rect        bounds_;
    ShapePtr    shape_;
};

/// \~chinese
/// @brief �߶�ͼ�ν�ɫ
class KGE_API LineActor : public ShapeActor
{
public:
    LineActor();

    virtual ~LineActor();

    /// \~chinese
    /// @brief ��ȡ�߶����
    Point const& GetBeginPoint() const;

    /// \~chinese
    /// @brief ��ȡ�߶��յ�
    Point const& GetEndPoint() const;

    /// \~chinese
    /// @brief �����߶����
    /// @param begin �߶����
    void SetBeginPoint(Point const& begin);

    /// \~chinese
    /// @brief �����߶��յ�
    /// @param end �߶��յ�
    void SetEndPoint(Point const& end);

    /// \~chinese
    /// @brief ���þ��δ�С
    /// @param begin �߶����
    /// @param end �߶��յ�
    void SetLine(Point const& begin, Point const& end);

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

    virtual ~RectActor();

    /// \~chinese
    /// @brief ��ȡ���δ�С
    Size const& GetRectSize() const;

    /// \~chinese
    /// @brief ���þ��δ�С
    /// @param size ���δ�С
    void SetRectSize(Size const& size);

private:
    Size rect_size_;
};

/// \~chinese
/// @brief Բ�Ǿ��ν�ɫ
class KGE_API RoundRectActor : public ShapeActor
{
public:
    RoundRectActor();

    virtual ~RoundRectActor();

    /// \~chinese
    /// @brief ��ȡԲ�ǰ뾶
    Vec2 GetRadius() const;

    /// \~chinese
    /// @brief ��ȡԲ�Ǿ��δ�С
    Size GetRectSize() const;

    /// \~chinese
    /// @brief ����Բ�ǰ뾶
    /// @param radius Բ�ǰ뾶
    void SetRadius(Vec2 const& radius);

    /// \~chinese
    /// @brief ����Բ�Ǿ��δ�С
    /// @param size Բ�Ǿ��δ�С
    void SetRectSize(Size const& size);

    /// \~chinese
    /// @brief ����Բ�Ǿ���
    /// @param size Բ�Ǿ��δ�С
    /// @param radius Բ�ǰ뾶
    void SetRoundedRect(Size const& size, Vec2 const& radius);

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

    virtual ~EllipseActor();

    /// \~chinese
    /// @brief ��ȡ��Բ�뾶
    Vec2 GetRadius() const;

    /// \~chinese
    /// @brief ������Բ�뾶
    /// @param radius ��Բ�뾶
    void SetRadius(Vec2 const& radius);

private:
    Vec2 radius_;
};

/// \~chinese
/// @brief ����ν�ɫ
class KGE_API PolygonActor : public ShapeActor
{
public:
    PolygonActor();

    virtual ~PolygonActor();

    /// \~chinese
    /// @brief ���ö���ζ˵�
    /// @param points ����ζ˵㼯��
    void SetVertices(Vector<Point> const& points);
};


/** @} */

inline void ShapeActor::SetStrokeColor(Color const& color)
{
    if (!stroke_brush_)
    {
        stroke_brush_ = new Brush;
    }
    stroke_brush_->SetColor(color);
}

inline void ShapeActor::SetFillColor(Color const& color)
{
    if (!fill_brush_)
    {
        fill_brush_ = new Brush;
    }
    fill_brush_->SetColor(color);
}

inline void ShapeActor::SetFillBrush(BrushPtr brush)
{
    fill_brush_ = brush;
}
inline void ShapeActor::SetStrokeBrush(BrushPtr brush)
{
    stroke_brush_ = brush;
}
inline BrushPtr ShapeActor::GetFillBrush() const
{
    return fill_brush_;
}
inline BrushPtr ShapeActor::GetStrokeBrush() const
{
    return stroke_brush_;
}
inline float ShapeActor::GetStrokeWidth() const
{
    return stroke_width_;
}
inline const StrokeStyle& ShapeActor::GetStrokeStyle() const
{
    return stroke_style_;
}
inline ShapePtr ShapeActor::GetShape() const
{
    return shape_;
}

inline Point const& LineActor::GetBeginPoint() const
{
    return begin_;
}
inline Point const& LineActor::GetEndPoint() const
{
    return end_;
}
inline void LineActor::SetBeginPoint(Point const& begin)
{
    SetLine(begin, end_);
}
inline void LineActor::SetEndPoint(Point const& end)
{
    SetLine(begin_, end);
}

inline Size const& RectActor::GetRectSize() const
{
    return rect_size_;
}

inline Vec2 RoundRectActor::GetRadius() const
{
    return radius_;
}
inline Size RoundRectActor::GetRectSize() const
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
