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
#include <kiwano/core/time.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/win32/ComPtr.hpp>
#include <kiwano/renderer/Brush.h>
#include <kiwano/renderer/Texture.h>
#include <kiwano/renderer/Geometry.h>
#include <kiwano/renderer/TextLayout.h>
#include <kiwano/renderer/LayerArea.h>
#include <kiwano/renderer/win32/TextRenderer.h>

namespace kiwano
{
	class Renderer;

	KGE_DECLARE_SMART_PTR(RenderTarget);
	KGE_DECLARE_SMART_PTR(TextureRenderTarget);

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
		: public ObjectBase
	{
		friend class Renderer;

	public:
		bool IsValid() const;

		void BeginDraw();

		void EndDraw();

		void DrawGeometry(
			Geometry const& geometry,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void FillGeometry(
			Geometry const& geometry
		);

		void DrawLine(
			Point const& point1,
			Point const& point2,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void DrawRectangle(
			Rect const& rect,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void FillRectangle(
			Rect const& rect
		);

		void DrawRoundedRectangle(
			Rect const& rect,
			Vec2 const& radius,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void FillRoundedRectangle(
			Rect const& rect,
			Vec2 const& radius
		);

		void DrawEllipse(
			Point const& center,
			Vec2 const& radius,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void FillEllipse(
			Point const& center,
			Vec2 const& radius
		);

		void DrawTexture(
			Texture const& texture,
			Rect const& src_rect,
			Rect const& dest_rect,
			float opacity = 1.0f
		);

		void DrawTexture(
			Texture const& texture,
			const Rect* src_rect = nullptr,
			const Rect* dest_rect = nullptr,
			float opacity = 1.0f
		);

		void DrawTextLayout(
			TextLayout const& layout,
			Point const& offset = Point{}
		);

		void CreateTexture(
			Texture& texture,
			math::Vec2T<uint32_t> size,
			D2D1_PIXEL_FORMAT format
		);

		void PushClipRect(Rect const& clip_rect);

		void PopClipRect();

		void PushLayer(LayerArea& layer);

		void PopLayer();

		void Clear();

		void Clear(Color const& clear_color);

		BrushPtr GetCurrentBrush() const;

		Matrix3x2 GetGlobalTransform() const;

		void SetCurrentBrush(
			BrushPtr brush
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

		void Resize(
			Size const& size
		);

	public:
		struct Status
		{
			int primitives;
			Time start;
			Duration duration;

			Status() : primitives(0) {}
		};

		void SetCollectingStatus(bool collecting);

		void IncreasePrimitivesCount() const;

		inline Status const&				GetStatus() const						{ return status_; }

	protected:
		RenderTarget();

		inline ComPtr<ID2D1RenderTarget>	GetRenderTarget() const					{ KGE_ASSERT(render_target_); return render_target_; }

		inline ComPtr<ITextRenderer>		GetTextRenderer() const					{ KGE_ASSERT(text_renderer_); return text_renderer_; }

		ComPtr<ID2D1StrokeStyle>			GetStrokeStyle(StrokeStyle style);

	private:
		HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> rt);

		void DiscardDeviceResources();

	private:
		bool							antialias_;
		bool							fast_global_transform_;
		TextAntialiasMode				text_antialias_;
		ComPtr<ITextRenderer>			text_renderer_;
		ComPtr<ID2D1RenderTarget>		render_target_;
		ComPtr<ID2D1StrokeStyle>		miter_stroke_style_;
		ComPtr<ID2D1StrokeStyle>		bevel_stroke_style_;
		ComPtr<ID2D1StrokeStyle>		round_stroke_style_;
		BrushPtr						current_brush_;
		Rect							visible_size_;
		Matrix3x2						global_transform_;

		mutable bool					collecting_status_;
		mutable Status					status_;
	};


	/// \~chinese
	/// @brief 纹理渲染目标
	/// @details 纹理渲染目标将渲染输出到一个纹理对象中
	class KGE_API TextureRenderTarget
		: public RenderTarget
	{
		friend class Renderer;

	public:
		/// \~chinese
		/// @brief 是否有效
		bool IsValid() const;

		/// \~chinese
		/// @brief 获取渲染输出
		bool GetOutput(Texture& texture);

	private:
		TextureRenderTarget();

		ComPtr<ID2D1BitmapRenderTarget> GetBitmapRenderTarget() const;

		void SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> rt);

	private:
		ComPtr<ID2D1BitmapRenderTarget> bitmap_rt_;
	};


	inline BrushPtr RenderTarget::GetCurrentBrush() const
	{
		return current_brush_;
	}

	inline Matrix3x2 RenderTarget::GetGlobalTransform() const
	{
		return global_transform_;
	}

	inline void RenderTarget::SetGlobalTransform(const Matrix3x2& matrix)
	{
		SetGlobalTransform(&matrix);
	}

	inline void RenderTarget::SetCurrentBrush(BrushPtr brush)
	{
		current_brush_ = brush;
	}

	inline bool TextureRenderTarget::IsValid() const
	{
		return bitmap_rt_ != nullptr;
	}

	inline ComPtr<ID2D1BitmapRenderTarget> TextureRenderTarget::GetBitmapRenderTarget() const
	{
		return bitmap_rt_;
	}

	inline void TextureRenderTarget::SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> rt)
	{
		bitmap_rt_ = rt;
	}
}
