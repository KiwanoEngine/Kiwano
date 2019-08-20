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
#include "../renderer/RenderTarget.h"

#ifdef DrawText
#	undef DrawText
#endif

namespace kiwano
{
	// 画布
	class KGE_API Canvas
		: public Actor
	{
	public:
		Canvas();

		virtual ~Canvas();

		// 开始绘图
		void BeginDraw();

		// 结束绘图
		void EndDraw();

		// 画直线
		void DrawLine(
			Point const& begin,
			Point const& end
		);

		// 画圆形边框
		void DrawCircle(
			Point const& center,
			Float32 radius
		);

		// 画椭圆形边框
		void DrawEllipse(
			Point const& center,
			Vec2 const& radius
		);

		// 画矩形边框
		void DrawRect(
			Rect const& rect
		);

		// 画圆角矩形边框
		void DrawRoundedRect(
			Rect const& rect,
			Vec2 const& radius
		);

		// 填充圆形
		void FillCircle(
			Point const& center,
			Float32 radius
		);

		// 填充椭圆形
		void FillEllipse(
			Point const& center,
			Vec2 const& radius
		);

		// 填充矩形
		void FillRect(
			Rect const& rect
		);

		// 填充圆角矩形
		void FillRoundedRect(
			Rect const& rect,
			Vec2 const& radius
		);

		// 画图片
		void DrawImage(
			Image const& image,
			const Rect* src_rect = nullptr,
			const Rect* dest_rect = nullptr
		);

		// 画文字
		void DrawText(
			String const& text,		/* 文字 */
			Point const& point		/* 文字位置 */
		);

		// 开始绘制路径
		void BeginPath(
			Point const& begin_pos	/* 路径起始点 */
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
			Point const& radius,	/* 椭圆半径 */
			Float32 rotation,		/* 椭圆旋转角度 */
			bool clockwise = true,	/* 顺时针 or 逆时针 */
			bool is_small = true	/* 是否取小于 180° 的弧 */
		);

		// 路径描边
		void StrokePath();

		// 路径填充
		void FillPath();

		// 清空画布
		void Clear();

		// 清空画布
		void Clear(
			Color const& clear_color
		);

		// 设置填充颜色
		void SetFillColor(
			Color const& color
		);

		// 设置线条颜色
		void SetStrokeColor(
			Color const& color
		);

		// 设置线条宽度
		void SetStrokeWidth(
			Float32 width
		);

		// 设置线条样式
		void SetStrokeStyle(
			StrokeStyle stroke_style
		);

		// 设置文字字体
		void SetTextFont(
			Font const& font
		);

		// 设置文字画刷样式
		void SetTextStyle(
			TextStyle const& text_style
		);

		// 设置画笔透明度
		void SetBrushOpacity(
			Float32 opacity
		);

		// 画笔二维变换
		void SetBrushTransform(
			Transform const& transform
		);

		// 画笔二维变换
		void SetBrushTransform(
			Matrix3x2 const& transform
		);

		// 设置图层
		void PushLayerArea(
			LayerArea& area
		);

		// 弹出图层
		void PopLayerArea();

		// 设置裁剪区域
		void PushClipRect(
			Rect const& clip_rect
		);

		// 弹出裁剪区域
		void PopClipRect();

		// 获取填充颜色
		Color GetFillColor() const;

		// 获取线条颜色
		Color GetStrokeColor() const;

		// 获取线条宽度
		Float32 GetStrokeWidth() const;

		// 获取画笔透明度
		Float32 GetBrushOpacity() const;

		// 导出为图片
		Image ExportToImage() const;

		void OnRender(RenderTarget* rt) override;

	protected:
		void UpdateCache() const;

	protected:
		Float32				stroke_width_;
		Color				fill_color_;
		Color				stroke_color_;
		Font				text_font_;
		TextStyle			text_style_;
		StrokeStyle			stroke_style_;
		GeometrySink		geo_sink_;
		ImageRenderTarget	rt_;

		mutable bool		cache_expired_;
		mutable Image		image_cached_;
	};
}
