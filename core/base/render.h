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
#include "../math/Matrix.hpp"
#include <unordered_map>

namespace easy2d
{
	class GraphicsDevice
		: protected Noncopyable
	{
		E2D_DECLARE_SINGLETON(GraphicsDevice);

		struct Status
		{
			TimePoint start;
			Duration duration;
			int primitives;
		};

		using BitmapMap = std::unordered_map<size_t, cpBitmap>;

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
			cpLayer& layer
		);

		HRESULT CreateSolidColorBrush(
			cpSolidColorBrush& brush
		) const;

		HRESULT CreateBitmapFromFile(
			cpBitmap& bitmap,
			std::wstring const& file_path
		);

		HRESULT CreateBitmapFromResource(
			cpBitmap& bitmap,
			Resource const& res
		);

		HRESULT CreateBitmapRenderTarget(
			cpBitmapRenderTarget& brt
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
			const Matrix& clip_matrix,
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

		cpHwndRenderTarget const& GetRenderTarget() const;

		cpSolidColorBrush const& GetSolidBrush() const;

	protected:
		GraphicsDevice();

		~GraphicsDevice();

	protected:
		bool				debug_;
		bool				window_occluded_;
		bool				vsync_enabled_;
		bool				antialias_;
		float				opacity_;
		D2D1_COLOR_F		clear_color_;
		TextAntialias		text_antialias_;
		Status				status_;
		cpTextRenderer		text_renderer_;
		cpSolidColorBrush	solid_brush_;
		cpHwndRenderTarget	render_target_;
		cpTextFormat		fps_text_format_;
		cpTextLayout		fps_text_layout_;
		BitmapMap			bitmap_cache_;
	};

	E2D_DECLARE_SINGLETON_TYPE(GraphicsDevice, Graphics);
}
