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
#include "../base/time.h"
#include "Texture.h"
#include "Geometry.h"
#include "TextLayout.h"
#include "LayerArea.h"
#include "win32/TextRenderer.h"

namespace kiwano
{
	// 文字抗锯齿模式
	enum class TextAntialiasMode
	{
		Default,	// 系统默认
		ClearType,	// ClearType 抗锯齿
		GrayScale,	// 灰度抗锯齿
		None		// 不启用抗锯齿
	};


	// 渲染目标
	class KGE_API RenderTarget
		: public noncopyable
	{
	public:
		bool IsValid() const;

		void BeginDraw();

		void EndDraw();

		void CreateLayer(
			LayerArea& layer
		) const;

		void DrawGeometry(
			Geometry const& geometry,
			Color const& stroke_color,
			Float32 stroke_width,
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
			Float32 stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void DrawRectangle(
			Rect const& rect,
			Color const& stroke_color,
			Float32 stroke_width,
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
			Float32 stroke_width,
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
			Float32 stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		) const;

		void FillEllipse(
			Point const& center,
			Vec2 const& radius,
			Color const& fill_color
		) const;

		void DrawTexture(
			Texture const& texture,
			Rect const& src_rect,
			Rect const& dest_rect
		) const;

		void DrawTexture(
			Texture const& texture,
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
			LayerArea& layer
		);

		void PopLayer();

		void Clear();

		void Clear(
			Color const& clear_color
		);

		Float32 GetOpacity() const;

		Matrix3x2 GetGlobalTransform() const;

		void SetOpacity(
			Float32 opacity
		);

		void SetTransform(
			const Matrix3x2& matrix
		);

		void SetGlobalTransform(
			const Matrix3x2& matrix
		);

		void SetGlobalTransform(
			const Matrix3x2* matrix
		);

		// 设置抗锯齿模式
		void SetAntialiasMode(
			bool enabled
		);

		// 设置文字抗锯齿模式
		void SetTextAntialiasMode(
			TextAntialiasMode mode
		);

		// 检查边界是否在视区内
		bool CheckVisibility(
			Rect const& bounds,
			Matrix3x2 const& transform
		);

	public:
		struct Status
		{
			Int32 primitives;
			Time start;
			Duration duration;

			Status() : primitives(0) {}
		};

		void SetCollectingStatus(bool collecting);

		void IncreasePrimitivesCount() const;

		inline Status const&				GetStatus() const			{ return status_; }

		inline ComPtr<ID2D1RenderTarget>	GetRenderTarget() const		{ KGE_ASSERT(render_target_); return render_target_; }

		inline ComPtr<ITextRenderer>		GetTextRenderer() const		{ KGE_ASSERT(text_renderer_); return text_renderer_; }

		ComPtr<ID2D1StrokeStyle>			GetStrokeStyle(StrokeStyle style) const;

	public:
		RenderTarget();

		HRESULT CreateDeviceResources(
			ComPtr<ID2D1RenderTarget> rt,
			ComPtr<ID2DDeviceResources> dev_res
		);

		void DiscardDeviceResources();

	protected:
		Float32							opacity_;
		bool							antialias_;
		bool							fast_global_transform_;
		mutable bool					collecting_status_;
		mutable Status					status_;
		TextAntialiasMode				text_antialias_;
		ComPtr<ITextRenderer>			text_renderer_;
		ComPtr<ID2D1RenderTarget>		render_target_;
		ComPtr<ID2D1SolidColorBrush>	default_brush_;
		ComPtr<ID2D1Brush>				current_brush_;
		ComPtr<ID2DDeviceResources>		device_resources_;
		Matrix3x2						global_transform_;
	};


	// 位图渲染目标
	class KGE_API TextureRenderTarget
		: public RenderTarget
	{
	public:
		TextureRenderTarget();

		Texture GetOutput() const;
	};
}
