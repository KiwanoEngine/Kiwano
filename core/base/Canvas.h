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
#include "Node.h"

namespace easy2d
{
	E2D_DECLARE_SMART_PTR(CanvasBrush);

	// 画笔
	class CanvasBrush
		: public RefCounter
	{
	public:
		CanvasBrush();

		virtual ~CanvasBrush();

		// 画直线
		void DrawLine(
			const Point& begin,
			const Point& end
		);

		// 画圆形边框
		void DrawCircle(
			const Point& center,
			float radius
		);

		// 画椭圆形边框
		void DrawEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// 画矩形边框
		void DrawRect(
			const Rect& rect
		);

		// 画圆角矩形边框
		void DrawRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

		// 填充圆形
		void FillCircle(
			const Point& center,
			float radius
		);

		// 填充椭圆形
		void FillEllipse(
			const Point& center,
			float radius_x,
			float radius_y
		);

		// 填充矩形
		void FillRect(
			const Rect& rect
		);

		// 填充圆角矩形
		void FillRoundedRect(
			const Rect& rect,
			float radius_x,
			float radius_y
		);

		// 设置线条颜色
		void SetLineColor(
			const Color& color
		);

		// 设置填充颜色
		void SetFillColor(
			const Color& color
		);

		// 设置线条宽度
		void SetStrokeWidth(
			float width
		);

		// 设置线条相交样式
		void SetStrokeStyle(
			StrokeStyle stroke
		);

		// 获取线条颜色
		Color GetLineColor() const;

		// 获取填充颜色
		Color GetFillColor() const;

		// 获取线条宽度
		float GetStrokeWidth() const;

		// 获取线条相交样式
		StrokeStyle GetStrokeStyle() const;

	protected:
		float				stroke_width_;
		StrokeStyle			stroke_;
		cpRenderTarget		render_target_;
		cpSolidColorBrush	fill_brush_;
		cpSolidColorBrush	line_brush_;
		cpStrokeStyle		stroke_style_;
	};


	// 画布
	class Canvas
		: public Node
	{
	public:
		Canvas();

		Canvas(
			float width,
			float height
		);

		virtual ~Canvas();

		virtual void OnDraw(CanvasBrush& brush) = 0;

		// 设置画刷
		void SetBrush(
			spCanvasBrush const& brush
		);

		virtual void OnDraw() override;

	private:
		spCanvasBrush brush_;
	};
}