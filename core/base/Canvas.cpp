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

#include "base.h"
#include "Canvas.h"
#include "render.h"
#include "logs.h"

namespace easy2d
{
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// FIXME!!!
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

	Canvas::Canvas(float width, float height)
		: render_target_(nullptr)
		, fill_brush_(nullptr)
		, line_brush_(nullptr)
		, stroke_style_(nullptr)
		, stroke_width_(1.0f)
		, stroke_(StrokeStyle::Miter)
	{
		// render_target_ = render::D2D.HwndRenderTarget;
		render_target_->AddRef();

		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&fill_brush_
			)
		);

		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&line_brush_
			)
		);

		this->SetClipEnabled(true);
		this->SetWidth(width);
		this->SetHeight(height);
		this->SetStrokeStyle(stroke_);
	}

	Canvas::~Canvas()
	{
		SafeRelease(stroke_style_);
		SafeRelease(line_brush_);
		SafeRelease(fill_brush_);
		SafeRelease(render_target_);
	}

	void Canvas::SetLineColor(const Color & color)
	{
		line_brush_->SetColor(D2D_COLOR_F(color));
	}

	void Canvas::SetFillColor(const Color & color)
	{
		fill_brush_->SetColor(D2D_COLOR_F(color));
	}

	void Canvas::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void Canvas::SetStrokeStyle(StrokeStyle stroke)
	{
		SafeRelease(stroke_style_);

		stroke_style_ = devices::Graphics::Instance()->GetStrokeStyle(stroke);

		if (stroke_style_)
			stroke_style_->AddRef();
	}

	Color Canvas::GetLineColor() const
	{
		return line_brush_->GetColor();
	}

	Color Canvas::GetFillColor() const
	{
		return fill_brush_->GetColor();
	}

	float Canvas::GetStrokeWidth() const
	{
		return stroke_width_;
	}

	StrokeStyle Canvas::GetStrokeStyle() const
	{
		return stroke_;
	}

	void Canvas::DrawLine(const Point & begin, const Point & end)
	{
		render_target_->DrawLine(
			D2D1::Point2F(begin.x, begin.y),
			D2D1::Point2F(end.x, end.y),
			line_brush_,
			stroke_width_,
			stroke_style_
		);
	}

	void Canvas::DrawCircle(const Point & center, float radius)
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
			line_brush_,
			stroke_width_,
			stroke_style_
		);
	}

	void Canvas::DrawEllipse(const Point & center, float radius_x, float radius_y)
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
			line_brush_,
			stroke_width_,
			stroke_style_
		);
	}

	void Canvas::DrawRect(const Rect & rect)
	{
		render_target_->DrawRectangle(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.width,
				rect.origin.y + rect.size.height
			),
			line_brush_,
			stroke_width_,
			stroke_style_
		);
	}

	void Canvas::DrawRoundedRect(const Rect & rect, float radius_x, float radius_y)
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
			line_brush_,
			stroke_width_,
			stroke_style_
		);
	}

	void Canvas::FillCircle(const Point & center, float radius)
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
			fill_brush_
		);
	}

	void Canvas::FillEllipse(const Point & center, float radius_x, float radius_y)
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
			fill_brush_
		);
	}

	void Canvas::FillRect(const Rect & rect)
	{
		render_target_->FillRectangle(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.width,
				rect.origin.y + rect.size.height
			),
			fill_brush_
		);
	}

	void Canvas::FillRoundedRect(const Rect & rect, float radius_x, float radius_y)
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
			fill_brush_
		);
	}
}