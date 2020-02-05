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
    if (!IsOpened())
    {
        path_geo_.reset();
        Renderer::Instance().CreateShapeSink(*this);

        win32::ThrowIfFailed(path_geo_->Open(&sink_));
    }
}

void ShapeSink::Close()
{
    if (IsOpened())
    {
        win32::ThrowIfFailed(sink_->Close());
        sink_.reset();
    }

    shape_ = new Shape;
    shape_->SetGeometry(path_geo_);
}

bool ShapeSink::IsOpened() const
{
    return sink_ != nullptr;
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

    if (input && input->IsValid())
    {
        ComPtr<ID2D1Geometry> geo = input->GetGeometry();
        win32::ThrowIfFailed(
            geo->Outline(DX::ConvertToMatrix3x2F(input_matrix), D2D1_DEFAULT_FLATTENING_TOLERANCE, sink_.get()));
    }
    return (*this);
}

ShapeSink& ShapeSink::BeginPath(Point const& begin_pos)
{
    if (!IsOpened())
    {
        Open();
    }

    sink_->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
    return (*this);
}

ShapeSink& ShapeSink::EndPath(bool closed)
{
    KGE_ASSERT(sink_);
    sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
    return (*this);
}

ShapeSink& ShapeSink::AddLine(Point const& point)
{
    KGE_ASSERT(sink_);
    sink_->AddLine(DX::ConvertToPoint2F(point));
    return (*this);
}

ShapeSink& ShapeSink::AddLines(Vector<Point> const& points)
{
    KGE_ASSERT(sink_);
    sink_->AddLines(reinterpret_cast<const D2D_POINT_2F*>(&points[0]), static_cast<uint32_t>(points.size()));
    return (*this);
}

ShapeSink& kiwano::ShapeSink::AddLines(const Point* points, size_t count)
{
    KGE_ASSERT(sink_);
    sink_->AddLines(reinterpret_cast<const D2D_POINT_2F*>(points), UINT32(count));
    return (*this);
}

ShapeSink& ShapeSink::AddBezier(Point const& point1, Point const& point2, Point const& point3)
{
    KGE_ASSERT(sink_);
    sink_->AddBezier(
        D2D1::BezierSegment(DX::ConvertToPoint2F(point1), DX::ConvertToPoint2F(point2), DX::ConvertToPoint2F(point3)));
    return (*this);
}

ShapeSink& ShapeSink::AddArc(Point const& point, Size const& radius, float rotation, bool clockwise, bool is_small)
{
    KGE_ASSERT(sink_);
    sink_->AddArc(D2D1::ArcSegment(DX::ConvertToPoint2F(point), DX::ConvertToSizeF(radius), rotation,
                                   clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
                                   is_small ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE));
    return (*this);
}

ShapeSink& ShapeSink::Combine(Shape const& geo_a, Shape const& geo_b, CombineMode mode, const Matrix3x2* matrix)
{
    if (!IsOpened())
    {
        Open();
    }

    ComPtr<ID2D1Geometry> geo_a_raw = geo_a.geo_;
    ComPtr<ID2D1Geometry> geo_b_raw = geo_b.geo_;
    win32::ThrowIfFailed(geo_a_raw->CombineWithGeometry(geo_b_raw.get(), D2D1_COMBINE_MODE(mode),
                                                        DX::ConvertToMatrix3x2F(matrix), sink_.get()));
    return (*this);
}

void ShapeSink::Clear()
{
    Close();

    path_geo_.reset();
}

}  // namespace kiwano
