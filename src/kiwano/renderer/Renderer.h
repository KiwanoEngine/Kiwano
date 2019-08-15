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

#pragma once
#include "../base/time.h"
#include "../base/Component.h"
#include "../base/Resource.h"
#include "../2d/include-forwards.h"
#include "helper.hpp"
#include "Image.h"
#include "Geometry.h"
#include "TextLayout.h"
#include "TextRenderer.h"

#if defined(KGE_USE_DIRECTX10)
#	include "D3D10DeviceResources.h"
#else
#	include "D3D11DeviceResources.h"
#endif

namespace kiwano
{

#if defined(KGE_USE_DIRECTX10)
	typedef ID3D10DeviceResources ID3DDeviceResources;
#else
	typedef ID3D11DeviceResources ID3DDeviceResources;
#endif

	class KGE_API Renderer
		: public Singleton<Renderer>
		, public Component
	{
		KGE_DECLARE_SINGLETON(Renderer);

	public:
		// 设置清屏颜色
		void SetClearColor(
			Color const& clear_color
		);

		// 设置抗锯齿模式
		void SetAntialiasMode(
			bool enabled
		);

		// 设置文字抗锯齿模式
		void SetTextAntialiasMode(
			TextAntialias mode
		);

		// 开启或关闭垂直同步
		void SetVSyncEnabled(
			bool enabled
		);

	public:
		void CreateLayer(
			ComPtr<ID2D1Layer>& layer
		);

		void CreateTextFormat(
			TextFormat& format,
			Font const& font
		);

		void CreateTextLayout(
			TextLayout& layout,
			String const& text,
			TextStyle const& style,
			TextFormat const& format
		);

		void CreateLineGeometry(
			Geometry& geo,
			Point const& begin_pos,
			Point const& end_pos
		);

		void CreateRectGeometry(
			Geometry& geo,
			Rect const& rect
		);

		void CreateRoundedRectGeometry(
			Geometry& geo,
			Rect const& rect,
			Vec2 const& radius
		);

		void CreateEllipseGeometry(
			Geometry& geo,
			Point const& center,
			Vec2 const& radius
		);

		void CreatePathGeometrySink(
			GeometrySink& sink
		);

		void DrawGeometry(
			Geometry const& geometry,
			const Color& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void FillGeometry(
			Geometry const& geometry,
			Color const& fill_color
		);

		void DrawRectangle(
			Rect const& rect,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		void FillRectangle(
			Rect const& rect,
			Color const& fill_color
		);

		void DrawBitmap(
			ComPtr<ID2D1Bitmap> const& bitmap,
			Rect const& src_rect,
			Rect const& dest_rect
		);

		void DrawTextLayout(
			TextLayout const& layout
		);

		void SetOpacity(
			float opacity
		);

		void SetTransform(
			const Matrix3x2& matrix
		);

		void SetTextStyle(
			float opacity,
			const Color& color,
			bool has_outline,
			const Color& outline_color,
			float outline_width,
			StrokeStyle outline_stroke
		);

		void PushClip(
			const Matrix3x2& clip_matrix,
			const Size& clip_size
		);

		void PopClip();

		void PushLayer(
			ComPtr<ID2D1Layer> const& layer,
			LayerProperties const& properties
		);

		void PopLayer();

		void Resize(
			UINT width,
			UINT height
		);

		bool CheckVisibility(
			Size const& content_size,
			Matrix3x2 const& transform
		);

	public:
		void SetupComponent() override;

		void DestroyComponent() override;

		void BeforeRender() override;

		void AfterRender() override;

		void HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

		void SetCollectingStatus(bool collecting);

	public:
		struct Status
		{
			Time start;
			Duration duration;
			int primitives;
		};

		inline HWND						GetTargetWindow() const			{ return hwnd_; }

		inline Status const&			GetStatus() const				{ return status_; }

		inline Size const&				GetOutputSize() const			{ return output_size_; }

		inline ID2DDeviceResources*		GetD2DDeviceResources() const	{ KGE_ASSERT(d2d_res_); return d2d_res_.get(); }

		inline ID3DDeviceResources*		GetD3DDeviceResources() const	{ KGE_ASSERT(d3d_res_); return d3d_res_.get(); }

		inline ITextRenderer*			GetTextRenderer() const			{ KGE_ASSERT(text_renderer_); return text_renderer_.get(); }

		inline ID2D1SolidColorBrush*	GetSolidColorBrush() const		{ KGE_ASSERT(solid_color_brush_); return solid_color_brush_.get(); }

	private:
		Renderer();

		~Renderer();

		HRESULT CreateDeviceResources();

		HRESULT HandleDeviceLost();

		HRESULT BeginDraw();

		HRESULT EndDraw();

		void IncreasePrimitivesCount();

	private:
		bool			vsync_;
		bool			antialias_;
		bool			collecting_status_;
		float			opacity_;
		HWND			hwnd_;
		Size			output_size_;
		Color			clear_color_;
		TextAntialias	text_antialias_;
		Status			status_;

		ComPtr<ID2DDeviceResources>		d2d_res_;
		ComPtr<ID3DDeviceResources>		d3d_res_;
		ComPtr<ID2D1DeviceContext>		device_context_;
		ComPtr<ID2D1DrawingStateBlock>	drawing_state_block_;
		ComPtr<ITextRenderer>			text_renderer_;
		ComPtr<ID2D1SolidColorBrush>	solid_color_brush_;
	};
}
