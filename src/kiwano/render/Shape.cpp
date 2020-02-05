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

#include <kiwano/render/Shape.h>
#include <kiwano/render/ShapeSink.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

Shape::Shape() {}

bool Shape::IsValid() const
{
    return geo_ != nullptr;
}

Rect Shape::GetBoundingBox() const
{
    Rect bounds;
    if (geo_)
    {
        // no matter it failed or not
        geo_->GetBounds(nullptr, DX::ConvertToRectF(&bounds));
    }
    return bounds;
}

Rect Shape::GetBoundingBox(Matrix3x2 const& transform) const
{
    Rect bounds;
    if (geo_)
    {
        // no matter it failed or not
        geo_->GetBounds(DX::ConvertToMatrix3x2F(transform), DX::ConvertToRectF(&bounds));
    }
    return bounds;
}

float Shape::GetLength() const
{
    float length = 0.f;
    if (geo_)
    {
        // no matter it failed or not
        geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
    }
    return length;
}

bool Shape::ComputePointAtLength(float length, Point& point, Vec2& tangent) const
{
    if (geo_)
    {
        HRESULT hr = geo_->ComputePointAtLength(length, D2D1::Matrix3x2F::Identity(), DX::ConvertToPoint2F(&point),
                                                DX::ConvertToPoint2F(&tangent));

        return SUCCEEDED(hr);
    }
    return false;
}

void Shape::Clear()
{
    geo_.reset();
}

float Shape::ComputeArea() const
{
    if (!geo_)
        return 0.f;

    float area = 0.f;
    // no matter it failed or not
    geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
    return area;
}

bool Shape::ContainsPoint(Point const& point, const Matrix3x2* transform) const
{
    if (!geo_)
        return false;

    BOOL ret = 0;
    // no matter it failed or not
    geo_->FillContainsPoint(DX::ConvertToPoint2F(point), DX::ConvertToMatrix3x2F(transform),
                            D2D1_DEFAULT_FLATTENING_TOLERANCE, &ret);
    return !!ret;
}

ShapePtr Shape::CreateLine(Point const& begin, Point const& end)
{
    ShapePtr output = new Shape;
    Renderer::Instance().CreateLineShape(*output, begin, end);
    return output;
}

ShapePtr Shape::CreateRect(Rect const& rect)
{
    ShapePtr output = new Shape;
    Renderer::Instance().CreateRectShape(*output, rect);
    return output;
}

ShapePtr Shape::CreateRoundedRect(Rect const& rect, Vec2 const& radius)
{
    ShapePtr output = new Shape;
    Renderer::Instance().CreateRoundedRectShape(*output, rect, radius);
    return output;
}

ShapePtr Shape::CreateCircle(Point const& center, float radius)
{
    ShapePtr output = new Shape;
    Renderer::Instance().CreateEllipseShape(*output, center, Vec2{ radius, radius });
    return output;
}

ShapePtr Shape::CreateEllipse(Point const& center, Vec2 const& radius)
{
    ShapePtr output = new Shape;
    Renderer::Instance().CreateEllipseShape(*output, center, radius);
    return output;
}

}  // namespace kiwano
