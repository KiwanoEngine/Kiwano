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
		, fill_color_(0, 0, 0)
		, stroke_color_(Color::White)
		, stroke_style_(StrokeStyle::Miter)
	{
		Renderer::GetInstance()->CreateImageRenderTarget(rt_);
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::BeginDraw()
	{
		rt_.BeginDraw();
	}

	void Canvas::EndDraw()
	{
		rt_.EndDraw();
		cache_expired_ = true;
	}

	void Canvas::OnRender(RenderTarget* rt)
	{
		UpdateCache();
		
		if (image_cached_.IsValid())
		{
			PrepareRender(rt);

			Rect bitmap_rect(0.f, 0.f, image_cached_.GetWidth(), image_cached_.GetHeight());
			rt->DrawImage(image_cached_, bitmap_rect, bitmap_rect);
		}
	}

	void Canvas::SetStrokeColor(Color const& color)
	{
		stroke_color_ = color;
	}

	void Canvas::SetFillColor(Color const& color)
	{
		fill_color_ = color;
	}

	void Canvas::SetStrokeWidth(Float32 width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void Canvas::SetStrokeStyle(StrokeStyle stroke_style)
	{
		stroke_style_ = stroke_style;
	}

	void Canvas::SetTextFont(Font const& font)
	{
		text_font_ = font;
	}

	void Canvas::SetTextStyle(TextStyle const & text_style)
	{
		text_style_ = text_style;
	}

	void Canvas::SetBrushOpacity(Float32 opacity)
	{
		rt_.SetOpacity(opacity);
	}

	Color Canvas::GetStrokeColor() const
	{
		return stroke_color_;
	}

	Color Canvas::GetFillColor() const
	{
		return fill_color_;
	}

	Float32 Canvas::GetStrokeWidth() const
	{
		return stroke_width_;
	}

	Float32 Canvas::GetBrushOpacity() const
	{
		return rt_.GetOpacity();
	}

	void Canvas::SetBrushTransform(Transform const& transform)
	{
		rt_.SetTransform(transform.ToMatrix());
	}

	void Canvas::SetBrushTransform(Matrix3x2 const & transform)
	{
		rt_.SetTransform(transform);
	}

	void Canvas::PushLayerArea(LayerArea& area)
	{
		rt_.PushLayer(area);
	}

	void Canvas::PopLayerArea()
	{
		rt_.PopLayer();
	}

	void Canvas::PushClipRect(Rect const& clip_rect)
	{
		rt_.PushClipRect(clip_rect);
	}

	void Canvas::PopClipRect()
	{
		rt_.PopClipRect();
	}

	void Canvas::DrawLine(Point const& begin, Point const& end)
	{
		rt_.DrawLine(
			begin,
			end,
			stroke_color_,
			stroke_width_,
			stroke_style_
		);
		cache_expired_ = true;
	}

	void Canvas::DrawCircle(Point const& center, Float32 radius)
	{
		rt_.DrawEllipse(
			center,
			Vec2(radius, radius),
			stroke_color_,
			stroke_width_,
			stroke_style_
		);
		cache_expired_ = true;
	}

	void Canvas::DrawEllipse(Point const& center, Vec2 const& radius)
	{
		rt_.DrawEllipse(
			center,
			radius,
			stroke_color_,
			stroke_width_,
			stroke_style_
		);
		cache_expired_ = true;
	}

	void Canvas::DrawRect(Rect const& rect)
	{
		rt_.DrawRectangle(
			rect,
			stroke_color_,
			stroke_width_,
			stroke_style_
		);
		cache_expired_ = true;
	}

	void Canvas::DrawRoundedRect(Rect const& rect, Vec2 const& radius)
	{
		rt_.DrawRoundedRectangle(
			rect,
			radius,
			stroke_color_,
			stroke_width_,
			stroke_style_
		);
		cache_expired_ = true;
	}

	void Canvas::FillCircle(Point const& center, Float32 radius)
	{
		rt_.FillEllipse(
			center,
			Vec2(radius, radius),
			fill_color_
		);
		cache_expired_ = true;
	}

	void Canvas::FillEllipse(Point const& center, Vec2 const& radius)
	{
		rt_.FillEllipse(
			center,
			radius,
			fill_color_
		);
		cache_expired_ = true;
	}

	void Canvas::FillRect(Rect const& rect)
	{
		rt_.FillRectangle(
			rect,
			fill_color_
		);
		cache_expired_ = true;
	}

	void Canvas::FillRoundedRect(Rect const& rect, Vec2 const& radius)
	{
		rt_.FillRoundedRectangle(
			rect,
			radius,
			fill_color_
		);
		cache_expired_ = true;
	}

	void Canvas::DrawImage(Image const& image, const Rect* src_rect, const Rect* dest_rect)
	{
		if (image.IsValid())
		{
			rt_.DrawImage(image, src_rect, dest_rect);
			cache_expired_ = true;
		}
	}

	void Canvas::DrawText(String const& text, Point const& point)
	{
		if (text.empty())
			return;

		TextLayout layout(text, text_font_, text_style_);

		rt_.DrawTextLayout(layout, point);
	}

	void Canvas::BeginPath(Point const& begin_pos)
	{
		geo_sink_.BeginPath(begin_pos);
	}

	void Canvas::EndPath(bool closed)
	{
		geo_sink_.EndPath(closed);
	}

	void Canvas::AddLine(Point const & point)
	{
		geo_sink_.AddLine(point);
	}

	void Canvas::AddLines(Vector<Point> const& points)
	{
		geo_sink_.AddLines(points);
	}

	void Canvas::AddBezier(Point const & point1, Point const & point2, Point const & point3)
	{
		geo_sink_.AddBezier(point1, point2, point3);
	}

	void Canvas::AddArc(Point const & point, Point const & radius, Float32 rotation, bool clockwise, bool is_small)
	{
		geo_sink_.AddArc(point, radius, rotation, clockwise, is_small);
	}

	void Canvas::StrokePath()
	{
		rt_.DrawGeometry(
			geo_sink_.GetGeometry(),
			stroke_color_,
			stroke_width_,
			stroke_style_
		);
		cache_expired_ = true;
	}

	void Canvas::FillPath()
	{
		rt_.FillGeometry(
			geo_sink_.GetGeometry(),
			fill_color_
		);
		cache_expired_ = true;
	}

	void Canvas::Clear()
	{
		rt_.Clear();
		cache_expired_ = true;
	}

	void Canvas::Clear(Color const& clear_color)
	{
		rt_.Clear(clear_color);
		cache_expired_ = true;
	}

	Image Canvas::ExportToImage() const
	{
		UpdateCache();
		return image_cached_;
	}

	void Canvas::UpdateCache() const
	{
		if (cache_expired_)
		{
			image_cached_ = rt_.GetOutput();
			cache_expired_ = false;
		}
	}

}
