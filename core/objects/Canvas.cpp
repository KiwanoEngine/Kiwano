#include "..\e2dobject.h"
#include "..\e2dmodule.h"

e2d::Canvas::Canvas(float width, float height)
	: render_target_(nullptr)
	, fill_brush_(nullptr)
	, line_brush_(nullptr)
	, stroke_style_(nullptr)
	, stroke_width_(1.0f)
	, stroke_(Stroke::Miter)
{
	render_target_ = Graphics::Get()->GetRenderTarget();
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

e2d::Canvas::~Canvas()
{
	SafeRelease(line_brush_);
	SafeRelease(fill_brush_);
	SafeRelease(render_target_);
}

void e2d::Canvas::SetLineColor(const Color & color)
{
	line_brush_->SetColor(D2D_COLOR_F(color));
}

void e2d::Canvas::SetFillColor(const Color & color)
{
	fill_brush_->SetColor(D2D_COLOR_F(color));
}

void e2d::Canvas::SetStrokeWidth(float width)
{
	stroke_width_ = std::max(width, 0.f);
}

void e2d::Canvas::SetStrokeStyle(Stroke strokeStyle)
{
	switch (strokeStyle)
	{
	case e2d::Stroke::Miter:
		stroke_style_ = Graphics::GetMiterStrokeStyle();
		break;
	case e2d::Stroke::Bevel:
		stroke_style_ = Graphics::GetBevelStrokeStyle();
		break;
	case e2d::Stroke::Round:
		stroke_style_ = Graphics::GetRoundStrokeStyle();
		break;
	}
}

e2d::Color e2d::Canvas::GetLineColor() const
{
	return line_brush_->GetColor();
}

e2d::Color e2d::Canvas::GetFillColor() const
{
	return fill_brush_->GetColor();
}

float e2d::Canvas::GetStrokeWidth() const
{
	return stroke_width_;
}

e2d::Stroke e2d::Canvas::GetStrokeStyle() const
{
	return stroke_;
}

void e2d::Canvas::DrawLine(const Point & begin, const Point & end)
{
	render_target_->DrawLine(
		D2D1::Point2F(begin.x, begin.y),
		D2D1::Point2F(end.x, end.y),
		line_brush_,
		stroke_width_,
		stroke_style_
	);
}

void e2d::Canvas::DrawCircle(const Point & center, float radius)
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

void e2d::Canvas::DrawEllipse(const Point & center, float radius_x, float radius_y)
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

void e2d::Canvas::DrawRect(const Rect & rect)
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

void e2d::Canvas::DrawRoundedRect(const Rect & rect, float radius_x, float radius_y)
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

void e2d::Canvas::FillCircle(const Point & center, float radius)
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

void e2d::Canvas::FillEllipse(const Point & center, float radius_x, float radius_y)
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

void e2d::Canvas::FillRect(const Rect & rect)
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

void e2d::Canvas::FillRoundedRect(const Rect & rect, float radius_x, float radius_y)
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
