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

	bool Geometry::IsValid() const
	{
		return geo_ != nullptr;
	}

	Rect Geometry::GetBoundingBox() const
	{
		Rect bounds;
		if (geo_)
		{
			// no matter it failed or not
			geo_->GetBounds(nullptr, DX::ConvertToRectF(&bounds));
		}
		return bounds;
	}

	Rect Geometry::GetBoundingBox(Matrix3x2 const& transform) const
	{
		Rect bounds;
		if (geo_)
		{
			// no matter it failed or not
			geo_->GetBounds(DX::ConvertToMatrix3x2F(transform), DX::ConvertToRectF(&bounds));
		}
		return bounds;
	}

	Float32 Geometry::GetLength() const
	{
		Float32 length = 0.f;
		if (geo_)
		{
			// no matter it failed or not
			geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
		}
		return length;
	}

	bool Geometry::ComputePointAtLength(Float32 length, Point& point, Vec2& tangent) const
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

	Geometry Geometry::CombineWith(Geometry input, CombineMode mode, Matrix3x2 const& input_matrix) const
	{
		if (geo_ && input.geo_)
		{
			GeometrySink sink;
			sink.Init();
			sink.OpenSink();

			ThrowIfFailed(
				geo_->CombineWithGeometry(
					input.geo_.get(),
					D2D1_COMBINE_MODE(mode),
					DX::ConvertToMatrix3x2F(input_matrix),
					sink.GetGeometrySink().get()
				)
			);

			sink.CloseSink();
			return sink.GetGeometry();
		}
		return Geometry();
	}

	Geometry Geometry::Combine(Vector<Geometry> const& geos, Vector<CombineMode> const& modes, Vector<Matrix3x2> const& matrixs)
	{
		if ((geos.size() == (modes.size() + 1) || modes.size() == 1)
			&& (geos.size() == (matrixs.size() + 1) || matrixs.size() == 1))
		{
			GeometrySink sink;
			sink.Init();
			sink.OpenSink();

			for (UInt32 i = 0; i < geos.size() - 1; i++)
			{
				CombineMode mode = (modes.size() == 1) ? modes[0] : modes[i];
				const Matrix3x2& matrix = (matrixs.size() == 1) ? matrixs[0] : matrixs[i];

				ThrowIfFailed(
					geos[i].geo_->CombineWithGeometry(
						geos[i + 1].geo_.get(),
						D2D1_COMBINE_MODE(mode),
						DX::ConvertToMatrix3x2F(matrix),
						sink.GetGeometrySink().get()
					)
				);
			}

			sink.CloseSink();
			return sink.GetGeometry();
		}
		return Geometry();
	}

	Float32 Geometry::ComputeArea() const
	{
		if (!geo_)
			return 0.f;

		Float32 area = 0.f;
		// no matter it failed or not
		geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
		return area;
	}

	bool Geometry::ContainsPoint(Point const& point, Matrix3x2 const& transform) const
	{
		if (!geo_)
			return false;

		BOOL ret = 0;
		// no matter it failed or not
		geo_->FillContainsPoint(
			DX::ConvertToPoint2F(point),
			DX::ConvertToMatrix3x2F(transform),
			&ret
		);
		return !!ret;
	}

	Geometry Geometry::CreateLine(Point const& begin, Point const& end)
	{
		Geometry output;
		Renderer::GetInstance()->CreateLineGeometry(output, begin, end);
		return output;
	}

	Geometry Geometry::CreateRect(Rect const& rect)
	{
		Geometry output;
		Renderer::GetInstance()->CreateRectGeometry(output, rect);
		return output;
	}

	Geometry Geometry::CreateRoundedRect(Rect const& rect, Vec2 const& radius)
	{
		Geometry output;
		Renderer::GetInstance()->CreateRoundedRectGeometry(output, rect, radius);
		return output;
	}

	Geometry Geometry::CreateCircle(Point const& center, Float32 radius)
	{
		Geometry output;
		Renderer::GetInstance()->CreateEllipseGeometry(output, center, Vec2{ radius, radius });
		return output;
	}

	Geometry Geometry::CreateEllipse(Point const& center, Vec2 const& radius)
	{
		Geometry output;
		Renderer::GetInstance()->CreateEllipseGeometry(output, center, radius);
		return output;
	}

	//
	// GeometrySink
	//

	GeometrySink::GeometrySink()
	{
	}

	GeometrySink& GeometrySink::BeginPath(Point const& begin_pos)
	{
		Init();

		if (!sink_)
		{
			OpenSink();
		}

		sink_->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
		return (*this);
	}

	GeometrySink& GeometrySink::EndPath(bool closed)
	{
		if (sink_)
		{
			sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
			
			CloseSink();
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
			static_cast<UInt32>(points.size())
		);
		return (*this);
	}

	GeometrySink& kiwano::GeometrySink::AddLines(const Point* points, UInt32 count)
	{
		if (!sink_) BeginPath();

		sink_->AddLines(reinterpret_cast<const D2D_POINT_2F*>(points), count);
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

	GeometrySink& GeometrySink::AddArc(Point const& point, Size const& radius, Float32 rotation, bool clockwise, bool is_small)
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

	void GeometrySink::Init()
	{
		if (!path_geo_)
		{
			Renderer::GetInstance()->CreatePathGeometrySink(*this);
		}
	}

	void GeometrySink::OpenSink()
	{
		if (!sink_)
		{
			ThrowIfFailed(path_geo_->Open(&sink_));
		}
	}

	void GeometrySink::CloseSink()
	{
		if (sink_)
		{
			ThrowIfFailed(sink_->Close());

			sink_ = nullptr;
		}
	}

}
