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
#include "../base/logs.h"

namespace kiwano
{
	ShapeActor::ShapeActor()
		: fill_color_(Color::White)
		, stroke_color_(Color(Color::Black, 0))
		, stroke_width_(1.f)
		, outline_join_(StrokeStyle::Miter)
	{
	}

	ShapeActor::ShapeActor(ComPtr<ID2D1Geometry> geometry)
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

		D2D1_RECT_F rect;
		// no matter it failed or not
		geo_->GetBounds(D2D1::Matrix3x2F::Identity(), &rect);
		return Rect{ rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
	}

	Rect ShapeActor::GetBoundingBox() const
	{
		if (!geo_)
			return Rect{};

		D2D1_RECT_F rect;
		// no matter it failed or not
		geo_->GetBounds(DX::ConvertToMatrix3x2F(transform_matrix_), &rect);
		return Rect{ rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
	}

	float ShapeActor::GetLength()
	{
		float length = 0.f;
		if (geo_)
		{
			// no matter it failed or not
			geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
		}
		return length;
	}

	bool ShapeActor::ComputePointAtLength(float length, Point& point, Vec2& tangent)
	{
		if (geo_)
		{
			if (SUCCEEDED(geo_->ComputePointAtLength(
				length,
				D2D1::Matrix3x2F::Identity(),
				DX::ConvertToPoint2F(&point),
				DX::ConvertToPoint2F(&tangent))))
			{
				return true;
			}
		}
		return false;
	}

	float ShapeActor::ComputeArea()
	{
		if (!geo_)
			return 0.f;

		float area = 0.f;
		// no matter it failed or not
		geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
		return area;
	}

	bool ShapeActor::ContainsPoint(Point const& point)
	{
		if (!geo_)
			return false;

		BOOL ret = 0;
		// no matter it failed or not
		geo_->FillContainsPoint(
			DX::ConvertToPoint2F(point),
			D2D1::Matrix3x2F::Identity(),
			&ret
		);
		return !!ret;
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

	LineActor::LineActor(Point const& begin, Point const& end)
	{
		SetLine(begin, end);
	}

	LineActor::~LineActor()
	{
	}

	void LineActor::SetLine(Point const& begin, Point const& end)
	{
		ComPtr<ID2D1PathGeometry> path_geo;
		ComPtr<ID2D1GeometrySink> path_sink;

		HRESULT hr = Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory()->CreatePathGeometry(&path_geo);

		if (SUCCEEDED(hr))
		{
			hr = path_geo->Open(&path_sink);
		}

		if (SUCCEEDED(hr))
		{
			path_sink->BeginFigure(DX::ConvertToPoint2F(begin), D2D1_FIGURE_BEGIN_FILLED);
			path_sink->AddLine(DX::ConvertToPoint2F(end));
			path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
			hr = path_sink->Close();
		}

		if (SUCCEEDED(hr))
		{
			geo_ = path_geo;
		}
	}

	void LineActor::SetBegin(Point const& begin)
	{
		SetLine(begin, end_);
	}

	void LineActor::SetEnd(Point const& end)
	{
		SetLine(begin_, end);
	}


	//-------------------------------------------------------
	// RectActor
	//-------------------------------------------------------

	RectActor::RectActor()
	{
	}

	RectActor::RectActor(Rect const& rect)
	{
		SetRect(rect);
	}

	RectActor::RectActor(Point const& left_top, Size const& size)
	{
		SetRect(Rect{ left_top, size });
	}

	RectActor::~RectActor()
	{
	}

	void RectActor::SetRect(Rect const& rect)
	{
		ComPtr<ID2D1RectangleGeometry> geo;
		auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory();

		if (SUCCEEDED(factory->CreateRectangleGeometry(DX::ConvertToRectF(rect), &geo)))
		{
			geo_ = geo;
			rect_ = rect;
		}
	}


	//-------------------------------------------------------
	// RoundRectActor
	//-------------------------------------------------------

	RoundRectActor::RoundRectActor()
		: radius_x_(0.f)
		, radius_y_(0.f)
	{
	}

	RoundRectActor::RoundRectActor(Rect const& rect, float radius_x, float radius_y)
	{
		SetRoundedRect(rect, radius_x, radius_y);
	}

	RoundRectActor::~RoundRectActor()
	{
	}

	void RoundRectActor::SetRadius(float radius_x, float radius_y)
	{
		SetRoundedRect(rect_, radius_x, radius_y);
	}

	void RoundRectActor::SetRect(Rect const& rect)
	{
		SetRoundedRect(rect, radius_x_, radius_y_);
	}

	void RoundRectActor::SetRoundedRect(Rect const& rect, float radius_x, float radius_y)
	{
		ComPtr<ID2D1RoundedRectangleGeometry> geo;
		auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory();

		if (SUCCEEDED(factory->CreateRoundedRectangleGeometry(
			D2D1::RoundedRect(
				DX::ConvertToRectF(rect),
				radius_x,
				radius_y
			),
			&geo)))
		{
			geo_ = geo;
			rect_ = rect;
			radius_x_ = radius_x;
			radius_y_ = radius_y;
		}
	}


	//-------------------------------------------------------
	// CircleActor
	//-------------------------------------------------------

	CircleActor::CircleActor()
		: radius_(0.f)
	{
	}

	CircleActor::CircleActor(Point const& center, float radius)
	{
		SetCircle(center, radius);
	}

	CircleActor::~CircleActor()
	{
	}

	void CircleActor::SetRadius(float radius)
	{
		SetCircle(center_, radius);
	}

	void CircleActor::SetCenter(Point const& center)
	{
		SetCircle(center, radius_);
	}

	void CircleActor::SetCircle(Point const& center, float radius)
	{
		ComPtr<ID2D1EllipseGeometry> geo;
		auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory();

		if (SUCCEEDED(factory->CreateEllipseGeometry(
			D2D1::Ellipse(
				DX::ConvertToPoint2F(center),
				radius,
				radius),
			&geo)))
		{
			geo_ = geo;
			center_ = center;
			radius_ = radius;
		}
	}


	//-------------------------------------------------------
	// EllipseActor
	//-------------------------------------------------------

	EllipseActor::EllipseActor()
		: radius_x_(0.f)
		, radius_y_(0.f)
	{
	}

	EllipseActor::EllipseActor(Point const& center, float radius_x, float radius_y)
	{
		SetEllipse(center, radius_x, radius_y);
	}

	EllipseActor::~EllipseActor()
	{
	}

	void EllipseActor::SetRadius(float radius_x, float radius_y)
	{
		SetEllipse(center_, radius_x, radius_y);
	}

	void EllipseActor::SetCenter(Point const& center)
	{
		SetEllipse(center, radius_x_, radius_y_);
	}

	void EllipseActor::SetEllipse(Point const& center, float radius_x, float radius_y)
	{
		ComPtr<ID2D1EllipseGeometry> geo;
		auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory();

		if (SUCCEEDED(factory->CreateEllipseGeometry(
			D2D1::Ellipse(
				DX::ConvertToPoint2F(center),
				radius_x,
				radius_y),
			&geo)))
		{
			geo_ = geo;
			radius_x_ = radius_x;
			radius_y_ = radius_y;
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
		current_geometry_ = nullptr;

		auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory();

		ThrowIfFailed(
			factory->CreatePathGeometry(&current_geometry_)
		);

		ThrowIfFailed(
			current_geometry_->Open(&current_sink_)
		);

		current_sink_->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
	}

	void PathActor::EndPath(bool closed)
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

	void PathActor::AddLine(Point const& point)
	{
		if (current_sink_)
			current_sink_->AddLine(DX::ConvertToPoint2F(point));
	}

	void PathActor::AddLines(Vector<Point> const& points)
	{
		if (current_sink_ && !points.empty())
		{
			current_sink_->AddLines(
				reinterpret_cast<const D2D_POINT_2F*>(&points[0]),
				static_cast<UINT32>(points.size())
			);
		}
	}

	void PathActor::AddBezier(Point const& point1, Point const& point2, Point const& point3)
	{
		if (current_sink_)
		{
			current_sink_->AddBezier(
				D2D1::BezierSegment(
					DX::ConvertToPoint2F(point1),
					DX::ConvertToPoint2F(point2),
					DX::ConvertToPoint2F(point3)
				)
			);
		}
	}

	void PathActor::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
	{
		if (current_sink_)
		{
			current_sink_->AddArc(
				D2D1::ArcSegment(
					DX::ConvertToPoint2F(point),
					DX::ConvertToSizeF(radius),
					rotation,
					clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
					is_small ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE
				)
			);
		}
	}

	void PathActor::ClearPath()
	{
		geo_ = nullptr;
		current_sink_ = nullptr;
		current_geometry_ = nullptr;
	}

}
