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
#include <kiwano/2d/action/ActionTween.h>
#include <kiwano/render/Geometry.h>
#include <kiwano/render/GeometrySink.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(ActionWalk);

	/**
	* \addtogroup Actions
	* @{
	*/

	/// \~chinese
	/// @brief 路径行走动画
	class KGE_API ActionWalk
		: public ActionTween
	{
	public:
		/// \~chinese
		/// @brief 构造路径行走动画
		/// @param duration 持续时长
		/// @param rotating 是否沿路径切线方向旋转
		/// @param start 路径起点（百分比）
		/// @param end 路径终点（百分比）
		/// @param func 动画速度缓动函数
		ActionWalk(Duration duration, bool rotating = false, float start = 0.f, float end = 1.f, EaseFunc func = nullptr);

		/// \~chinese
		/// @brief 构造路径行走动画
		/// @param duration 持续时长
		/// @param path 路径几何形状
		/// @param rotating 是否沿路径切线方向旋转
		/// @param start 路径起点（百分比）
		/// @param end 路径终点（百分比）
		/// @param func 动画速度缓动函数
		ActionWalk(Duration duration, Geometry const& path, bool rotating = false, float start = 0.f, float end = 1.f, EaseFunc func = nullptr);

		/// \~chinese
		/// @brief 获取该动画的拷贝对象
		ActionPtr Clone() const override;

		/// \~chinese
		/// @brief 获取该动画的倒转
		ActionPtr Reverse() const override;

		/// \~chinese
		/// @brief 开始添加路线
		void BeginPath();

		/// \~chinese
		/// @brief 结束路线
		/// @param closed 路线是否闭合
		void EndPath(bool closed = false);

		/// \~chinese
		/// @brief 添加一条线段
		/// @param point 线段端点
		void AddLine(Point const& point);

		/// \~chinese
		/// @brief 添加多条线段
		/// @param points 端点集合
		void AddLines(Vector<Point> const& points);

		/// \~chinese
		/// @brief 添加一条三次方贝塞尔曲线
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

		/// \~chinese
		/// @brief 获取路线
		Geometry const& GetPath() const;

		/// \~chinese
		/// @brief 设置路径几何形状
		void SetPath(Geometry const& path);

	protected:
		void Init(Actor* target) override;

		void UpdateTween(Actor* target, float percent) override;

	private:
		bool			rotating_;
		float			start_;
		float			end_;
		float			length_;
		Point			start_pos_;
		Geometry		path_;
		GeometrySink	sink_;
	};

	/** @} */

	inline Geometry const&	ActionWalk::GetPath() const					{ return path_; }

	inline void				ActionWalk::SetPath(Geometry const& path)	{ path_ = path; }
}
