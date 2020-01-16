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
#include <kiwano/2d/Actor.h>
#include <kiwano/renderer/Brush.h>
#include <kiwano/renderer/Geometry.h>
#include <kiwano/renderer/GeometrySink.h>
#include <kiwano/renderer/StrokeStyle.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(ShapeActor);
	KGE_DECLARE_SMART_PTR(LineActor);
	KGE_DECLARE_SMART_PTR(RectActor);
	KGE_DECLARE_SMART_PTR(RoundRectActor);
	KGE_DECLARE_SMART_PTR(CircleActor);
	KGE_DECLARE_SMART_PTR(EllipseActor);
	KGE_DECLARE_SMART_PTR(PolygonActor);
	KGE_DECLARE_SMART_PTR(PathShapeActor);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief 二维形状角色
	*/
	class KGE_API ShapeActor
		: public Actor
	{
	public:
		/// \~chinese
		/// @brief 构造二维形状角色
		ShapeActor();

		virtual ~ShapeActor();

		/// \~chinese
		/// @brief 获取填充画刷
		BrushPtr GetFillBrush() const;

		/// \~chinese
		/// @brief 获取轮廓画刷
		BrushPtr GetStrokeBrush() const;

		/// \~chinese
		/// @brief 获取线条宽度
		float GetStrokeWidth() const;

		/// \~chinese
		/// @brief 获取线条样式
		const StrokeStyle& GetStrokeStyle() const;

		/// \~chinese
		/// @brief 获取形状
		Geometry GetGeometry() const;

		/// \~chinese
		/// @brief 获取边界
		Rect GetBounds() const override;

		/// \~chinese
		/// @brief 获取外切包围盒
		Rect GetBoundingBox() const override;

		/// \~chinese
		/// @brief 判断点是否在形状内
		bool ContainsPoint(const Point& point) const override;

		/// \~chinese
		/// @brief 设置填充颜色
		/// @param color 填充颜色
		void SetFillColor(Color const& color);

		/// \~chinese
		/// @brief 设置填充画刷
		/// @param[in] brush 填充画刷
		void SetFillBrush(BrushPtr brush);

		/// \~chinese
		/// @brief 设置轮廓颜色
		/// @param color 轮廓颜色
		void SetStrokeColor(Color const& color);

		/// \~chinese
		/// @brief 设置轮廓画刷
		/// @param[in] brush 轮廓画刷
		void SetStrokeBrush(BrushPtr brush);

		/// \~chinese
		/// @brief 设置线条宽度，默认为 1.0
		void SetStrokeWidth(float width);

		/// \~chinese
		/// @brief 设置线条样式
		void SetStrokeStyle(const StrokeStyle& stroke_style);

		/// \~chinese
		/// @brief 设置几何形状
		void SetGeometry(Geometry const& geometry);

		void OnRender(RenderContext& ctx) override;

	protected:
		bool CheckVisibility(RenderContext& ctx) const override;

	private:
		BrushPtr	fill_brush_;
		BrushPtr	stroke_brush_;
		float		stroke_width_;
		StrokeStyle	stroke_style_;
		Rect		bounds_;
		Geometry	geo_;
	};


	/// \~chinese
	/// @brief 线段图形角色
	class KGE_API LineActor
		: public ShapeActor
	{
	public:
		LineActor();

		virtual ~LineActor();

		/// \~chinese
		/// @brief 获取线段起点
		Point const& GetBeginPoint() const;

		/// \~chinese
		/// @brief 获取线段终点
		Point const& GetEndPoint() const;

		/// \~chinese
		/// @brief 设置线段起点
		/// @param begin 线段起点
		void SetBeginPoint(Point const& begin);

		/// \~chinese
		/// @brief 设置线段终点
		/// @param end 线段终点
		void SetEndPoint(Point const& end);

		/// \~chinese
		/// @brief 设置矩形大小
		/// @param begin 线段起点
		/// @param end 线段终点
		void SetLine(Point const& begin, Point const& end);

	private:
		Point begin_;
		Point end_;
	};


	/// \~chinese
	/// @brief 矩形角色
	class KGE_API RectActor
		: public ShapeActor
	{
	public:
		RectActor();

		virtual ~RectActor();

		/// \~chinese
		/// @brief 获取矩形大小
		Size const& GetRectSize() const;

		/// \~chinese
		/// @brief 设置矩形大小
		/// @param size 矩形大小
		void SetRectSize(Size const& size);

	private:
		Size rect_size_;
	};



	/// \~chinese
	/// @brief 圆角矩形角色
	class KGE_API RoundRectActor
		: public ShapeActor
	{
	public:
		RoundRectActor();

		virtual ~RoundRectActor();

		/// \~chinese
		/// @brief 获取圆角半径
		Vec2 GetRadius() const;

		/// \~chinese
		/// @brief 获取圆角矩形大小
		Size GetRectSize() const;

		/// \~chinese
		/// @brief 设置圆角半径
		/// @param radius 圆角半径
		void SetRadius(Vec2 const& radius);

		/// \~chinese
		/// @brief 设置圆角矩形大小
		/// @param size 圆角矩形大小
		void SetRectSize(Size const& size);

		/// \~chinese
		/// @brief 设置圆角矩形
		/// @param size 圆角矩形大小
		/// @param radius 圆角半径
		void SetRoundedRect(Size const& size, Vec2 const& radius);

	private:
		Size rect_size_;
		Vec2 radius_;
	};


	/// \~chinese
	/// @brief 圆形角色
	class KGE_API CircleActor
		: public ShapeActor
	{
	public:
		CircleActor();

		virtual ~CircleActor();

		/// \~chinese
		/// @brief 获取圆形半径
		float GetRadius() const;

		/// \~chinese
		/// @brief 设置圆形半径
		/// @param radius 圆形半径
		void SetRadius(float radius);

	private:
		float radius_;
	};


	/// \~chinese
	/// @brief 椭圆角色
	class KGE_API EllipseActor
		: public ShapeActor
	{
	public:
		EllipseActor();

		virtual ~EllipseActor();

		/// \~chinese
		/// @brief 获取椭圆半径
		Vec2 GetRadius() const;

		/// \~chinese
		/// @brief 设置椭圆半径
		/// @param radius 椭圆半径
		void SetRadius(Vec2 const& radius);

	private:
		Vec2 radius_;
	};


	/// \~chinese
	/// @brief 多边形角色
	class KGE_API PolygonActor
		: public ShapeActor
	{
	public:
		PolygonActor();

		virtual ~PolygonActor();

		/// \~chinese
		/// @brief 设置多边形端点
		/// @param points 多边形端点集合
		void SetVertices(Vector<Point> const& points);
	};


	/// \~chinese
	/// @brief 路径图形角色
	class KGE_API PathShapeActor
		: public ShapeActor
	{
	public:
		PathShapeActor();

		virtual ~PathShapeActor();

		/// \~chinese
		/// @brief 开始添加路径
		/// @param begin_pos 起始点
		void BeginPath(Point const& begin_pos = Point());

		/// \~chinese
		/// @brief 结束添加路径
		/// @param closed 路径是否闭合
		void EndPath(bool closed = true);

		/// \~chinese
		/// @brief 添加一条线段
		/// @param point 线段端点
		void AddLine(Point const& point);

		/// \~chinese
		/// @brief 添加多条线段
		/// @param points 线段端点集合
		void AddLines(Vector<Point> const& points);

		/// \~chinese
		/// @brief 添加一条三次方贝塞尔曲线
		/// @param point1 贝塞尔曲线的第一个控制点
		/// @param point2 贝塞尔曲线的第二个控制点
		/// @param point3 贝塞尔曲线的终点
		void AddBezier(Point const& point1, Point const& point2, Point const& point3);

		/// \~chinese
		/// @brief 添加弧线
		/// @param point 椭圆圆心
		/// @param radius 椭圆半径
		/// @param rotation 椭圆旋转角度
		/// @param clockwise 顺时针 or 逆时针
		/// @param is_small 是否取小于 180° 的弧
		void AddArc(Point const& point, Size const& radius, float rotation, bool clockwise = true, bool is_small = true);

		/// \~chinese
		/// @brief 清除路径
		void ClearPath();

	private:
		GeometrySink sink_;
	};

	/** @} */

	inline void ShapeActor::SetStrokeColor(Color const& color)
	{
		if (!stroke_brush_)
		{
			stroke_brush_ = new Brush;
		}
		stroke_brush_->SetColor(color);
	}

	inline void ShapeActor::SetFillColor(Color const& color)
	{
		if (!fill_brush_)
		{
			fill_brush_ = new Brush;
		}
		fill_brush_->SetColor(color);
	}

	inline void					ShapeActor::SetFillBrush(BrushPtr brush)		{ fill_brush_ = brush; }
	inline void					ShapeActor::SetStrokeBrush(BrushPtr brush)		{ stroke_brush_ = brush; }
	inline BrushPtr				ShapeActor::GetFillBrush() const				{ return fill_brush_; }
	inline BrushPtr				ShapeActor::GetStrokeBrush() const				{ return stroke_brush_; }
	inline float				ShapeActor::GetStrokeWidth() const				{ return stroke_width_; }
	inline const StrokeStyle&	ShapeActor::GetStrokeStyle() const				{ return stroke_style_; }
	inline Geometry				ShapeActor::GetGeometry() const					{ return geo_; }

	inline Point const&			LineActor::GetBeginPoint() const				{ return begin_; }
	inline Point const&			LineActor::GetEndPoint() const					{ return end_; }
	inline void					LineActor::SetBeginPoint(Point const& begin)	{ SetLine(begin, end_); }
	inline void					LineActor::SetEndPoint(Point const& end)		{ SetLine(begin_, end); }

	inline Size const&			RectActor::GetRectSize() const					{ return rect_size_; }

	inline Vec2					RoundRectActor::GetRadius() const				{ return radius_; }
	inline Size					RoundRectActor::GetRectSize() const				{ return GetSize(); }

	inline float				CircleActor::GetRadius() const					{ return radius_; }

	inline Vec2			EllipseActor::GetRadius() const					{ return radius_; }
}
