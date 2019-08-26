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

#include "ShapeActor.h"
#include "../base/Logger.h"
#include "../renderer/Renderer.h"

namespace kiwano
{
	ShapeActor::ShapeActor()
		: fill_color_(Color::White)
		, stroke_color_(Color(Color::Black, 0))
		, stroke_width_(1.f)
		, stroke_style_(StrokeStyle::Miter)
	{
	}

	ShapeActor::ShapeActor(Geometry const& geometry)
		: ShapeActor()
	{
		SetGeometry(geometry);
	}

	ShapeActor::~ShapeActor()
	{
	}

	Rect ShapeActor::GetBounds() const
	{
		return bounds_;
	}

	Rect ShapeActor::GetBoundingBox() const
	{
		if (!geo_)
			return Rect{};

		return geo_.GetBoundingBox(GetTransformMatrix());
	}

	bool ShapeActor::ContainsPoint(const Point& point) const
	{
		return geo_.ContainsPoint(point, GetTransformMatrix());
	}

	void ShapeActor::SetFillColor(const Color & color)
	{
		fill_color_ = color;
	}

	void ShapeActor::SetStrokeColor(const Color & color)
	{
		stroke_color_ = color;
	}

	void ShapeActor::SetStrokeWidth(Float32 width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void ShapeActor::SetStrokeStyle(StrokeStyle stroke_style)
	{
		stroke_style_ = stroke_style;
	}

	void ShapeActor::SetGeometry(Geometry const& geometry)
	{
		geo_ = geometry;
		if (geo_)
		{
			bounds_ = geo_.GetBoundingBox(Matrix3x2());
			SetSize(bounds_.GetSize());
		}
		else
		{
			bounds_ = Rect{};
			SetSize(0.f, 0.f);
		}
	}

	void ShapeActor::OnRender(RenderTarget* rt)
	{
		if (geo_ && rt->CheckVisibility(GetBounds(), GetTransformMatrix()))
		{
			PrepareRender(rt);

			rt->FillGeometry(
				geo_,
				fill_color_
			);

			rt->DrawGeometry(
				geo_,
				stroke_color_,
				stroke_width_,
				stroke_style_
			);
		}
	}

	//-------------------------------------------------------
	// LineActor
	//-------------------------------------------------------

	LineActor::LineActor()
	{
	}

	LineActor::LineActor(Point const& point)
	{
		SetPoint(point);
	}

	LineActor::~LineActor()
	{
	}

	void LineActor::SetPoint(Point const& point)
	{
		SetGeometry(Geometry::CreateLine(Point{}, point));
	}


	//-------------------------------------------------------
	// RectActor
	//-------------------------------------------------------

	RectActor::RectActor()
	{
	}

	RectActor::RectActor(Size const& size)
	{
		SetRectSize(size);
	}

	RectActor::~RectActor()
	{
	}

	void RectActor::SetRectSize(Size const& size)
	{
		SetGeometry(Geometry::CreateRect(Rect{ Point{}, size }));
	}


	//-------------------------------------------------------
	// RoundRectActor
	//-------------------------------------------------------

	RoundRectActor::RoundRectActor()
	{
	}

	RoundRectActor::RoundRectActor(Size const& size, Vec2 const& radius)
	{
		SetRoundedRect(size, radius);
	}

	RoundRectActor::~RoundRectActor()
	{
	}

	void RoundRectActor::SetRadius(Vec2 const& radius)
	{
		SetRoundedRect(size_, radius);
	}

	void RoundRectActor::SetRectSize(Size const& size)
	{
		SetRoundedRect(size, radius_);
	}

	void RoundRectActor::SetRoundedRect(Size const& size, Vec2 const& radius)
	{
		SetGeometry(Geometry::CreateRoundedRect(Rect{ Point{}, size }, radius));
	}


	//-------------------------------------------------------
	// CircleActor
	//-------------------------------------------------------

	CircleActor::CircleActor()
		: radius_(0.f)
	{
	}

	CircleActor::CircleActor(Float32 radius)
	{
		SetRadius(radius);
	}

	CircleActor::~CircleActor()
	{
	}

	void CircleActor::SetRadius(Float32 radius)
	{
		SetGeometry(Geometry::CreateCircle(Point{ radius, radius }, radius));
	}


	//-------------------------------------------------------
	// EllipseActor
	//-------------------------------------------------------

	EllipseActor::EllipseActor()
	{
	}

	EllipseActor::EllipseActor(Vec2 const& radius)
	{
		SetRadius(radius);
	}

	EllipseActor::~EllipseActor()
	{
	}

	void EllipseActor::SetRadius(Vec2 const& radius)
	{
		SetGeometry(Geometry::CreateEllipse(radius, radius));
	}


	//-------------------------------------------------------
	// PolygonActor
	//-------------------------------------------------------

	PolygonActor::PolygonActor()
	{
	}

	PolygonActor::PolygonActor(Vector<Point> const& points)
	{
		SetVertices(points);
	}

	PolygonActor::~PolygonActor()
	{
	}

	void PolygonActor::SetVertices(Vector<Point> const& points)
	{
		if (points.size() > 1)
		{
			SetGeometry(
				GeometrySink()
				.BeginPath(points[0])
				.AddLines(&points[1], points.size() - 1)
				.EndPath(true)
				.GetGeometry()
			);
		}
	}


	//-------------------------------------------------------
	// PathActor
	//-------------------------------------------------------

	PathActor::PathActor()
	{
	}

	PathActor::~PathActor()
	{
	}

	void PathActor::BeginPath(Point const& begin_pos)
	{
		sink_.BeginPath(begin_pos);
	}

	void PathActor::EndPath(bool closed)
	{
		sink_.EndPath(closed);
		Geometry geo = sink_.GetGeometry();

		if (geo)
		{
			SetGeometry(geo);
		}
	}

	void PathActor::AddLine(Point const& point)
	{
		sink_.AddLine(point);
	}

	void PathActor::AddLines(Vector<Point> const& points)
	{
		sink_.AddLines(points);
	}

	void PathActor::AddBezier(Point const& point1, Point const& point2, Point const& point3)
	{
		sink_.AddBezier(point1, point2, point3);
	}

	void PathActor::AddArc(Point const& point, Size const& radius, Float32 rotation, bool clockwise, bool is_small)
	{
		sink_.AddArc(point, radius, rotation, clockwise, is_small);
	}

	void PathActor::ClearPath()
	{
		geo_.SetGeometry(nullptr);
	}

}
