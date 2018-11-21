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
#include "base.hpp"
#include "Singleton.hpp"
#include "Font.hpp"
#include "Resource.h"
#include "TextRenderer.h"
#include "TextStyle.hpp"
#include "../math/Matrix.hpp"

namespace easy2d
{
	enum class TextAntialias
	{
		Default,	// 系统默认
		ClearType,	// ClearType 抗锯齿
		GrayScale,	// 灰度抗锯齿
		None		// 不启用抗锯齿
	};

	// 图形渲染选项
	struct GraphicsOptions
	{
		bool vsync;						// 垂直同步
		bool antialias;					// 抗锯齿
		TextAntialias text_antialias;	// 文字抗锯齿模式

		GraphicsOptions()
			: vsync(true)
			, antialias(true)
			, text_antialias(TextAntialias::ClearType)
		{}
	};

	namespace devices
	{
		class GraphicsDevice
			: protected Noncopyable
		{
			E2D_DECLARE_SINGLETON(GraphicsDevice);

		public:
			void Init(HWND hwnd, GraphicsOptions options, bool debug);

			// 开始渲染
			void BeginDraw(HWND hwnd);

			// 结束渲染
			void EndDraw();

			// 设置背景色
			void SetBackgroundColor(
				const Color& color
			);

			void CreateDeviceResources(HWND hwnd);

			HRESULT CreateLayer(
				cpLayer& layer
			);

			HRESULT CreateSolidColorBrush(
				cpSolidColorBrush& brush
			) const;

			HRESULT CreateBitmapFromFile(
				cpBitmap& bitmap,
				String const& file_path
			);

			HRESULT CreateBitmapFromResource(
				cpBitmap& bitmap,
				Resource const& res
			);

			HRESULT CreateBitmapRenderTarget(
				cpBitmapRenderTarget& brt
			);

			HRESULT SetTransform(
				const math::Matrix& matrix
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
				cpGeometry const& geometry,
				const Color& stroke_color,
				float stroke_width,
				StrokeStyle stroke = StrokeStyle::Miter
			);

			HRESULT FillGeometry(
				cpGeometry const& geometry,
				const Color& fill_color
			);

			HRESULT DrawImage(
				spImage const& image
			);

			HRESULT DrawBitmap(
				cpBitmap const& bitmap
			);

			HRESULT DrawTextLayout(
				cpTextLayout const& text_layout
			);

			HRESULT PushClip(
				const math::Matrix& clip_matrix,
				const Size& clip_size
			);

			HRESULT PopClip();

			HRESULT PushLayer(
				cpLayer const& layer,
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

		protected:
			GraphicsDevice();

			~GraphicsDevice();

		protected:
			bool						initialized_;
			bool						window_occluded_;
			float						opacity_;
			GraphicsOptions				options_;
			cpTextRenderer				text_renderer_;
			cpSolidColorBrush			solid_brush_;
			cpHwndRenderTarget			render_target_;
			D2D1_COLOR_F				clear_color_;
			cpTextFormat				fps_text_format_;
			cpTextLayout				fps_text_layout_;
			std::map<size_t, cpBitmap>	bitmap_cache_;
		};

		E2D_DECLARE_SINGLETON_TYPE(GraphicsDevice, Graphics);
	}
}
