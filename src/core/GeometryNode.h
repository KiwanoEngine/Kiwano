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
#include "Geometry.h"
#include "Node.h"

namespace easy2d
{
	// 几何图形
	class E2D_API GeometryNode
		: public Node
	{
	public:
		GeometryNode();

		GeometryNode(
			GeometryPtr const& geometry
		);

		virtual ~GeometryNode();

		// 设置形状
		void SetGeometry(
			GeometryPtr const& geometry
		);

		// 设置填充颜色
		void SetFillColor(
			const Color& color
		);

		// 设置线条颜色
		void SetStrokeColor(
			const Color& color
		);

		// 设置线条宽度
		void SetStrokeWidth(
			float width
		);

		// 设置线条相交样式
		void SetOutlineJoinStyle(
			StrokeStyle outline_join
		);

		// 获取形状
		GeometryPtr const& GetGeometry() const { return geometry_; }

		// 获取填充颜色
		Color GetFillColor() const { return fill_color_; }

		// 获取线条颜色
		Color GetStrokeColor() const { return stroke_color_; }

		// 获取线条宽度
		float GetStrokeWidth() const { return stroke_width_; }

		// 获取线条相交样式
		StrokeStyle SetOutlineJoinStyle() const { return outline_join_; }

		void OnRender() override;

	protected:
		Color		fill_color_;
		Color		stroke_color_;
		float		stroke_width_;
		StrokeStyle	outline_join_;
		GeometryPtr	geometry_;
	};
}
