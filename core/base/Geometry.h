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
#include "Unit.h"

namespace easy2d
{
	// 几何图形间相交关系
	enum class GeometryRelation : int
	{
		Unknown,
		Disjoin,		// 无交集
		IsContained,	// 被包含
		Contains,		// 包含
		Overlap			// 重叠
	};


	// 几何抽象
	class Geometry
		: public Unit
	{
	public:
		Geometry();

		virtual ~Geometry();

		cpGeometry const& GetD2DGeometry() const { return geo_; }

		float GetLength();

		bool ComputePointAt(
			float length,
			Point* point,
			Point* tangent
		);

		// 判断图形是否包含点
		bool ContainsPoint(
			Point const& point
		);

		// 判断两图形相交状态
		GeometryRelation GetRelationWith(
			spGeometry const& other
		);

	protected:
		cpGeometry geo_;
	};


	// 几何矩形
	class RectangleGeometry
		: public Geometry
	{
	public:
		RectangleGeometry();

		RectangleGeometry(
			Size const& rect_size
		);

		virtual ~RectangleGeometry();

		Size const& GetSize() const { return size_; }

		void SetSize(Size const& rect_size);

	protected:
		Size size_;
	};


	// 几何圆形
	class CircleGeometry
		: public Geometry
	{
	public:
		CircleGeometry();

		CircleGeometry(
			float radius
		);

		virtual ~CircleGeometry();

		float GetRadius() const { return radius_; }

		void SetRadius(
			float radius
		);

	protected:
		float radius_;
	};


	// 几何椭圆
	class EllipseGeometry
		: public Geometry
	{
	public:
		EllipseGeometry();

		EllipseGeometry(
			float radius_x,
			float radius_y
		);

		virtual ~EllipseGeometry();

		float GetRadiusX() const { return radius_x_; }

		float GetRadiusY() const { return radius_y_; }

		void SetRadius(
			float radius_x,
			float radius_y
		);

	protected:
		float radius_x_;
		float radius_y_;
	};


	// 几何路径
	class PathGeometry
		: public Geometry
	{
	public:
		PathGeometry();

		virtual ~PathGeometry();

		// 开始添加路径
		void BeginPath();

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
			std::vector<Point> const& points
		);

		// 添加一条三次方贝塞尔曲线
		void AddBezier(
			Point const& point1,	/* 贝塞尔曲线的第一个控制点 */
			Point const& point2,	/* 贝塞尔曲线的第二个控制点 */
			Point const& point3		/* 贝塞尔曲线的终点 */
		);

		// 清除路径
		void ClearPath();

	protected:
		cpPathGeometry	current_geometry_;
		cpGeometrySink	current_sink_;
	};
}
