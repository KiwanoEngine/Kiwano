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

#pragma once
#include "include-forwards.h"
#include "time.h"
#include "Font.hpp"
#include "Resource.h"
#include "TextRenderer.h"
#include "TextStyle.hpp"
#include "Singleton.hpp"
#include "../math/Matrix.h"
#include <unordered_map>

namespace easy2d
{
	class RenderSystem
		: public ISingleton<RenderSystem>
	{
		E2D_DECLARE_SINGLETON(RenderSystem);

		struct Status
		{
			TimePoint start;
			Duration duration;
			int primitives;
		};

		using BitmapMap = UnorderedMap<size_t, D2DBitmapPtr>;

	public:
		HRESULT Init(HWND hwnd, bool vsync, bool debug);

		// 开始渲染
		HRESULT BeginDraw(HWND hwnd);

		// 结束渲染
		HRESULT EndDraw();

		// 设置清空屏幕的颜色
		void SetClearColor(
			const Color& color
		);

		// 设置抗锯齿模式
		HRESULT SetAntialiasMode(
			bool enabled
		);

		// 设置文字抗锯齿模式
		HRESULT SetTextAntialiasMode(
			TextAntialias mode
		);

		Status const& GetStatus() const;

		HRESULT CreateResources(
			HWND hwnd
		);

		void DiscardResources();

		HRESULT CreateLayer(
			D2DLayerPtr& layer
		);

		HRESULT CreateSolidColorBrush(
			D2DSolidColorBrushPtr& brush
		) const;

		HRESULT CreateBitmapFromFile(
			D2DBitmapPtr& bitmap,
			String const& file_path
		);

		HRESULT CreateBitmapFromResource(
			D2DBitmapPtr& bitmap,
			Resource const& res
		);

		HRESULT CreateBitmapRenderTarget(
			D2DBitmapRenderTargetPtr& brt
		);

		HRESULT SetTransform(
			const Matrix& matrix
		);

		HRESULT SetOpacity(
			float opacity
		);

		HRESULT SetTextStyle(
			const Color& color,
			bool has_outline,
			const Color& outline_color,
			float outline_width,
			StrokeStyle outline_stroke
		);

		HRESULT DrawGeometry(
			D2DGeometryPtr const& geometry,
			const Color& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		HRESULT FillGeometry(
			D2DGeometryPtr const& geometry,
			const Color& fill_color
		);

		HRESULT DrawImage(
			ImagePtr const& image
		);

		HRESULT DrawBitmap(
			D2DBitmapPtr const& bitmap
		);

		HRESULT DrawTextLayout(
			D2DTextLayoutPtr const& text_layout
		);

		HRESULT PushClip(
			const Matrix& clip_matrix,
			const Size& clip_size
		);

		HRESULT PopClip();

		HRESULT PushLayer(
			D2DLayerPtr const& layer,
			LayerProperties const& properties
		);

		HRESULT PopLayer();

		HRESULT GetSize(
			Size& size
		);

		HRESULT Resize(
			UINT32 width,
			UINT32 height
		);

		void ClearImageCache();

		D2DHwndRenderTargetPtr const& GetRenderTarget() const;

		D2DSolidColorBrushPtr const& GetSolidBrush() const;

	protected:
		RenderSystem();

		~RenderSystem();

	protected:
		bool					debug_;
		bool					window_occluded_;
		bool					vsync_enabled_;
		bool					antialias_;
		float					opacity_;
		D2D1_COLOR_F			clear_color_;
		TextAntialias			text_antialias_;
		Status					status_;
		D2DTextRendererPtr		text_renderer_;
		D2DSolidColorBrushPtr	solid_brush_;
		D2DHwndRenderTargetPtr	render_target_;
		D2DTextFormatPtr		fps_text_format_;
		D2DTextLayoutPtr		fps_text_layout_;
		BitmapMap				bitmap_cache_;
	};
}
