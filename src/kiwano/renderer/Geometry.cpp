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

#include <kiwano/renderer/Geometry.h>
#include <kiwano/renderer/GeometrySink.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{

	Geometry::Geometry()
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

	float Geometry::GetLength() const
	{
		float length = 0.f;
		if (geo_)
		{
			// no matter it failed or not
			geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
		}
		return length;
	}

	bool Geometry::ComputePointAtLength(float length, Point& point, Vec2& tangent) const
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

	void Geometry::Clear()
	{
		geo_.reset();
	}

	float Geometry::ComputeArea() const
	{
		if (!geo_)
			return 0.f;

		float area = 0.f;
		// no matter it failed or not
		geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
		return area;
	}

	bool Geometry::ContainsPoint(Point const& point, const Matrix3x2* transform) const
	{
		if (!geo_)
			return false;

		BOOL ret = 0;
		// no matter it failed or not
		geo_->FillContainsPoint(
			DX::ConvertToPoint2F(point),
			DX::ConvertToMatrix3x2F(transform),
			D2D1_DEFAULT_FLATTENING_TOLERANCE,
			&ret
		);
		return !!ret;
	}

	Geometry Geometry::CreateLine(Point const& begin, Point const& end)
	{
		Geometry output;
		Renderer::Instance().CreateLineGeometry(output, begin, end);
		return output;
	}

	Geometry Geometry::CreateRect(Rect const& rect)
	{
		Geometry output;
		Renderer::Instance().CreateRectGeometry(output, rect);
		return output;
	}

	Geometry Geometry::CreateRoundedRect(Rect const& rect, Vec2 const& radius)
	{
		Geometry output;
		Renderer::Instance().CreateRoundedRectGeometry(output, rect, radius);
		return output;
	}

	Geometry Geometry::CreateCircle(Point const& center, float radius)
	{
		Geometry output;
		Renderer::Instance().CreateEllipseGeometry(output, center, Vec2{ radius, radius });
		return output;
	}

	Geometry Geometry::CreateEllipse(Point const& center, Vec2 const& radius)
	{
		Geometry output;
		Renderer::Instance().CreateEllipseGeometry(output, center, radius);
		return output;
	}

}
