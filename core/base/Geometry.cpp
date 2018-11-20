// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "Geometry.h"
#include "render.h"
#include "logs.h"

namespace easy2d
{
	//-------------------------------------------------------
	// Geometry
	//-------------------------------------------------------

	Geometry::Geometry()
	{
	}

	Geometry::~Geometry()
	{
	}

	Rect Geometry::GetBoundingBox()
	{
		if (!geo_)
			return Rect{};

		D2D1_RECT_F rect;
		// no matter it failed or not
		geo_->GetBounds(
			ConvertToD2DMatrix(GetTransformMatrix()),
			&rect
		);
		return rect;
	}

	float Geometry::GetLength()
	{
		float length = 0.f;
		if (geo_)
		{
			// no matter it failed or not
			geo_->ComputeLength(
				ConvertToD2DMatrix(GetTransformMatrix()),
				&length
			);
		}
		return length;
	}

	bool Geometry::ComputePointAt(float length, Point* point, Point* tangent)
	{
		if (geo_)
		{
			D2D1_POINT_2F point_tmp, tangent_tmp;
			if (SUCCEEDED(geo_->ComputePointAtLength(
				length,
				ConvertToD2DMatrix(GetTransformMatrix()),
				&point_tmp,
				&tangent_tmp)))
			{
				(*point).x = point_tmp.x;
				(*point).y = point_tmp.y;
				(*tangent).x = tangent_tmp.x;
				(*tangent).y = tangent_tmp.y;
				return true;
			}
		}
		return false;
	}

	float Geometry::ComputeArea()
	{
		if (!geo_)
			return 0.f;

		float area = 0.f;
		// no matter it failed or not
		geo_->ComputeArea(
			ConvertToD2DMatrix(GetTransformMatrix()),
			&area
		);
		return area;
	}

	bool Geometry::ContainsPoint(Point const & point)
	{
		if (!geo_)
			return false;

		BOOL ret = 0;
		// no matter it failed or not
		geo_->FillContainsPoint(
			point,
			ConvertToD2DMatrix(GetTransformMatrix()),
			&ret
		);
		return !!ret;
	}

	GeometryRelation Geometry::GetRelationWith(spGeometry const & other)
	{
		if (!geo_ || !other->geo_)
			return GeometryRelation::Unknown;

		cpTransformedGeometry transformed;
		HRESULT hr = devices::Graphics::Instance()->CreateTransformedGeometry(
			transformed,
			GetTransformMatrix(),
			geo_.Get()
		);

		D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
		
		if (SUCCEEDED(hr))
		{
			transformed->CompareWithGeometry(
				other->geo_.Get(),
				ConvertToD2DMatrix(other->GetTransformMatrix()),
				&relation
			);
		}
		return GeometryRelation(relation);
	}


	//-------------------------------------------------------
	// LineGeometry
	//-------------------------------------------------------

	LineGeometry::LineGeometry()
	{
	}

	LineGeometry::LineGeometry(Point const & begin, Point const & end)
	{
		SetLine(begin, end);
	}

	LineGeometry::~LineGeometry()
	{
	}

	void LineGeometry::SetLine(Point const & begin, Point const & end)
	{
		cpPathGeometry path_geo;
		cpGeometrySink path_sink;

		HRESULT hr = devices::Graphics::Instance()->CreatePathGeometry(path_geo);

		if (SUCCEEDED(hr))
		{
			hr = path_geo->Open(&path_sink);
		}

		if (SUCCEEDED(hr))
		{
			path_sink->BeginFigure(begin, D2D1_FIGURE_BEGIN_FILLED);
			path_sink->AddLine(end);
			path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
			hr = path_sink->Close();
		}

		if (SUCCEEDED(hr))
		{
			geo_ = path_geo;
		}
	}

	void LineGeometry::SetBegin(Point const & begin)
	{
		SetLine(begin, end_);
	}

	void LineGeometry::SetEnd(Point const & end)
	{
		SetLine(begin_, end);
	}


	//-------------------------------------------------------
	// RectangleGeometry
	//-------------------------------------------------------

	RectangleGeometry::RectangleGeometry()
	{
	}

	RectangleGeometry::RectangleGeometry(Rect const & rect)
	{
		SetRect(rect);
	}

	RectangleGeometry::RectangleGeometry(Point const & left_top, Size const & size)
	{
		SetRect(Rect{ left_top, size });
	}

	RectangleGeometry::~RectangleGeometry()
	{
	}

	void RectangleGeometry::SetRect(Rect const & rect)
	{
		cpRectangleGeometry geo;
		if (SUCCEEDED(devices::Graphics::Instance()->CreateRectangleGeometry(geo, rect)))
		{
			geo_ = geo;
			rect_ = rect;
		}
	}


	//-------------------------------------------------------
	// CircleGeometry
	//-------------------------------------------------------

	CircleGeometry::CircleGeometry()
		: radius_(0.f)
	{
	}

	CircleGeometry::CircleGeometry(Point const & center, float radius)
	{
		SetCircle(center, radius);
	}

