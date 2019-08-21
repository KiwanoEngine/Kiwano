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
#include "Actor.h"
#include "../renderer/Geometry.h"
#include "../renderer/StrokeStyle.h"

namespace kiwano
{
	// 二维图形角色
	class KGE_API ShapeActor
		: public Actor
	{
	public:
		ShapeActor();

		ShapeActor(
			Geometry geometry
		);

		virtual ~ShapeActor();

		// 获取填充颜色
		inline Color GetFillColor() const			{ return fill_color_; }

		// 获取线条颜色
		inline Color GetStrokeColor() const			{ return stroke_color_; }

		// 获取线条宽度
		inline Float32 GetStrokeWidth() const		{ return stroke_width_; }

		// 获取线条样式
		inline StrokeStyle SetStrokeStyle() const	{ return stroke_style_; }

		// 获取形状
		inline Geometry GetGeometry() const			{ return geo_; }

		// 获取边界
		Rect GetBounds() const override;

		// 获取外切包围盒
		Rect GetBoundingBox() const override;

		// 判断点是否在形状内
		bool ContainsPoint(const Point& point) const override;

		// 设置填充颜色
		void SetFillColor(const Color& color);

		// 设置线条颜色
		void SetStrokeColor(const Color& color);

		// 设置线条宽度
		void SetStrokeWidth(Float32 width);

		// 设置线条样式
		void SetStrokeStyle(StrokeStyle stroke_style);

		// 设置形状
		void SetGeometry(Geometry geometry);

		void OnRender(RenderTarget* rt) override;

	protected:
		Color		fill_color_;
		Color		stroke_color_;
		Float32		stroke_width_;
		StrokeStyle	stroke_style_;
		Rect		bounds_;
		Geometry	geo_;
	};


	// 直线角色
	class KGE_API LineActor
		: public ShapeActor
	{
	public:
		LineActor();

		LineActor(
			Point const& point
		);

		virtual ~LineActor();

		Point const& GetPoint() const { return point_; }

		void SetPoint(
			Point const& point
		);

	protected:
		Point point_;
	};


	// 矩形角色
	class KGE_API RectActor
		: public ShapeActor
	{
	public:
		RectActor();

		RectActor(
			Size const& size
		);

		virtual ~RectActor();

		void SetRectSize(Size const& size);

		inline Size const& GetRectSize() const { return rect_size_; }

	protected:
		Size rect_size_;
	};


	// 圆角矩形角色
	class KGE_API RoundRectActor
		: public ShapeActor
	{
	public:
		RoundRectActor();

		RoundRectActor(
			Size const& size,
			Vec2 const& radius
		);

		virtual ~RoundRectActor();

		void SetRadius(
			Vec2 const& radius
		);

		void SetRectSize(
			Size const& size
		);

		void SetRoundedRect(
			Size const& size,
			Vec2 const& radius
		);

		inline Vec2 GetRadius() const	{ return radius_; }

		inline Size GetRectSize() const	{ return size_; }

	protected:
		Size rect_size_;
		Vec2 radius_;
	};


	// 圆形角色
	class KGE_API CircleActor
		: public ShapeActor
	{
	public:
		CircleActor();

		CircleActor(
			Float32 radius
		);

		virtual ~CircleActor();

		inline Float32 GetRadius() const { return radius_; }

		void SetRadius(Float32 radius);

	protected:
		Float32 radius_;
	};


	// 椭圆角色
	class KGE_API EllipseActor
		: public ShapeActor
	{
	public:
		EllipseActor();

		EllipseActor(
			Vec2 const& radius
		);

		virtual ~EllipseActor();

		Vec2 GetRadius() const { return radius_; }

		void SetRadius(
			Vec2 const& radius
		);

	protected:
		Vec2 radius_;
	};


	// 路径角色
	class KGE_API PathActor
		: public ShapeActor
	{
	public:
		PathActor();

		virtual ~PathActor();

		// 开始添加路径
		void BeginPath(
			Point const& begin_pos = Point{}	/* 起始点 */
		);

		// 结束路径
		void EndPath(
			bool closed = true		/* 路径是否闭合 */
		);

		// 添加一条线段
		void AddLine(
			Point const& point		/* 端点 */
		);

		// 添加多条线段
		void AddLines(
			Vector<Point> const& points
		);

		// 添加一条三次方贝塞尔曲线
		void AddBezier(
			Point const& point1,	/* 贝塞尔曲线的第一个控制点 */
			Point const& point2,	/* 贝塞尔曲线的第二个控制点 */
			Point const& point3		/* 贝塞尔曲线的终点 */
		);

		// 添加弧线
		void AddArc(
			Point const& point,		/* 终点 */
			Size const& radius,		/* 椭圆半径 */
			Float32 rotation,		/* 椭圆旋转角度 */
			bool clockwise = true,	/* 顺时针 or 逆时针 */
			bool is_small = true	/* 是否取小于 180° 的弧 */
		);

		// 清除路径
		void ClearPath();

	protected:
		GeometrySink sink_;
	};

}
