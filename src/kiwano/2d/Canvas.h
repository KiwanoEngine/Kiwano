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
#include <kiwano/renderer/RenderTarget.h>
#include <kiwano/renderer/GeometrySink.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Canvas);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief 画布，用于绘制图元
	*/
	class KGE_API Canvas
		: public Actor
	{
	public:
		/// \~chinese
		/// @brief 构建空画布
		Canvas();

		virtual ~Canvas();

		/// \~chinese
		/// @brief 开始绘图
		void BeginDraw();

		/// \~chinese
		/// @brief 结束绘图
		void EndDraw();

		/// \~chinese
		/// @brief 画线段
		/// @param begin 线段起点
		/// @param end 线段终点
		void DrawLine(Point const& begin, Point const& end);

		/// \~chinese
		/// @brief 画圆形边框
		/// @param center 圆形原点
		/// @param radius 圆形半径
		void DrawCircle(Point const& center, float radius);

		/// \~chinese
		/// @brief 画椭圆形边框
		/// @param center 椭圆原点
		/// @param radius 椭圆半径
		void DrawEllipse(Point const& center, Vec2 const& radius);

		/// \~chinese
		/// @brief 画矩形边框
		/// @param rect 矩形
		void DrawRect(Rect const& rect);

		/// \~chinese
		/// @brief 画圆角矩形边框
		/// @param rect 矩形
		/// @param radius 矩形圆角半径
		void DrawRoundedRect(Rect const& rect, Vec2 const& radius);

		/// \~chinese
		/// @brief 填充圆形
		/// @param center 圆形原点
		/// @param radius 圆形半径
		void FillCircle(Point const& center, float radius);

		/// \~chinese
		/// @brief 填充椭圆形
		/// @param center 椭圆原点
		/// @param radius 椭圆半径
		void FillEllipse(Point const& center, Vec2 const& radius);

		/// \~chinese
		/// @brief 填充矩形
		/// @param rect 矩形
		void FillRect(Rect const& rect);

		/// \~chinese
		/// @brief 填充圆角矩形
		/// @param rect 矩形
		/// @param radius 矩形圆角半径
		void FillRoundedRect(Rect const& rect, Vec2 const& radius);

		/// \~chinese
		/// @brief 绘制纹理
		/// @param texture 纹理
		/// @param src_rect 纹理裁剪区域
		/// @param dest_rect 绘制目标区域
		void DrawTexture(TexturePtr texture, const Rect* src_rect = nullptr, const Rect* dest_rect = nullptr);

		/// \~chinese
		/// @brief 绘制文字布局
		/// @param text 文字
		/// @param point 绘制文字的位置
		void DrawTextLayout(String const& text, Point const& point);

		/// \~chinese
		/// @brief 绘制文字布局
		/// @param layout 文字布局
		/// @param point 绘制布局的位置
		void DrawTextLayout(TextLayout const& layout, Point const& point);

		/// \~chinese
		/// @brief 开始绘制路径
		/// @param begin_pos 路径起始点
		void BeginPath(Point const& begin_pos);

		/// \~chinese
		/// @brief 结束路径
		/// @param closed 路径是否闭合
		void EndPath(bool closed = false);

		/// \~chinese
		/// @brief 添加一条线段
		/// @param point 端点
		void AddLine(Point const& point);

		/// \~chinese
		/// @brief 添加多条线段
		/// @param points 端点集合
		void AddLines(Vector<Point> const& points);

		/// \~chinese
		/// @brief 添加一条三次方贝塞尔曲线
		/// @param point1 贝塞尔曲线的第一个控制点
		/// @param point2 贝塞尔曲线的第二个控制点
		/// @param point3 贝塞尔曲线的终点
		void AddBezier(Point const& point1, Point const& point2, Point const& point3);

		/// \~chinese
		/// @brief 添加弧线
		/// @param point 终点
		/// @param radius 椭圆半径
		/// @param rotation 椭圆旋转角度
		/// @param clockwise 顺时针 or 逆时针
		/// @param is_small 是否取小于 180° 的弧
		void AddArc(Point const& point, Size const& radius, float rotation, bool clockwise = true, bool is_small = true);

		/// \~chinese
		/// @brief 以描边的方式绘制路径
		void StrokePath();

		/// \~chinese
		/// @brief 以填充的方式绘制路径
		void FillPath();

		/// \~chinese
		/// @brief 清空画布
		void Clear();

		/// \~chinese
		/// @brief 清空画布
		/// @param clear_color 清空颜色
		void Clear(Color const& clear_color);

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
		/// @brief 设置轮廓宽度
		/// @param width 轮廓宽度
		void SetStrokeWidth(float width);

		/// \~chinese
		/// @brief 设置轮廓样式
		/// @param stroke_style 轮廓样式
		void SetStrokeStyle(const StrokeStyle& stroke_style);

		/// \~chinese
		/// @brief 设置文字画刷样式
		/// @param text_style 文字画刷样式
		void SetTextStyle(TextStyle const& text_style);

		/// \~chinese
		/// @brief 设置画刷
		/// @param[in] brush 画刷
		void SetBrush(BrushPtr brush);

		/// \~chinese
		/// @brief 设置画刷二维变换
		/// @param transform 二维变换
		void SetBrushTransform(Transform const& transform);

		/// \~chinese
		/// @brief 设置画刷二维变换矩阵
		/// @param transform 二维变换矩阵
		void SetBrushTransform(Matrix3x2 const& transform);

		/// \~chinese
		/// @brief 添加一个图层
		/// @param area 图层区域
		void PushLayerArea(LayerArea& area);

		/// \~chinese
		/// @brief 删除最近添加的图层
		void PopLayerArea();

		/// \~chinese
		/// @brief 添加一个裁剪区域
		/// @param clip_rect 裁剪矩形
		void PushClipRect(Rect const& clip_rect);

		/// \~chinese
		/// @brief 删除最近添加的裁剪区域
		void PopClipRect();

		/// \~chinese
		/// @brief 获取轮廓宽度
		float GetStrokeWidth() const;

		/// \~chinese
		/// @brief 获取填充画刷
		BrushPtr GetFillBrush() const;

		/// \~chinese
		/// @brief 获取轮廓画刷
		BrushPtr GetStrokeBrush() const;

		/// \~chinese
		/// @brief 导出纹理
		TexturePtr ExportToTexture() const;

		void OnRender(RenderTarget* rt) override;

	private:
		void InitRenderTargetAndBrushs();

		void UpdateCache() const;

	private:
		float			stroke_width_;
		TextStyle		text_style_;
		StrokeStyle		stroke_style_;
		GeometrySink	geo_sink_;
		BrushPtr		fill_brush_;
		BrushPtr		stroke_brush_;

		mutable bool					cache_expired_;
		mutable TexturePtr				texture_cached_;
		mutable TextureRenderTargetPtr	rt_;
	};

	/** @} */

	inline void Canvas::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	inline void Canvas::SetStrokeStyle(const StrokeStyle& stroke_style)
	{
		stroke_style_ = stroke_style;
	}

	inline void Canvas::SetTextStyle(TextStyle const& text_style)
	{
		text_style_ = text_style;
	}

	inline void Canvas::SetStrokeColor(Color const& color)
	{
		InitRenderTargetAndBrushs();
		stroke_brush_->SetColor(color);
	}

	inline void Canvas::SetFillColor(Color const& color)
	{
		InitRenderTargetAndBrushs();
		fill_brush_->SetColor(color);
	}

	inline void Canvas::SetFillBrush(BrushPtr brush)
	{
		fill_brush_ = brush;
	}

	inline void Canvas::SetStrokeBrush(BrushPtr brush)
	{
		stroke_brush_ = brush;
	}

	inline BrushPtr Canvas::GetFillBrush() const
	{
		return fill_brush_;
	}

	inline BrushPtr Canvas::GetStrokeBrush() const
	{
		return stroke_brush_;
	}

}
