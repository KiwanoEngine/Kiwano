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

#include <kiwano/render/ShapeSink.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

ShapeSink::ShapeSink() {}

ShapeSink::~ShapeSink()
{
    Close();
}

void ShapeSink::Open()
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (!IsOpened())
    {
        path_geo_.reset();
        Renderer::GetInstance().CreateShapeSink(*this);

        ThrowIfFailed(path_geo_->Open(&sink_), "Open ID2D1GeometrySink failed");
    }
#else
    return;  // not supported
#endif
}

void ShapeSink::Close()
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (IsOpened())
    {
        ThrowIfFailed(sink_->Close(), "Close ID2D1GeometrySink failed");
        sink_.reset();
    }

    shape_ = new Shape;
    shape_->SetGeometry(path_geo_);
#else
    return;  // not supported
#endif
}

bool ShapeSink::IsOpened() const
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    return sink_ != nullptr;
#else
    return false;  // not supported
#endif
}

ShapePtr ShapeSink::GetShape()
{
    Close();
    return shape_;
}

ShapeSink& ShapeSink::AddShape(ShapePtr input, const Matrix3x2* input_matrix)
{
    if (!IsOpened())
    {
        Open();
    }

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (input && input->IsValid())
    {
        ComPtr<ID2D1Geometry> geo = input->GetGeometry();

        HRESULT hr =
            geo->Outline(DX::ConvertToMatrix3x2F(input_matrix), D2D1_DEFAULT_FLATTENING_TOLERANCE, sink_.get());
        ThrowIfFailed(hr, "Get outline of ID2D1Geometry failed");
    }
    return (*this);
#else
    return (*this);  // not supported
#endif
}

ShapeSink& ShapeSink::BeginPath(Point const& begin_pos)
{
    if (!IsOpened())
    {
        Open();
    }

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    sink_->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& ShapeSink::EndPath(bool closed)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(sink_);
    sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& ShapeSink::AddLine(Point const& point)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(sink_);
    sink_->AddLine(DX::ConvertToPoint2F(point));
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& ShapeSink::AddLines(Vector<Point> const& points)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(sink_);
    sink_->AddLines(reinterpret_cast<const D2D_POINT_2F*>(&points[0]), static_cast<uint32_t>(points.size()));
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& kiwano::ShapeSink::AddLines(const Point* points, size_t count)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(sink_);
    sink_->AddLines(reinterpret_cast<const D2D_POINT_2F*>(points), UINT32(count));
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& ShapeSink::AddBezier(Point const& point1, Point const& point2, Point const& point3)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(sink_);
    sink_->AddBezier(
        D2D1::BezierSegment(DX::ConvertToPoint2F(point1), DX::ConvertToPoint2F(point2), DX::ConvertToPoint2F(point3)));
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& ShapeSink::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    KGE_ASSERT(sink_);
    sink_->AddArc(D2D1::ArcSegment(DX::ConvertToPoint2F(point), DX::ConvertToSizeF(radius), rotation,
                                   clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
                                   is_small ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE));
#else
    // not supported
#endif
    return (*this);
}

ShapeSink& ShapeSink::Combine(ShapePtr shape_a, ShapePtr shape_b, CombineMode mode, const Matrix3x2* matrix)
{
    if (!IsOpened())
    {
        Open();
    }

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (shape_a && shape_b)
    {
        ComPtr<ID2D1Geometry> geo_a_raw = shape_a->geo_;
        ComPtr<ID2D1Geometry> geo_b_raw = shape_b->geo_;

        HRESULT hr = geo_a_raw->CombineWithGeometry(geo_b_raw.get(), D2D1_COMBINE_MODE(mode),
                                                    DX::ConvertToMatrix3x2F(matrix), sink_.get());
        ThrowIfFailed(hr, "Combine ID2D1Geometry failed");
    }
#else
    // not supported
#endif
    return (*this);
}

void ShapeSink::Clear()
{
    Close();

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    path_geo_.reset();
#else
    // not supported
#endif
}

}  // namespace kiwano