	CircleGeometry::~CircleGeometry()
	{
	}

	void CircleGeometry::SetRadius(float radius)
	{
		SetCircle(center_, radius);
	}

	void CircleGeometry::SetCenter(Point const & center)
	{
		SetCircle(center, radius_);
	}

	void CircleGeometry::SetCircle(Point const & center, float radius)
	{
		cpEllipseGeometry geo;
		if (SUCCEEDED(devices::Graphics::Instance()->CreateEllipseGeometry(geo, center, radius, radius)))
		{
			geo_ = geo;
			center_ = center;
			radius_ = radius;
		}
	}


	//-------------------------------------------------------
	// EllipseGeometry
	//-------------------------------------------------------

	EllipseGeometry::EllipseGeometry()
		: radius_x_(0.f)
		, radius_y_(0.f)
	{
	}

	EllipseGeometry::EllipseGeometry(Point const & center, float radius_x, float radius_y)
	{
		SetEllipse(center, radius_x, radius_y);
	}

	EllipseGeometry::~EllipseGeometry()
	{
	}

	void EllipseGeometry::SetRadius(float radius_x, float radius_y)
	{
		SetEllipse(center_, radius_x, radius_y);
	}

	void EllipseGeometry::SetCenter(Point const & center)
	{
		SetEllipse(center, radius_x_, radius_y_);
	}

	void EllipseGeometry::SetEllipse(Point const & center, float radius_x, float radius_y)
	{
		cpEllipseGeometry geo;
		if (SUCCEEDED(devices::Graphics::Instance()->CreateEllipseGeometry(geo, center, radius_x, radius_y)))
		{
			geo_ = geo;
			radius_x_ = radius_x;
			radius_y_ = radius_y;
		}
	}


	//-------------------------------------------------------
	// PathGeometry
	//-------------------------------------------------------

	PathGeometry::PathGeometry()
	{
	}

	PathGeometry::~PathGeometry()
	{
	}

	void PathGeometry::BeginPath(Point const& begin_pos)
	{
		current_geometry_ = nullptr;

		ThrowIfFailed(
			devices::Graphics::Instance()->CreatePathGeometry(current_geometry_)
		);

		ThrowIfFailed(
			current_geometry_->Open(&current_sink_)
		);

		current_sink_->BeginFigure(begin_pos, D2D1_FIGURE_BEGIN_FILLED);
	}

	void PathGeometry::EndPath(bool closed)
	{
		if (current_sink_)
		{
			current_sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
			ThrowIfFailed(
				current_sink_->Close()
			);

			geo_ = current_geometry_;

			current_sink_ = nullptr;
			current_geometry_ = nullptr;
		}
	}

	void PathGeometry::AddLine(Point const & point)
	{
		if (current_sink_)
			current_sink_->AddLine(point);
	}

	void PathGeometry::AddLines(std::vector<Point> const& points)
	{
		if (current_sink_)
		{
			if (!points.empty())
			{
				size_t size = points.size();
				std::vector<D2D1_POINT_2F> d2d_points(size);
				for (size_t i = 0; i < size; ++i)
				{
					d2d_points[i] = points[i];
				}

				current_sink_->AddLines(
					&d2d_points[0],
					static_cast<UINT32>(size)
				);
			}
		}
	}

	void PathGeometry::AddBezier(Point const & point1, Point const & point2, Point const & point3)
	{
		if (current_sink_)
		{
			current_sink_->AddBezier(
				D2D1::BezierSegment(
					point1,
					point2,
					point3
				)
			);
		}
	}

	void PathGeometry::ClearPath()
	{
		geo_ = nullptr;
		current_sink_ = nullptr;
		current_geometry_ = nullptr;
	}


	//-------------------------------------------------------
	// RoundedRectGeometry
	//-------------------------------------------------------

	RoundedRectGeometry::RoundedRectGeometry()
		: radius_x_(0.f)
		, radius_y_(0.f)
	{
	}

	RoundedRectGeometry::RoundedRectGeometry(Rect const & rect, float radius_x, float radius_y)
	{
		SetRoundedRect(rect, radius_x, radius_y);
	}

	RoundedRectGeometry::~RoundedRectGeometry()
	{
	}

	void RoundedRectGeometry::SetRadius(float radius_x, float radius_y)
	{
		SetRoundedRect(rect_, radius_x, radius_y);
	}

	void RoundedRectGeometry::SetRect(Rect const & rect)
	{
		SetRoundedRect(rect, radius_x_, radius_y_);
	}

	void RoundedRectGeometry::SetRoundedRect(Rect const & rect, float radius_x, float radius_y)
	{
		cpRoundedRectangleGeometry geo;
		if (SUCCEEDED(devices::Graphics::Instance()->CreateRoundedRectangleGeometry(geo, rect, radius_x, radius_y)))
		{
			geo_ = geo;
			rect_ = rect;
			radius_x_ = radius_x;
			radius_y_ = radius_y;
		}
	}

}