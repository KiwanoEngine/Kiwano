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

#include "Brush.h"
#include "Renderer.h"
#include "../base/Logger.h"

namespace kiwano
{
	SolidColorStyle::SolidColorStyle(Color const& color)
		: color(color)
	{
	}

	LinearGradientStyle::LinearGradientStyle(Point const& begin, Point const& end, Vector<GradientStop> const& stops, GradientExtendMode extend_mode)
		: begin(begin)
		, end(end)
		, stops(stops)
		, extend_mode(extend_mode)
	{
	}

	RadialGradientStyle::RadialGradientStyle(Point const& center, Vec2 const& offset, Vec2 const& radius, Vector<GradientStop> const& stops, GradientExtendMode extend_mode)
		: center(center)
		, offset(offset)
		, radius(radius)
		, stops(stops)
		, extend_mode(extend_mode)
	{
	}

	Brush::Brush()
		: opacity_(1.f)
		, type_(Type::Unknown)
	{
	}

	Brush::Brush(Color const& color)
		: Brush()
	{
		SetColor(color);
	}

	Brush::Brush(SolidColorStyle const& style)
		: Brush()
	{
		SetStyle(style);
	}

	Brush::Brush(LinearGradientStyle const& style)
		: Brush()
	{
		SetStyle(style);
	}

	Brush::Brush(RadialGradientStyle const& style)
		: Brush()
	{
		SetStyle(style);
	}

	bool Brush::IsValid() const
	{
		return raw_ != nullptr;
	}

	Brush::Brush(ComPtr<ID2D1Brush> brush)
		: Brush()
	{
		SetBrush(brush);
	}

	float Brush::GetOpacity() const
	{
		return opacity_;
	}

	void Brush::SetOpacity(float opacity)
	{
		opacity_ = opacity;
		if (raw_)
		{
			raw_->SetOpacity(opacity);
		}
	}

	void Brush::SetStyle(SolidColorStyle const& style)
	{
		if (type_ == Type::SolidColor && raw_)
		{
			auto solid_brush = dynamic_cast<ID2D1SolidColorBrush*>(raw_.get());
			KGE_ASSERT(solid_brush != nullptr);
			solid_brush->SetColor(DX::ConvertToColorF(style.color));
		}
		else
		{
			Renderer::GetInstance()->CreateSolidBrush(*this, style.color);
			type_ = Type::SolidColor;
		}
	}

	void Brush::SetStyle(LinearGradientStyle const& style)
	{
		Renderer::GetInstance()->CreateLinearGradientBrush(*this, style.begin, style.end, style.stops, style.extend_mode);
		type_ = Type::LinearGradient;
	}

	void Brush::SetStyle(RadialGradientStyle const& style)
	{
		Renderer::GetInstance()->CreateRadialGradientBrush(*this, style.center, style.offset, style.radius, style.stops, style.extend_mode);
		type_ = Type::RadialGradient;
	}

	void Brush::SetBrush(ComPtr<ID2D1Brush> const& brush)
	{
		type_ = Type::Unknown;
		raw_ = brush;

		if (raw_)
		{
			raw_->SetOpacity(opacity_);
		}
	}

}
