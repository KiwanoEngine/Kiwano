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

#include <kiwano/utils/Logger.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Renderer.h>

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/helper.h>
#endif

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

LinearGradientStyle::LinearGradientStyle()
    : extend_mode(GradientExtendMode::Clamp)
{
}

LinearGradientStyle::LinearGradientStyle(const Point& begin, const Point& end, const Vector<GradientStop>& stops,
                                         GradientExtendMode extend_mode)
    : begin(begin)
    , end(end)
    , stops(stops)
    , extend_mode(extend_mode)
{
}

RadialGradientStyle::RadialGradientStyle()
    : extend_mode(GradientExtendMode::Clamp)
{
}

RadialGradientStyle::RadialGradientStyle(const Point& center, const Vec2& offset, const Vec2& radius,
                                         const Vector<GradientStop>& stops, GradientExtendMode extend_mode)
    : center(center)
    , offset(offset)
    , radius(radius)
    , stops(stops)
    , extend_mode(extend_mode)
{
}

Brush::Brush(const Color& color)
    : Brush()
{
    SetColor(color);
}

Brush::Brush(const LinearGradientStyle& style)
    : Brush()
{
    SetStyle(style);
}

Brush::Brush(const RadialGradientStyle& style)
    : Brush()
{
    SetStyle(style);
}

Brush::Brush()
    : type_(Type::None)
{
}

void Brush::SetColor(const Color& color)
{
    Renderer::GetInstance().CreateBrush(*this, color);
    type_ = Brush::Type::SolidColor;
}

void Brush::SetStyle(const LinearGradientStyle& style)
{
    Renderer::GetInstance().CreateBrush(*this, style);
    type_ = Brush::Type::LinearGradient;
}

void Brush::SetStyle(const RadialGradientStyle& style)
{
    Renderer::GetInstance().CreateBrush(*this, style);
    type_ = Brush::Type::RadialGradient;
}

void Brush::SetImage(RefPtr<Image> image, const Rect& src_rect)
{
    Renderer::GetInstance().CreateBrush(*this, image, src_rect);
    type_ = Brush::Type::Image;
}

void Brush::SetBitmap(RefPtr<Bitmap> bitmap)
{
    SetImage(bitmap, Rect(Point(), bitmap->GetSize()));
}

void Brush::SetTransform(const Transform& transform)
{
    SetTransform(transform.ToMatrix());
}

void Brush::SetTransform(const Matrix3x2& transform)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto native = ComPolicy::Get<ID2D1Brush>(this);
    KGE_ASSERT(native);

    if (native)
    {
        native->SetTransform(DX::ConvertToMatrix3x2F(transform));
    }
#else
    // not supported
#endif
}

}  // namespace kiwano
