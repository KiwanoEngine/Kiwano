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

#include "Canvas.h"
#include "../base/Logger.h"
#include "../renderer/Renderer.h"

namespace kiwano
{
	Canvas::Canvas()
		: cache_expired_(false)
		, stroke_width_(1.0f)
	{
		auto ctx = Renderer::GetInstance()->GetD2DDeviceResources()->GetDeviceContext();

		ThrowIfFailed(
			ctx->CreateCompatibleRenderTarget(&render_target_)
		);

		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				D2D1::ColorF(0, 0, 0, 0),
				D2D1::BrushProperties(),
				&fill_brush_)
		);

		ThrowIfFailed(
			render_target_->CreateSolidColorBrush(
				D2D1::ColorF(Color::White),
				D2D1::BrushProperties(),
				&stroke_brush_)
		);

		ThrowIfFailed(
			ITextRenderer::Create(
				&text_renderer_,
				render_target_.get()
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

	void Canvas::OnRender(Renderer* renderer)
	{
		if (cache_expired_)
		{
			GetBitmap();
		}
		
		if (bitmap_cached_)
		{
			PrepareRender(renderer);

			Rect bitmap_rect(0.f, 0.f, bitmap_cached_->GetSize().width, bitmap_cached_->GetSize().height);
			renderer->DrawBitmap(
				bitmap_cached_,
				bitmap_rect,
				bitmap_rect
			);
		}
	}

	void Canvas::SetStrokeColor(Color const& color)
	{
		stroke_brush_->SetColor(DX::ConvertToColorF(color));
	}

	void Canvas::SetFillColor(Color const& color)
	{
		fill_brush_->SetColor(DX::ConvertToColorF(color));
	}

	void Canvas::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void Canvas::SetOutlineJoinStyle(StrokeStyle outline_join)
	{
		outline_join_style_ = Renderer::GetInstance()->GetD2DDeviceResources()->GetStrokeStyle(outline_join);
	}

	void Canvas::SetTextStyle(Font const& font, TextStyle const & text_style)
	{
		text_font_ = font;
		text_style_ = text_style;

		text_renderer_->SetTextStyle(
			1.f,
			DX::ConvertToColorF(text_style_.color),
			text_style_.outline,
			DX::ConvertToColorF(text_style_.outline_color),
			text_style_.outline_width,
			Renderer::GetInstance()->GetD2DDeviceResources()->GetStrokeStyle(text_style_.outline_stroke)
		);

		// clear text format
		text_format_ = nullptr;
	}

	Color Canvas::GetStrokeColor() const
	{
		auto color_f = stroke_brush_->GetColor();
		return Color{ color_f.r, color_f.g, color_f.b, color_f.a };
	}

	Color Canvas::GetFillColor() const
	{
		auto color_f = fill_brush_->GetColor();
		return Color{ color_f.r, color_f.g, color_f.b, color_f.a };
	}

	float Canvas::GetStrokeWidth() const
	{
		return stroke_width_;
	}

	void Canvas::SetBrushTransform(Matrix3x2 const & transform)
	{
		render_target_->SetTransform(DX::ConvertToMatrix3x2F(transform));
	}

	void Canvas::DrawLine(const Point & begin, const Point & end)
	{
		render_target_->DrawLine(
			D2D1::Point2F(begin.x, begin.y),
			D2D1::Point2F(end.x, end.y),
			stroke_brush_.get(),
			stroke_width_,
			outline_join_style_.get()
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
			stroke_brush_.get(),
			stroke_width_,
			outline_join_style_.get()
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
			stroke_brush_.get(),
			stroke_width_,
			outline_join_style_.get()
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
			stroke_brush_.get(),
			stroke_width_,
			outline_join_style_.get()
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
			stroke_brush_.get(),
			stroke_width_,
			outline_join_style_.get()
		);
		cache_expired_ = true;
	}

	void Canvas::DrawImage(ImagePtr image, float opacity)
	{
		if (image && image->GetBitmap())
		{
			render_target_->DrawBitmap(
				image->GetBitmap().get(),
				D2D1::RectF(0, 0, image->GetWidth(), image->GetHeight()),
				opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				D2D1::RectF(0, 0, image->GetWidth(), image->GetHeight())
			);
			cache_expired_ = true;
		}
	}

	void Canvas::DrawText(String const & text, Point const & point)
	{
		if (text.empty())
			return;

		if (!text_format_)
		{
			ThrowIfFailed(
				Renderer::GetInstance()->GetD2DDeviceResources()->CreateTextFormat(
					text_format_,
					text_font_,
					text_style_
				)
			);
		}

		ComPtr<IDWriteTextLayout> text_layout;
		Size layout_size;
		ThrowIfFailed(
			Renderer::GetInstance()->GetD2DDeviceResources()->CreateTextLayout(
				text_layout,
				layout_size,
				text,
				text_format_,
				text_style_
			)
		);

		ThrowIfFailed(
			text_layout->Draw(nullptr, text_renderer_.get(), point.x, point.y)
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
			fill_brush_.get()
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
			fill_brush_.get()
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
			fill_brush_.get()
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
			fill_brush_.get()
		);
		cache_expired_ = true;
	}

	void Canvas::BeginPath(Point const& begin_pos)
	{
		current_geometry_ = nullptr;

		ThrowIfFailed(
			Renderer::GetInstance()->GetD2DDeviceResources()->GetFactory()->CreatePathGeometry(&current_geometry_)
		);
		
		ThrowIfFailed(
			current_geometry_->Open(&current_sink_)
		);

		current_sink_->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
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
			current_sink_->AddLine(DX::ConvertToPoint2F(point));
	}

	void Canvas::AddLines(Vector<Point> const& points)
	{
		if (current_sink_ && !points.empty())
		{
			current_sink_->AddLines(
				reinterpret_cast<const D2D_POINT_2F*>(&points[0]),
				static_cast<UINT32>(points.size())
			);
		}
	}

	void Canvas::AddBezier(Point const & point1, Point const & point2, Point const & point3)
	{
		if (current_sink_)
		{
			current_sink_->AddBezier(
				D2D1::BezierSegment(
					DX::ConvertToPoint2F(point1),
					DX::ConvertToPoint2F(point2),
					DX::ConvertToPoint2F(point3)
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
					DX::ConvertToPoint2F(point),
					DX::ConvertToSizeF(radius),
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
			current_geometry_.get(),
			stroke_brush_.get(),
			stroke_width_,
			outline_join_style_.get()
		);
		cache_expired_ = true;
	}

	void Canvas::FillPath()
	{
		render_target_->FillGeometry(
			current_geometry_.get(),
			fill_brush_.get()
		);
		cache_expired_ = true;
	}

	void Canvas::Clear()
	{
		render_target_->Clear();
		cache_expired_ = true;
	}

	ImagePtr Canvas::ExportToImage() const
	{
		ImagePtr image = new Image(GetBitmap());
		return image;
	}

	ComPtr<ID2D1Bitmap> const& kiwano::Canvas::GetBitmap() const
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
