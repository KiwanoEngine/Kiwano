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

#include <kiwano/2d/ShapeActor.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

ShapeActor::ShapeActor() {}

ShapeActor::ShapeActor(ShapePtr shape)
{
    SetShape(shape);
}

ShapeActor::ShapeActor(ShapePtr shape, const Color& fill_color, const Color& stroke_color)
    : ShapeActor(shape)
{
    SetFillColor(fill_color);
    SetStrokeColor(stroke_color);
}

ShapeActor::ShapeActor(ShapePtr shape, BrushPtr fill_brush, BrushPtr stroke_brush)
    : ShapeActor(shape)
{
    SetFillBrush(fill_brush);
    SetStrokeBrush(stroke_brush);
}

ShapeActor::~ShapeActor() {}

Rect ShapeActor::GetBounds() const
{
    return bounds_;
}

Rect ShapeActor::GetBoundingBox() const
{
    if (!shape_)
        return Rect{};

    return shape_->GetBoundingBox(GetTransformMatrix());
}

bool ShapeActor::ContainsPoint(const Point& point) const
{
    if (!shape_)
        return false;

    return shape_->ContainsPoint(point, &GetTransformMatrix());
}

void ShapeActor::SetShape(ShapePtr shape)
{
    shape_ = shape;
    if (shape_)
    {
        bounds_ = shape_->GetBoundingBox();
        SetSize(bounds_.GetSize());
    }
    else
    {
        bounds_ = Rect{};
        SetSize(0.f, 0.f);
    }
}

void ShapeActor::OnRender(RenderContext& ctx)
{
    if (shape_)
    {
        if (stroke_brush_)
        {
            ctx.SetCurrentBrush(stroke_brush_);
            ctx.SetCurrentStrokeStyle(stroke_style_);
            ctx.DrawShape(*shape_);
        }

        if (fill_brush_)
        {
            ctx.SetCurrentBrush(fill_brush_);
            ctx.FillShape(*shape_);
        }
    }
}

bool ShapeActor::CheckVisibility(RenderContext& ctx) const
{
    return shape_ && Actor::CheckVisibility(ctx);
}

//-------------------------------------------------------
// LineActor
//-------------------------------------------------------

LineActor::LineActor() {}

LineActor::LineActor(const Point& begin, const Point& end)
{
    SetLine(begin, end);
}

LineActor::~LineActor() {}

void LineActor::SetLine(const Point& begin, const Point& end)
{
    if (begin_ != begin || end_ != end)
    {
        begin_ = begin;
        end_   = end;
        SetShape(Shape::CreateLine(begin, end));
    }
}

//-------------------------------------------------------
// RectActor
//-------------------------------------------------------

RectActor::RectActor() {}

RectActor::RectActor(const Size& size)
{
    SetRectSize(size);
}

RectActor::~RectActor() {}

void RectActor::SetRectSize(const Size& size)
{
    if (size != rect_size_)
    {
        rect_size_ = size;
        SetShape(Shape::CreateRect(Rect{ Point{}, size }));
    }
}

//-------------------------------------------------------
// RoundedRectActor
//-------------------------------------------------------

RoundedRectActor::RoundedRectActor() {}

RoundedRectActor::RoundedRectActor(const Size& size, const Vec2& radius)
{
    SetRoundedRect(size, radius);
}

RoundedRectActor::~RoundedRectActor() {}

void RoundedRectActor::SetRadius(const Vec2& radius)
{
    SetRoundedRect(GetSize(), radius);
}

void RoundedRectActor::SetRectSize(const Size& size)
{
    SetRoundedRect(size, radius_);
}

void RoundedRectActor::SetRoundedRect(const Size& size, const Vec2& radius)
{
    if (rect_size_ != size || radius_ != radius)
    {
        rect_size_ = size;
        radius_    = radius;
        SetShape(Shape::CreateRoundedRect(Rect{ Point{}, size }, radius));
    }
}

//-------------------------------------------------------
// CircleActor
//-------------------------------------------------------

CircleActor::CircleActor()
    : radius_(0.f)
{
}

CircleActor::CircleActor(float radius)
    : radius_(0.f)
{
    SetRadius(radius);
}

CircleActor::~CircleActor() {}

void CircleActor::SetRadius(float radius)
{
    if (radius_ != radius)
    {
        radius_ = radius;
        SetShape(Shape::CreateCircle(Point{ radius, radius }, radius));
    }
}

//-------------------------------------------------------
// EllipseActor
//-------------------------------------------------------

EllipseActor::EllipseActor() {}

EllipseActor::EllipseActor(const Vec2& radius)
{
    SetRadius(radius);
}

EllipseActor::~EllipseActor() {}

void EllipseActor::SetRadius(const Vec2& radius)
{
    if (radius_ != radius)
    {
        radius_ = radius;
        SetShape(Shape::CreateEllipse(radius, radius));
    }
}

//-------------------------------------------------------
// PolygonActor
//-------------------------------------------------------

PolygonActor::PolygonActor() {}

PolygonActor::PolygonActor(const Vector<Point>& vertices)
{
    SetVertices(vertices);
}

PolygonActor::~PolygonActor() {}

void PolygonActor::SetVertices(const Vector<Point>& vertices)
{
    if (vertices.size() > 1)
    {
        ShapeMaker maker;
        maker.BeginPath(vertices[0]);
        maker.AddLines(&vertices[1], vertices.size() - 1);
        maker.EndPath(true);

        SetShape(maker.GetShape());
    }
}

}  // namespace kiwano
