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
		, outline_join_(StrokeStyle::Miter)
	{
	}

	ShapeActor::ShapeActor(Geometry geometry)
		: ShapeActor()
	{
		SetGeometry(geometry);
	}

	ShapeActor::~ShapeActor()
	{
	}

	Rect ShapeActor::GetBounds() const
	{
		if (!geo_)
			return Rect{};

		return geo_.GetBoundingBox(Matrix3x2());
	}

	Rect ShapeActor::GetBoundingBox() const
	{
		if (!geo_)
			return Rect{};

		return geo_.GetBoundingBox(GetTransformMatrix());
	}

	void ShapeActor::SetFillColor(const Color & color)
	{
		fill_color_ = color;
	}

	void ShapeActor::SetStrokeColor(const Color & color)
	{
		stroke_color_ = color;
	}

	void ShapeActor::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void ShapeActor::SetOutlineJoinStyle(StrokeStyle outline_join)
	{
		outline_join_ = outline_join;
	}

	void ShapeActor::OnRender(Renderer* renderer)
	{
		if (geo_)
		{
			PrepareRender(renderer);

			renderer->FillGeometry(
				geo_,
				fill_color_
			);

			renderer->DrawGeometry(
				geo_,
				stroke_color_,
				stroke_width_,
				outline_join_
			);
		}
	}

	//-------------------------------------------------------
	// LineActor
	//-------------------------------------------------------

	LineActor::LineActor()
	{
	}

	LineActor::LineActor(Point const& end)
	{
		SetEndPoint(end);
	}

	LineActor::~LineActor()
	{
	}

	void LineActor::SetEndPoint(Point const& end)
	{
		geo_ = Geometry::CreateLine(Point{}, end);

		if (geo_)
		{
			SetSize(end);
		}
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
		geo_ = Geometry::CreateRect(Rect{ Point{}, size });

		if (geo_)
		{
			SetSize(size);
		}
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
		geo_ = Geometry::CreateRoundedRect(Rect{ Point{}, size }, radius);

		if (geo_)
		{
			SetSize(size);
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
	{
		SetRadius(radius);
	}

	CircleActor::~CircleActor()
	{
	}

	void CircleActor::SetRadius(float radius)
	{
		geo_ = Geometry::CreateCircle(Point{}, radius);

		if (geo_)
		{
			SetSize(radius * 2, radius * 2);
		}
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
		geo_ = Geometry::CreateEllipse(Point{}, radius);

		if (geo_)
		{
			SetSize(radius * 2);
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
		geo_ = sink_.GetGeometry();
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

	void PathActor::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
	{
		sink_.AddArc(point, radius, rotation, clockwise, is_small);
	}

	void PathActor::ClearPath()
	{
		geo_.SetGeometry(nullptr);
	}

}
