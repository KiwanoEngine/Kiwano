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
#include <kiwano/core/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{
ShapeActor::ShapeActor()
    : stroke_width_(1.f)
    , stroke_style_()
{
}

ShapeActor::~ShapeActor() {}

Rect ShapeActor::GetBounds() const
{
    return bounds_;
}

Rect ShapeActor::GetBoundingBox() const
{
    if (!geo_.IsValid())
        return Rect{};

    return geo_.GetBoundingBox(GetTransformMatrix());
}

bool ShapeActor::ContainsPoint(const Point& point) const
{
    return geo_.ContainsPoint(point, &GetTransformMatrix());
}

void ShapeActor::SetStrokeWidth(float width)
{
    stroke_width_ = std::max(width, 0.f);
}

void ShapeActor::SetStrokeStyle(const StrokeStyle& stroke_style)
{
    stroke_style_ = stroke_style;
}

void ShapeActor::SetGeometry(Geometry const& geometry)
{
    geo_ = geometry;
    if (geo_.IsValid())
    {
        bounds_ = geo_.GetBoundingBox();
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
    // Create default brush
    if (!fill_brush_)
    {
        fill_brush_ = new Brush;
        fill_brush_->SetColor(Color::White);
    }

    if (!stroke_brush_)
    {
        stroke_brush_ = new Brush;
        stroke_brush_->SetColor(Color::Transparent);
    }

    ctx.SetCurrentBrush(stroke_brush_);
    ctx.DrawGeometry(geo_, stroke_width_ * 2 /* twice width for widening */, stroke_style_);

    ctx.SetCurrentBrush(fill_brush_);
    ctx.FillGeometry(geo_);
}

bool ShapeActor::CheckVisibility(RenderContext& ctx) const
{
    return geo_.IsValid() && Actor::CheckVisibility(ctx);
}

//-------------------------------------------------------
// LineActor
//-------------------------------------------------------

LineActor::LineActor() {}

LineActor::~LineActor() {}

void LineActor::SetLine(Point const& begin, Point const& end)
{
    if (begin_ != begin || end_ != end)
    {
        begin_ = begin;
        end_   = end;
        SetGeometry(Geometry::CreateLine(begin, end));
    }
}

//-------------------------------------------------------
// RectActor
//-------------------------------------------------------

RectActor::RectActor() {}

RectActor::~RectActor() {}

void RectActor::SetRectSize(Size const& size)
{
    if (size != rect_size_)
    {
        rect_size_ = size;
        SetGeometry(Geometry::CreateRect(Rect{ Point{}, size }));
    }
}

//-------------------------------------------------------
// RoundRectActor
//-------------------------------------------------------

RoundRectActor::RoundRectActor() {}

RoundRectActor::~RoundRectActor() {}

void RoundRectActor::SetRadius(Vec2 const& radius)
{
    SetRoundedRect(GetSize(), radius);
}

void RoundRectActor::SetRectSize(Size const& size)
{
    SetRoundedRect(size, radius_);
}

void RoundRectActor::SetRoundedRect(Size const& size, Vec2 const& radius)
{
    if (rect_size_ != size || radius_ != radius)
    {
        rect_size_ = size;
        radius_    = radius;
        SetGeometry(Geometry::CreateRoundedRect(Rect{ Point{}, size }, radius));
    }
}

//-------------------------------------------------------
// CircleActor
//-------------------------------------------------------

CircleActor::CircleActor()
    : radius_(0.f)
{
}

CircleActor::~CircleActor() {}

void CircleActor::SetRadius(float radius)
{
    if (radius_ != radius)
    {
        radius_ = radius;
        SetGeometry(Geometry::CreateCircle(Point{ radius, radius }, radius));
    }
}

//-------------------------------------------------------
// EllipseActor
//-------------------------------------------------------

EllipseActor::EllipseActor() {}

EllipseActor::~EllipseActor() {}

void EllipseActor::SetRadius(Vec2 const& radius)
{
    if (radius_ != radius)
    {
        radius_ = radius;
        SetGeometry(Geometry::CreateEllipse(radius, radius));
    }
}

//-------------------------------------------------------
// PolygonActor
//-------------------------------------------------------

PolygonActor::PolygonActor() {}

PolygonActor::~PolygonActor() {}

void PolygonActor::SetVertices(Vector<Point> const& points)
{
    if (points.size() > 1)
    {
        SetGeometry(
            GeometrySink().BeginPath(points[0]).AddLines(&points[1], points.size() - 1).EndPath(true).GetGeometry());
    }
}

}  // namespace kiwano
