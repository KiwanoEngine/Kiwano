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

#pragma once
#include "D2DDeviceResources.h"
#include "Image.h"
#include "Geometry.h"
#include "TextLayout.h"
#include "TextRenderer.h"

namespace kiwano
{
	// 渲染目标
	class KGE_API RenderTarget
		: public noncopyable
	{
	public:
		void BeginDraw();

		void EndDraw();

		void CreateLayer(
			ComPtr<ID2D1Layer>& layer
		) const;

		void DrawGeometry(
			Geometry const& geometry,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void FillGeometry(
			Geometry const& geometry,
			Color const& fill_color
		) const;

		void DrawLine(
			Point const& point1,
			Point const& point2,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void DrawRectangle(
			Rect const& rect,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void FillRectangle(
			Rect const& rect,
			Color const& fill_color
		) const;

		void DrawRoundedRectangle(
			Rect const& rect,
			Vec2 const& radius,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void FillRoundedRectangle(
			Rect const& rect,
			Vec2 const& radius,
			Color const& fill_color
		) const;

		void DrawEllipse(
			Point const& center,
			Vec2 const& radius,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void FillEllipse(
			Point const& center,
			Vec2 const& radius,
			Color const& fill_color
		) const;

		void DrawImage(
			Image const& image,
			Rect const& src_rect,
			Rect const& dest_rect
		) const;

		void DrawImage(
			Image const& image,
			const Rect* src_rect = nullptr,
			const Rect* dest_rect = nullptr
		) const;

		void DrawTextLayout(
			TextLayout const& layout,
			Point const& offset = Point{}
		) const;

		void PushClipRect(
			Rect const& clip_rect
		);

		void PopClipRect();

		void PushLayer(
			ComPtr<ID2D1Layer> const& layer,
			LayerProperties const& properties
		);

		void PopLayer();

		void Clear();

		void Clear(
			Color const& clear_color
		);

		float GetOpacity() const;

		void SetOpacity(
			float opacity
		);

		void SetTransform(
			const Matrix3x2& matrix
		);

		// 设置抗锯齿模式
		void SetAntialiasMode(
			bool enabled
		);

		// 设置文字抗锯齿模式
		void SetTextAntialiasMode(
			TextAntialias mode
		);

	public:
		struct Status
		{
			int primitives;
			Time start;
			Duration duration;
		};

		void SetCollectingStatus(bool collecting);

		void IncreasePrimitivesCount() const;

		inline Status const& GetStatus() const { return status_; }

		inline ComPtr<ID2D1RenderTarget> GetRenderTarget() const { return render_target_; }

	public:
		RenderTarget();

		HRESULT InitDeviceResources(
			ComPtr<ID2D1RenderTarget> rt,
			ComPtr<ID2DDeviceResources> dev_res
		);

		bool IsValid() const;

	protected:
		float							opacity_;
		bool							antialias_;
		mutable bool					collecting_status_;
		mutable Status					status_;
		TextAntialias					text_antialias_;
		ComPtr<ID2D1RenderTarget>		render_target_;
		ComPtr<ITextRenderer>			text_renderer_;
		ComPtr<ID2D1SolidColorBrush>	solid_color_brush_;
		ComPtr<ID2DDeviceResources>		d2d_res_;
	};


	// 位图渲染目标
	class KGE_API ImageRenderTarget
		: public RenderTarget
	{
	public:
		ImageRenderTarget();

		void GetOutput(Image& output) const;
	};
}
