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

Rect Shape::GetBoundingBox() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    Rect bounds;
    if (geo_)
    {
        // no matter it failed or not
        geo_->GetBounds(nullptr, DX::ConvertToRectF(&bounds));
    }
    return bounds;
#else
    return Rect();  // not supported
#endif
}

Rect Shape::GetBoundingBox(Matrix3x2 const& transform) const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    Rect bounds;
    if (geo_)
    {
        // no matter it failed or not
        geo_->GetBounds(DX::ConvertToMatrix3x2F(transform), DX::ConvertToRectF(&bounds));
    }
    return bounds;
#else
    return Rect();  // not supported
#endif
}

float Shape::GetLength() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    float length = 0.f;
    if (geo_)
    {
        // no matter it failed or not
        geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
    }
    return length;
#else
    return 0.0f;  // not supported
#endif
}

bool Shape::ComputePointAtLength(float length, Point& point, Vec2& tangent) const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (geo_)
    {
        HRESULT hr = geo_->ComputePointAtLength(length, D2D1::Matrix3x2F::Identity(), DX::ConvertToPoint2F(&point),
                                                DX::ConvertToPoint2F(&tangent));

        return SUCCEEDED(hr);
    }
    return false;
#else
    return false;  // not supported
#endif
}

void Shape::Clear()
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    geo_.reset();
#else
    return;  // not supported
#endif
}

float Shape::ComputeArea() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (!geo_)
        return 0.f;

    float area = 0.f;
    // no matter it failed or not
    geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
    return area;
#else
    return 0.0f;  // not supported
#endif
}

bool Shape::ContainsPoint(Point const& point, const Matrix3x2* transform) const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (!geo_)
        return false;

    BOOL ret = 0;
    // no matter it failed or not
    geo_->FillContainsPoint(DX::ConvertToPoint2F(point), DX::ConvertToMatrix3x2F(transform),
                            D2D1_DEFAULT_FLATTENING_TOLERANCE, &ret);
    return !!ret;
#else
    return false;  // not supported
#endif
}

ShapePtr Shape::CreateLine(Point const& begin, Point const& end)
{
    ShapePtr output = new Shape;
    Renderer::GetInstance().CreateLineShape(*output, begin, end);
    return output;
}

ShapePtr Shape::CreateRect(Rect const& rect)
{
    ShapePtr output = new Shape;
    Renderer::GetInstance().CreateRectShape(*output, rect);
    return output;
}

ShapePtr Shape::CreateRoundedRect(Rect const& rect, Vec2 const& radius)
{
    ShapePtr output = new Shape;
    Renderer::GetInstance().CreateRoundedRectShape(*output, rect, radius);
    return output;
}

ShapePtr Shape::CreateCircle(Point const& center, float radius)
{
    ShapePtr output = new Shape;
    Renderer::GetInstance().CreateEllipseShape(*output, center, Vec2{ radius, radius });
    return output;
}

ShapePtr Shape::CreateEllipse(Point const& center, Vec2 const& radius)
{
    ShapePtr output = new Shape;
    Renderer::GetInstance().CreateEllipseShape(*output, center, radius);
    return output;
}

}  // namespace kiwano
