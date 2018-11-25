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
#include "Image.h"
#include "Geometry.h"
#include "Factory.h"

namespace easy2d
{
	Canvas::Canvas()
		: cache_expired_(false)
		, stroke_width_(1.0f)
	{
		ThrowIfFailed(
			Graphics::Instance()->CreateBitmapRenderTarget(render_target_)
		);

		auto properties = D2D1::BrushProperties();
		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				Color(Color::White, 0.f),
				properties,
				&fill_brush_)
		);

		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				Color(Color::White),
				properties,
				&stroke_brush_)
		);

		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				Color(Color::White),
				properties,
				&text_brush_)
		);

		ThrowIfFailed(
			Factory::Instance()->CreateTextRenderer(
				text_renderer_,
				render_target_,
				text_brush_
			)
		);

		SetTextStyle(Font{}, TextStyle{});
	}

	Canvas::Canvas(float width, float height)
		: Canvas()
	{
		this->SetSize(width, height);
	}

	Canvas::Canvas(Size const & size)
		: Canvas(size.x, size.y)
	{
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::BeginDraw()
	{
		render_target_->BeginDraw();
	}

	void Canvas::EndDraw()
	{
		ThrowIfFailed(
			render_target_->EndDraw()
		);
		cache_expired_ = true;
	}

	void Canvas::OnRender()
	{
		if (cache_expired_)
		{
			GetBitmap();
		}
		
		if (bitmap_cached_)
		{
			Graphics::Instance()->DrawBitmap(bitmap_cached_);
		}
	}

	void Canvas::SetStrokeColor(Color const& color)
	{
		stroke_brush_->SetColor(color);
	}

	void Canvas::SetFillColor(Color const& color)
	{
		fill_brush_->SetColor(color);
	}

	void Canvas::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void Canvas::SetOutlineJoinStyle(StrokeStyle outline_join)
	{
		outline_join_style_ = Factory::Instance()->GetStrokeStyle(outline_join);
	}

	void Canvas::SetTextStyle(Font const& font, TextStyle const & text_style)
	{
		text_font_ = font;
		text_style_ = text_style;

		text_renderer_->SetTextStyle(
			text_style_.color,
			text_style_.outline,
			text_style_.outline_color,
			text_style_.outline_width,
			Factory::Instance()->GetStrokeStyle(text_style_.outline_stroke).Get()
		);
	}

	Color Canvas::GetStrokeColor() const
	{
		return stroke_brush_->GetColor();
	}

	Color Canvas::GetFillColor() const
	{
		return fill_brush_->GetColor();
	}

	float Canvas::GetStrokeWidth() const
	{
		return stroke_width_;
	}

	void Canvas::SetBrushTransform(Matrix const & transform)
	{
		render_target_->SetTransform(transform);
	}

	void Canvas::DrawLine(const Point & begin, const Point & end)
	{
		render_target_->DrawLine(
			D2D1::Point2F(begin.x, begin.y),
			D2D1::Point2F(end.x, end.y),
			stroke_brush_.Get(),
			stroke_width_,
			outline_join_style_.Get()
		);
		cache_expired_ = true;
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
			stroke_brush_.Get(),
			stroke_width_,
			outline_join_style_.Get()
		);
		cache_expired_ = true;
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
			stroke_brush_.Get(),
			stroke_width_,
			outline_join_style_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::DrawRect(const Rect & rect)
	{
		render_target_->DrawRectangle(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.x,
				rect.origin.y + rect.size.y
			),
			stroke_brush_.Get(),
			stroke_width_,
			outline_join_style_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::DrawRoundedRect(const Rect & rect, float radius_x, float radius_y)
	{
		render_target_->DrawRoundedRectangle(
			D2D1::RoundedRect(
				D2D1::RectF(
					rect.origin.x,
					rect.origin.y,
					rect.origin.x + rect.size.x,
					rect.origin.y + rect.size.y
				),
				radius_x,
				radius_y
			),
			stroke_brush_.Get(),
			stroke_width_,
			outline_join_style_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::DrawImage(spImage const & image, float opacity)
	{
		if (image && image->GetBitmap())
		{
			render_target_->DrawBitmap(
				image->GetBitmap().Get(),
				Rect{ Point{}, image->GetSize() },
				opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				image->GetCropRect()
			);
			cache_expired_ = true;
		}
	}

	void Canvas::DrawText(std::wstring const & text, Point const & point)
	{
		if (text.empty())
			return;

		cpTextFormat text_format;
		ThrowIfFailed(
			Factory::Instance()->CreateTextFormat(
				text_format,
				text_font_,
				text_style_
			)
		);

		cpTextLayout text_layout;
		Size layout_size;
		ThrowIfFailed(
			Factory::Instance()->CreateTextLayout(
				text_layout,
				layout_size,
				text,
				text_format,
				text_style_
			)
		);

		ThrowIfFailed(
			text_layout->Draw(nullptr, text_renderer_.Get(), point.x, point.y)
		);
	}

	void Canvas::DrawGeometry(spGeometry const & geo)
	{
		if (geo && geo->geo_)
		{
			render_target_->DrawGeometry(
				geo->geo_.Get(),
				stroke_brush_.Get(),
				stroke_width_,
				outline_join_style_.Get()
			);
			cache_expired_ = true;
		}
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
			fill_brush_.Get()
		);
		cache_expired_ = true;
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
			fill_brush_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::FillRect(const Rect & rect)
	{
		render_target_->FillRectangle(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.x,
				rect.origin.y + rect.size.y
			),
			fill_brush_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::FillRoundedRect(const Rect & rect, float radius_x, float radius_y)
	{
		render_target_->FillRoundedRectangle(
			D2D1::RoundedRect(
				D2D1::RectF(
					rect.origin.x,
					rect.origin.y,
					rect.origin.x + rect.size.x,
					rect.origin.y + rect.size.y
				),
				radius_x,
				radius_y
			),
			fill_brush_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::FillGeometry(spGeometry const & geo)
	{
		if (geo && geo->geo_)
		{
			render_target_->FillGeometry(
				geo->geo_.Get(),
				fill_brush_.Get()
			);
			cache_expired_ = true;
		}
	}

	void Canvas::BeginPath(Point const& begin_pos)
	{
		current_geometry_ = nullptr;

		ThrowIfFailed(
			Factory::Instance()->CreatePathGeometry(current_geometry_)
		);
		
		ThrowIfFailed(
			current_geometry_->Open(&current_sink_)
		);

		current_sink_->BeginFigure(begin_pos, D2D1_FIGURE_BEGIN_FILLED);
	}

	void Canvas::EndPath(bool closed)
	{
		if (current_sink_)
		{
			current_sink_->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
			ThrowIfFailed(
				current_sink_->Close()
			);
			current_sink_ = nullptr;
		}
	}

	void Canvas::AddLine(Point const & point)
	{
		if (current_sink_)
			current_sink_->AddLine(point);
	}

	void Canvas::AddLines(std::vector<Point> const& points)
	{
		if (current_sink_)
		{
			if (!points.empty())
			{
				size_t size = points.size();
				std::vector<D2D1_POINT_2F> d2d_points(size);
				for (size_t i = 0; i < size; ++i)
				{
					d2d_points[i] = points[i];
				}

				current_sink_->AddLines(
					&d2d_points[0],
					static_cast<UINT32>(size)
				);
			}
		}
	}

	void Canvas::AddBezier(Point const & point1, Point const & point2, Point const & point3)
	{
		if (current_sink_)
		{
			current_sink_->AddBezier(
				D2D1::BezierSegment(
					point1,
					point2,
					point3
				)
			);
		}
	}

	void Canvas::AddArc(Point const & point, Point const & radius, float rotation, bool clockwise, bool is_small)
	{
		if (current_sink_)
		{
			current_sink_->AddArc(
				D2D1::ArcSegment(
					point,
					D2D1_SIZE_F{ radius.x, radius.y },
					rotation,
					clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
					is_small ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE
				)
			);
		}
	}

	void Canvas::StrokePath()
	{
		render_target_->DrawGeometry(
			current_geometry_.Get(),
			stroke_brush_.Get(),
			stroke_width_,
			outline_join_style_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::FillPath()
	{
		render_target_->FillGeometry(
			current_geometry_.Get(),
			fill_brush_.Get()
		);
		cache_expired_ = true;
	}

	void Canvas::Clear()
	{
		render_target_->Clear();
		cache_expired_ = true;
	}

	spImage Canvas::ExportToImage() const
	{
		auto image = new Image(GetBitmap());
		image->Crop(Rect(Point{}, this->GetSize()));
		return image;
	}

	cpBitmap const& easy2d::Canvas::GetBitmap() const
	{
		if (cache_expired_)
		{
			bitmap_cached_ = nullptr;
			ThrowIfFailed(
				render_target_->GetBitmap(&bitmap_cached_)
			);
			cache_expired_ = false;
		}
		return bitmap_cached_;
	}

}