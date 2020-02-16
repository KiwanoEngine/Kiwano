// Copyright (c) 2016-2019 Kiwano - Nomango
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

#include <kiwano/render/Renderer.h>
#include <kiwano/render/StrokeStyle.h>

namespace kiwano
{

StrokeStylePtr StrokeStyle::Create(float width, CapStyle cap, LineJoinStyle line_join)
{
    return StrokeStyle::Create(width, cap, line_join, DashStyle::Solid);
}

StrokeStylePtr StrokeStyle::Create(float width, CapStyle cap, LineJoinStyle line_join, DashStyle dash,
                                   float dash_offset)
{
    StrokeStylePtr ptr = new (std::nothrow) StrokeStyle;
    if (ptr)
    {
        ptr->SetStrokeWidth(width);
        ptr->SetCapStyle(cap);
        ptr->SetLineJoinStyle(line_join);
        ptr->SetDashStyle(dash);
        ptr->SetDashOffset(dash_offset);
    }
    return ptr;
}

StrokeStylePtr StrokeStyle::Create(float width, CapStyle cap, LineJoinStyle line_join, const float* dash_array,
                                   size_t dash_size, float dash_offset)
{
    StrokeStylePtr ptr = new (std::nothrow) StrokeStyle;
    if (ptr)
    {
        ptr->SetStrokeWidth(width);
        ptr->SetCapStyle(cap);
        ptr->SetLineJoinStyle(line_join);
        ptr->SetDashStyle(dash_array, dash_size);
        ptr->SetDashOffset(dash_offset);
    }
    return ptr;
}

StrokeStyle::StrokeStyle()
    : cap_(CapStyle::Flat)
    , line_join_(LineJoinStyle::Miter)
    , dash_offset_(0.0f)
    , stroke_width_(1.0f)
{
}

void StrokeStyle::SetDashStyle(DashStyle dash_style)
{
    switch (dash_style)
    {
    case DashStyle::Solid:
    {
        dash_array_.clear();
        break;
    }

    case DashStyle::Dash:
    {
        dash_array_ = { 2, 2 };
        break;
    }

    case DashStyle::Dot:
    {
        dash_array_ = { 0, 2 };
        break;
    }

    case DashStyle::DashDot:
    {
        dash_array_ = { 2, 2, 0, 2 };
        break;
    }

    case DashStyle::DashDotDot:
    {
        dash_array_ = { 2, 2, 0, 2, 0, 2 };
        break;
    }

    default:
        break;
    }
}

void StrokeStyle::SetDashStyle(const Vector<float>& dash_array)
{
    dash_array_ = dash_array;
}

void StrokeStyle::SetDashStyle(const float* dash_array, size_t dash_size)
{
    if (dash_size == 0)
        dash_array_.clear();
    else
        dash_array_.assign(dash_array, dash_array + dash_size);
}

}  // namespace kiwano
