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

#include <kiwano/core/Logger.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{
GradientStop::GradientStop()
    : offset(0.f)
    , color()
{
}

GradientStop::GradientStop(float offset, Color color)
    : offset(offset)
    , color(color)
{
}

LinearGradientStyle::LinearGradientStyle(Point const& begin, Point const& end, Vector<GradientStop> const& stops,
                                         GradientExtendMode extend_mode)
    : begin(begin)
    , end(end)
    , stops(stops)
    , extend_mode(extend_mode)
{
}

RadialGradientStyle::RadialGradientStyle(Point const& center, Vec2 const& offset, Vec2 const& radius,
                                         Vector<GradientStop> const& stops, GradientExtendMode extend_mode)
    : center(center)
    , offset(offset)
    , radius(radius)
    , stops(stops)
    , extend_mode(extend_mode)
{
}

BrushPtr Brush::Create(Color const& color)
{
    BrushPtr ptr = new (std::nothrow) Brush;
    if (ptr)
    {
        ptr->SetColor(color);
    }
    return ptr;
}

BrushPtr Brush::Create(LinearGradientStyle const& style)
{
    BrushPtr ptr = new (std::nothrow) Brush;
    if (ptr)
    {
        ptr->SetStyle(style);
    }
    return ptr;
}

BrushPtr Brush::Create(RadialGradientStyle const& style)
{
    BrushPtr ptr = new (std::nothrow) Brush;
    if (ptr)
    {
        ptr->SetStyle(style);
    }
    return ptr;
}

Brush::Brush()
    : type_(Type::Unknown)
{
}

void Brush::SetColor(Color const& color)
{
    Renderer::GetInstance().CreateBrush(*this, color);
    type_ = Brush::Type::SolidColor;
}

void Brush::SetStyle(LinearGradientStyle const& style)
{
    Renderer::GetInstance().CreateBrush(*this, style);
    type_ = Brush::Type::LinearGradient;
}

void Brush::SetStyle(RadialGradientStyle const& style)
{
    Renderer::GetInstance().CreateBrush(*this, style);
    type_ = Brush::Type::RadialGradient;
}

}  // namespace kiwano
