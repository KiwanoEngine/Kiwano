// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "Canvas.h"
#include "render.h"
#include "logs.h"

namespace easy2d
{
	//-------------------------------------------------------
	// CanvasBrush
	//-------------------------------------------------------

	CanvasBrush::CanvasBrush()
		: render_target_(nullptr)
		, fill_brush_(nullptr)
		, line_brush_(nullptr)
		, stroke_style_(nullptr)
		, stroke_width_(1.0f)
		, stroke_(StrokeStyle::Miter)
	{
		auto graphics = devices::Graphics::Instance();
		render_target_ = graphics->GetRenderTarget();

		ThrowIfFailed(
			graphics->CreateSolidColorBrush(fill_brush_)
		);

		ThrowIfFailed(
			graphics->CreateSolidColorBrush(line_brush_)
		);

		this->SetStrokeStyle(stroke_);
	}

	CanvasBrush::~CanvasBrush()
	{
	}

	void CanvasBrush::SetLineColor(Color const& color)
	{
		line_brush_->SetColor(D2D_COLOR_F(color));
	}

	void CanvasBrush::SetFillColor(Color const& color)
	{
		fill_brush_->SetColor(D2D_COLOR_F(color));
	}

	void CanvasBrush::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void CanvasBrush::SetStrokeStyle(StrokeStyle stroke)
	{
		stroke_style_ = devices::Graphics::Instance()->GetStrokeStyle(stroke);
	}

	Color CanvasBrush::GetLineColor() const
	{
		return line_brush_->GetColor();
	}

	Color CanvasBrush::GetFillColor() const
	{
		return fill_brush_->GetColor();
	}

	float CanvasBrush::GetStrokeWidth() const
	{
		return stroke_width_;
	}

	StrokeStyle CanvasBrush::GetStrokeStyle() const
	{
		return stroke_;
	}

	void CanvasBrush::DrawLine(const Point & begin, const Point & end)
	{
		render_target_->DrawLine(
			D2D1::Point2F(begin.x, begin.y),
			D2D1::Point2F(end.x, end.y),
			line_brush_.Get(),
			stroke_width_,
			stroke_style_.Get()
		);
	}

	void CanvasBrush::DrawCircle(const Point & center, float radius)
	{
		render_target_->DrawEllipse(
			D2D1::Ellipse(
				D2D1::Point2F(
					center.x,
					center.y
				),
				radius,
				radius
			),
			line_brush_.Get(),
			stroke_width_,
			stroke_style_.Get()
		);
	}

	void CanvasBrush::DrawEllipse(const Point & center, float radius_x, float radius_y)
	{
		render_target_->DrawEllipse(
			D2D1::Ellipse(
				D2D1::Point2F(
					center.x,
					center.y
				),
				radius_x,
				radius_y
			),
			line_brush_.Get(),
			stroke_width_,
			stroke_style_.Get()
		);
	}

	void CanvasBrush::DrawRect(const Rect & rect)
	{
		render_target_->DrawRectangle(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.width,
				rect.origin.y + rect.size.height
			),
			line_brush_.Get(),
			stroke_width_,
			stroke_style_.Get()
		);
	}

	void CanvasBrush::DrawRoundedRect(const Rect & rect, float radius_x, float radius_y)
	{
		render_target_->DrawRoundedRectangle(
			D2D1::RoundedRect(
				D2D1::RectF(
					rect.origin.x,
					rect.origin.y,
					rect.origin.x + rect.size.width,
					rect.origin.y + rect.size.height
				),
				radius_x,
				radius_y
			),
			line_brush_.Get(),
			stroke_width_,
			stroke_style_.Get()
		);
	}

	void CanvasBrush::FillCircle(const Point & center, float radius)
	{
		render_target_->FillEllipse(
			D2D1::Ellipse(
				D2D1::Point2F(
					center.x,
					center.y
				),
				radius,
				radius
			),
			fill_brush_.Get()
		);
	}

	void CanvasBrush::FillEllipse(const Point & center, float radius_x, float radius_y)
	{
		render_target_->FillEllipse(
			D2D1::Ellipse(
				D2D1::Point2F(
					center.x,
					center.y
				),
				radius_x,
				radius_y
			),
			fill_brush_.Get()
		);
	}

	void CanvasBrush::FillRect(const Rect & rect)
	{
		render_target_->FillRectangle(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.width,
				rect.origin.y + rect.size.height
			),
			fill_brush_.Get()
		);
	}

	void CanvasBrush::FillRoundedRect(const Rect & rect, float radius_x, float radius_y)
	{
		render_target_->FillRoundedRectangle(
			D2D1::RoundedRect(
				D2D1::RectF(
					rect.origin.x,
					rect.origin.y,
					rect.origin.x + rect.size.width,
					rect.origin.y + rect.size.height
				),
				radius_x,
				radius_y
			),
			fill_brush_.Get()
		);
	}


	//-------------------------------------------------------
	// Canvas
	//-------------------------------------------------------

	Canvas::Canvas()
	{
	}

	Canvas::Canvas(float width, float height)
	{
		this->SetClipEnabled(true);
		this->SetWidth(width);
		this->SetHeight(height);
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::SetBrush(spCanvasBrush const & brush)
	{
		brush_ = brush;
	}

	void Canvas::OnDraw()
	{
		if (!brush_)
			brush_ = new CanvasBrush;

		OnDraw(*brush_);
	}
}