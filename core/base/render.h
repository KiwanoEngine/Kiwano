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
#include "base.h"
#include "Singleton.hpp"
#include "Font.h"
#include "Resource.h"
#include "Image.h"
#include "TextRenderer.h"
#include "../math/Matrix.hpp"

namespace easy2d
{
	namespace devices
	{
		struct D2DResources
		{
			ID2D1Factory*			factory;
			IWICImagingFactory*		imaging_factory;
			IDWriteFactory*			write_factory;
			ITextRenderer*			text_renderer;
			ID2D1SolidColorBrush*	solid_brush;
			ID2D1HwndRenderTarget*	render_target;
			ID2D1StrokeStyle*		miter_stroke_style;
			ID2D1StrokeStyle*		bevel_stroke_style;
			ID2D1StrokeStyle*		round_stroke_style;
		};


		class GraphicsDevice
		{
			E2D_DECLARE_SINGLETON(GraphicsDevice);

			E2D_DISABLE_COPY(GraphicsDevice);

		public:
			void Initialize(HWND hwnd);

			// 开始渲染
			void BeginDraw(HWND hwnd);

			// 结束渲染
			void EndDraw();

			// 设置背景色
			void SetBackgroundColor(
				const Color& color
			);

			// 渲染调试信息
			void DrawDebugInfo();

			void CreateDeviceResources(HWND hwnd);

			HRESULT CreateRectGeometry(
				const math::Matrix& matrix,
				const Size& size,
				ID2D1Geometry** geometry
			) const;

			HRESULT CreateTextFormat(
				IDWriteTextFormat** text_format,
				const Font& font
			) const;

			HRESULT CreateTextLayout(
				IDWriteTextLayout** text_layout,
				const String& text,
				IDWriteTextFormat* text_format,
				float wrap_width
			) const;

			HRESULT CreateLayer(
				ID2D1Layer** layer
			);

			ID2D1StrokeStyle* GetStrokeStyle(
				StrokeStyle stroke
			) const;

			HRESULT SetTransform(
				const math::Matrix& matrix
			);

			HRESULT SetBrushOpacity(
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
				ID2D1Geometry* geometry,
				const Color& border_color,
				float opacity,
				float stroke_width,
				StrokeStyle stroke = StrokeStyle::Miter
			);

			HRESULT DrawImage(
				Image* image,
				float opacity,
				const Rect& dest_rect,
				const Rect& source_rect
			);

			HRESULT DrawTextLayout(
				IDWriteTextLayout* text_layout
			);

			HRESULT PushClip(
				const math::Matrix& clip_matrix,
				const Size& clip_size
			);

			HRESULT PopClip();

			HRESULT PushLayer(
				ID2D1Layer* layer,
				LayerProperties properties
			);

			HRESULT PopLayer();

			HRESULT CreateBitmapFromFile(
				const String& file_path,
				ID2D1Bitmap** bitmap
			);

			HRESULT CreateBitmapFromResource(
				Resource& res,
				ID2D1Bitmap** bitmap
			);

			HRESULT Resize(
				UINT32 width,
				UINT32 height
			);

			void ClearImageCache();

		protected:
			GraphicsDevice();

			~GraphicsDevice();

		protected:
			D2DResources					d2d;
			D2D1_COLOR_F					clear_color_;
			IDWriteTextFormat*				fps_text_format_;
			IDWriteTextLayout*				fps_text_layout_;
			std::map<size_t, ID2D1Bitmap*>	bitmap_cache_;
		};

		E2D_DECLARE_SINGLETON_TYPE(GraphicsDevice, Graphics);
	}
}
