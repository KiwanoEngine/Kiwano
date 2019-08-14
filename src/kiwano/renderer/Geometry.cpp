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

#include "Geometry.h"
#include "Renderer.h"
#include "../base/Logger.h"

namespace kiwano
{

	//
	// Geometry
	//

	Geometry::Geometry()
	{
	}

	Geometry::Geometry(ComPtr<ID2D1Geometry> geo)
		: geo_(geo)
	{
	}

	Rect Geometry::GetBoundingBox(Matrix const& transform) const
	{
		if (!geo_)
			return Rect{};

		D2D1_RECT_F rect;
		// no matter it failed or not
		geo_->GetBounds(DX::ConvertToMatrix3x2F(transform), &rect);
		return Rect{ rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
	}

	float Geometry::GetLength()
	{
		float length = 0.f;
		if (geo_)
		{
			// no matter it failed or not
			geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
		}
		return length;
	}

	bool Geometry::ComputePointAtLength(float length, Point& point, Vec2& tangent)
	{
		if (geo_)
		{
			HRESULT hr = geo_->ComputePointAtLength(
				length,
				D2D1::Matrix3x2F::Identity(),
				DX::ConvertToPoint2F(&point),
				DX::ConvertToPoint2F(&tangent)
			);

			return SUCCEEDED(hr);
		}
		return false;
	}

	float Geometry::ComputeArea()
	{
		if (!geo_)
			return 0.f;

		float area = 0.f;
		// no matter it failed or not
		geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
		return area;
	}

	bool Geometry::ContainsPoint(Point const& point)
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

	//
	// GeometrySink
	//

	GeometrySink::GeometrySink()
	{
	}

	GeometrySink& GeometrySink::BeginPath(Point const& begin_pos)
	{
		if (!path_geo_)
		{
			Renderer::GetInstance()->CreatePathGeometrySink(*this);
		}

		if (!sink_)
		{
			ThrowIfFailed(path_geo_->Open(&sink_));
		}

		if (sink_)
		{
			sink_->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
		}
		return (*this);
	}

	GeometrySink& GeometrySink::EndPath(bool closed)
	{
		if (sink_)
		{
			sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
			
			ThrowIfFailed(sink_->Close());

			sink_ = nullptr;
		}
		return (*this);
	}

	GeometrySink& GeometrySink::AddLine(Point const& point)
	{
		if (!sink_) BeginPath();

		sink_->AddLine(DX::ConvertToPoint2F(point));
		return (*this);
	}

	GeometrySink& GeometrySink::AddLines(Vector<Point> const& points)
	{
		if (!sink_) BeginPath();
		
		sink_->AddLines(
			reinterpret_cast<const D2D_POINT_2F*>(&points[0]),
			static_cast<UINT32>(points.size())
		);
		return (*this);
	}

	GeometrySink& GeometrySink::AddBezier(Point const& point1, Point const& point2, Point const& point3)
	{
		if (!sink_) BeginPath();
		
		sink_->AddBezier(
			D2D1::BezierSegment(
				DX::ConvertToPoint2F(point1),
				DX::ConvertToPoint2F(point2),
				DX::ConvertToPoint2F(point3)
			)
		);
		return (*this);
	}

	GeometrySink& GeometrySink::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
	{
		if (!sink_) BeginPath();

		sink_->AddArc(
			D2D1::ArcSegment(
				DX::ConvertToPoint2F(point),
				DX::ConvertToSizeF(radius),
				rotation,
				clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
				is_small ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE
			)
		);
		return (*this);
	}

	Geometry GeometrySink::GetGeometry()
	{
		if (sink_)
		{
			EndPath();
		}
		return Geometry(path_geo_);
	}

}
