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
#include "TextRenderer.h"

namespace easy2d
{
	namespace render
	{
		typedef struct
		{
			ID2D1Factory*			Factory;
			IWICImagingFactory*		WICImagingFactory;
			IDWriteFactory*			DWriteFactory;
			ITextRenderer*			TextRenderer;
			ID2D1SolidColorBrush*	SolidColorBrush;
			ID2D1HwndRenderTarget*	HwndRenderTarget;
			ID2D1StrokeStyle*		MiterStrokeStyle;
			ID2D1StrokeStyle*		BevelStrokeStyle;
			ID2D1StrokeStyle*		RoundStrokeStyle;
		} _D2D_Resource;

		extern _D2D_Resource D2D;

		void Initialize(HWND hwnd);

		void CreateDeviceResources(HWND hwnd);

		void Uninitialize();

		class GraphicsDevice
		{
		public:
			GraphicsDevice();

			~GraphicsDevice();

			// 开始渲染
			void BeginDraw(HWND hwnd);

			// 结束渲染
			void EndDraw();

			// 渲染调试信息
			void DrawDebugInfo();

			// 设置背景色
			void SetBackgroundColor(
				const Color& color
			);

		protected:
			D2D1_COLOR_F		clear_color_;
			IDWriteTextFormat*	fps_text_format_;
			IDWriteTextLayout*	fps_text_layout_;
		};

		extern GraphicsDevice instance;
	}
}
