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
#include "ActionTween.h"
#include "../../renderer/Geometry.h"  // Geometry, GeometrySink

namespace kiwano
{
	// 路线行走动作
	class KGE_API ActionWalk
		: public ActionTween
	{
	public:
		ActionWalk(
			Duration duration,		/* 持续时长 */
			bool rotating = false,	/* 沿路线切线方向旋转 */
			Float32 start = 0.f,		/* 起点 */
			Float32 end = 1.f,		/* 终点 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		ActionWalk(
			Duration duration,		/* 持续时长 */
			Geometry const& path,	/* 路线 */
			bool rotating = false,	/* 沿路线切线方向旋转 */
			Float32 start = 0.f,		/* 起点 */
			Float32 end = 1.f,		/* 终点 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

		// 开始添加路线
		void BeginPath();

		// 结束路线
		void EndPath(
			bool closed = false		/* 路线是否闭合 */
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
			Float32 rotation,			/* 椭圆旋转角度 */
			bool clockwise = true,	/* 顺时针 or 逆时针 */
			bool is_small = true	/* 是否取小于 180° 的弧 */
		);

		// 清除路径
		void ClearPath();

		// 获取路线
		inline Geometry GetPath() const		{ return path_; }

		// 设置路径
		inline void SetPath(Geometry path)	{ path_ = path; }

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, Float32 percent) override;

	protected:
		bool			rotating_;
		Float32			start_;
		Float32			end_;
		Float32			length_;
		Point			start_pos_;
		Geometry		path_;
		GeometrySink	sink_;
	};
}
