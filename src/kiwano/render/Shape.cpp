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
#include <kiwano/render/ShapeMaker.h>
#include <kiwano/render/Renderer.h>

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/helper.h>
#endif

namespace kiwano
{

Shape::Shape() {}

void Shape::Clear()
{
    ResetNative();
}

Rect Shape::GetBoundingBox() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    Rect bounds;
    auto geometry = ComPolicy::Get<ID2D1Geometry>(this);
    if (geometry)
    {
        // no matter it failed or not
        geometry->GetBounds(nullptr, DX::ConvertToRectF(&bounds));
    }
    return bounds;
#else
    return Rect();  // not supported
#endif
}

Rect Shape::GetBoundingBox(const Matrix3x2& transform) const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    Rect bounds;
    auto geometry = ComPolicy::Get<ID2D1Geometry>(this);
    if (geometry)
    {
        // no matter it failed or not
        geometry->GetBounds(DX::ConvertToMatrix3x2F(transform), DX::ConvertToRectF(&bounds));
    }
    return bounds;
#else
    return Rect();  // not supported
#endif
}

float Shape::GetLength() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    float length   = 0.f;
    auto  geometry = ComPolicy::Get<ID2D1Geometry>(this);
    if (geometry)
    {
        // no matter it failed or not
        geometry->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
    }
    return length;
#else
    return 0.0f;  // not supported
#endif
}

bool Shape::ComputePointAtLength(float length, Point& point, Vec2& tangent) const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto geometry = ComPolicy::Get<ID2D1Geometry>(this);
    if (geometry)
    {
        HRESULT hr = geometry->ComputePointAtLength(length, D2D1::Matrix3x2F::Identity(), DX::ConvertToPoint2F(&point),
                                                    DX::ConvertToPoint2F(&tangent));

        return SUCCEEDED(hr);
    }
    return false;
#else
    return false;  // not supported
#endif
}

float Shape::ComputeArea() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto geometry = ComPolicy::Get<ID2D1Geometry>(this);
    if (geometry)
    {
        float area = 0.f;
        // no matter it failed or not
        geometry->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
    }
    return 0.f;
#else
    return 0.0f;  // not supported
#endif
}

bool Shape::ContainsPoint(const Point& point, const Matrix3x2* transform) const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto geometry = ComPolicy::Get<ID2D1Geometry>(this);
    if (!geometry)
        return false;

    BOOL ret = 0;
    // no matter it failed or not
    geometry->FillContainsPoint(DX::ConvertToPoint2F(point), DX::ConvertToMatrix3x2F(transform),
                                D2D1_DEFAULT_FLATTENING_TOLERANCE, &ret);
    return !!ret;
#else
    return false;  // not supported
#endif
}

RefPtr<Shape> Shape::CreateLine(const Point& begin, const Point& end)
{
    RefPtr<Shape> output = MakePtr<Shape>();
    Renderer::GetInstance().CreateLineShape(*output, begin, end);
    return output;
}

RefPtr<Shape> Shape::CreateRect(const Rect& rect)
{
    RefPtr<Shape> output = MakePtr<Shape>();
    Renderer::GetInstance().CreateRectShape(*output, rect);
    return output;
}

RefPtr<Shape> Shape::CreateRoundedRect(const Rect& rect, const Vec2& radius)
{
    RefPtr<Shape> output = MakePtr<Shape>();
    Renderer::GetInstance().CreateRoundedRectShape(*output, rect, radius);
    return output;
}

RefPtr<Shape> Shape::CreateCircle(const Point& center, float radius)
{
    RefPtr<Shape> output = MakePtr<Shape>();
    Renderer::GetInstance().CreateEllipseShape(*output, center, Vec2{ radius, radius });
    return output;
}

RefPtr<Shape> Shape::CreateEllipse(const Point& center, const Vec2& radius)
{
    RefPtr<Shape> output = MakePtr<Shape>();
    Renderer::GetInstance().CreateEllipseShape(*output, center, radius);
    return output;
}

RefPtr<Shape> Shape::CreatePolygon(const Vector<Point>& vertices)
{
    if (vertices.size() > 1)
    {
        ShapeMaker maker;
        maker.BeginPath(vertices[0]);
        maker.AddLines(&vertices[1], vertices.size() - 1);
        maker.EndPath(true);
        return maker.GetShape();
    }
    return RefPtr<Shape>();
}

}  // namespace kiwano
