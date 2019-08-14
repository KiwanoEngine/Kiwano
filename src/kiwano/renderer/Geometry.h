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
#include "../base/SmartPtr.hpp"
#include "D2DDeviceResources.h"

namespace kiwano
{
	// 几何体
	class KGE_API Geometry
	{
	public:
		// 获取外切包围盒
		Rect GetBoundingBox(
			Matrix const& transform = Matrix()
		) const;

		// 判断图形是否包含点
		bool ContainsPoint(
			Point const& point
		);

		// 获取图形展开成一条直线的长度
		float GetLength();

		// 计算面积
		float ComputeArea();

		// 计算图形路径上点的位置和切线向量
		bool ComputePointAtLength(
			float length,
			Point& point,
			Vec2& tangent
		);

	public:
		Geometry();

		Geometry(ComPtr<ID2D1Geometry> geo);

	public:
		inline ComPtr<ID2D1Geometry> GetGeometry() const		{ return geo_; }

		inline void SetGeometry(ComPtr<ID2D1Geometry> geometry)	{ geo_ = geometry; }

		inline operator bool() const							{ return static_cast<bool>(geo_); }

	protected:
		ComPtr<ID2D1Geometry> geo_;
	};


	// 几何体生成器
	class KGE_API GeometrySink
		: protected noncopyable
	{
	public:
		GeometrySink();

		// 开始添加路径
		GeometrySink& BeginPath(
			Point const& begin_pos = Point{}	/* 起始点 */
		);

		// 结束路径
		GeometrySink& EndPath(
			bool closed = false		/* 路径是否闭合 */
		);

		// 添加一条线段
		GeometrySink& AddLine(
			Point const& point		/* 端点 */
		);

		// 添加多条线段
		GeometrySink& AddLines(
			Vector<Point> const& points
		);

		// 添加一条三次方贝塞尔曲线
		GeometrySink& AddBezier(
			Point const& point1,	/* 贝塞尔曲线的第一个控制点 */
			Point const& point2,	/* 贝塞尔曲线的第二个控制点 */
			Point const& point3		/* 贝塞尔曲线的终点 */
		);

		// 添加弧线
		GeometrySink& AddArc(
			Point const& point,		/* 终点 */
			Size const& radius,		/* 椭圆半径 */
			float rotation,			/* 椭圆旋转角度 */
			bool clockwise = true,	/* 顺时针 or 逆时针 */
			bool is_small = true	/* 是否取小于 180° 的弧 */
		);

		// 获取生成路径几何体
		Geometry GetGeometry();

	public:
		inline ComPtr<ID2D1PathGeometry> GetPathGeometry() const	{ return path_geo_; }

		inline void SetPathGeometry(ComPtr<ID2D1PathGeometry> path)	{ path_geo_ = path; }

	protected:
		ComPtr<ID2D1PathGeometry> path_geo_;
		ComPtr<ID2D1GeometrySink> sink_;
	};
}
