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
#include "Action.hpp"
#include "logs.h"
#include "../math/ease.hpp"

namespace easy2d
{
	// 缓动函数
	using EaseFunc = std::function<float(float)>;

	// 缓动函数枚举
	// See https://easings.net for more information
	struct Ease
	{
		static EaseFunc Linear;			// 线性
		static EaseFunc EaseIn;			// 由慢变快
		static EaseFunc EaseOut;		// 由快变慢
		static EaseFunc EaseInOut;		// 由慢变快, 再由快变慢
		static EaseFunc ExpoIn;			// 由慢变极快
		static EaseFunc ExpoOut;		// 由极快变慢
		static EaseFunc ExpoInOut;		// 由慢至极快, 再由极快边慢
		static EaseFunc ElasticIn;		// 自起点赋予弹性
		static EaseFunc ElasticOut;		// 自终点赋予弹性
		static EaseFunc ElasticInOut;	// 再起点和终点赋予弹性
		static EaseFunc BounceIn;		// 自起点赋予反弹力
		static EaseFunc BounceOut;		// 自终点赋予反弹力
		static EaseFunc BounceInOut;	// 在起点和终点赋予反弹力
		static EaseFunc BackIn;
		static EaseFunc BackOut;
		static EaseFunc BackInOut;
		static EaseFunc QuadIn;
		static EaseFunc QuadOut;
		static EaseFunc QuadInOut;
		static EaseFunc CubicIn;
		static EaseFunc CubicOut;
		static EaseFunc CubicInOut;
		static EaseFunc QuartIn;
		static EaseFunc QuartOut;
		static EaseFunc QuartInOut;
		static EaseFunc QuintIn;
		static EaseFunc QuintOut;
		static EaseFunc QuintInOut;
		static EaseFunc SineIn;
		static EaseFunc SineOut;
		static EaseFunc SineInOut;
	};

	inline EaseFunc MakeEaseIn(float rate) { return std::bind(math::EaseIn, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseOut(float rate) { return std::bind(math::EaseOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseInOut(float rate) { return std::bind(math::EaseInOut, std::placeholders::_1, rate); }
	inline EaseFunc MakeEaseElasticIn(float period) { return std::bind(math::EaseElasticIn, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticOut(float period) { return std::bind(math::EaseElasticOut, std::placeholders::_1, period); }
	inline EaseFunc MakeEaseElasticInOut(float period) { return std::bind(math::EaseElasticInOut, std::placeholders::_1, period); }


	// 补间动画
	class ActionTween
		: public Action
	{
	public:
		ActionTween();

		ActionTween(
			Duration duration,
			EaseFunc func
		);

		// 自定义缓动函数
		void SetEaseFunc(
			EaseFunc const& func
		);

		EaseFunc const& GetEaseFunc() const;

		void Reset() override;

		Duration GetDuration() const;

		void SetDuration(Duration duration);

	protected:
		void Init(Node* target) override;

		void Update(Node* target, Duration dt) override;

		virtual void UpdateStep(Node* target, float step) = 0;

	protected:
		Duration duration_;
		Duration elapsed_;
		EaseFunc ease_func_;
	};


	// 相对位移动作
	class MoveBy
		: public ActionTween
	{
	public:
		MoveBy(
			Duration duration,		/* 持续时长 */
			Point const& vector,	/* 移动距离 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// 位移动作
	class MoveTo
		: public MoveBy
	{
	public:
		MoveTo(
			Duration duration,		/* 持续时长 */
			Point const& pos,		/* 目的坐标 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in MoveTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// 相对跳跃动作
	class JumpBy
		: public ActionTween
	{
	public:
		JumpBy(
			Duration duration,		/* 持续时长 */
			Point const& vec,		/* 跳跃距离 */
			float height,			/* 跳跃高度 */
			int jumps = 1,			/* 跳跃次数 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		Point	start_pos_;
		Point	delta_pos_;
		float	height_;
		int		jumps_;
		Point	prev_pos_;
	};


	// 跳跃动作
	class JumpTo
		: public JumpBy
	{
	public:
		JumpTo(
			Duration duration,		/* 持续时长 */
			Point const& pos,		/* 目的坐标 */
			float height,			/* 跳跃高度 */
			int jumps = 1,			/* 跳跃次数 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in JumpTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// 相对缩放动作
	class ScaleBy
		: public ActionTween
	{
	public:
		ScaleBy(
			Duration duration,		/* 持续时长 */
			float scale,			/* 相对变化值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		ScaleBy(
			Duration duration,		/* 持续时长 */
			float scale_x,			/* 横向缩放相对变化值 */
			float scale_y,			/* 纵向缩放相对变化值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		float	start_scale_x_;
		float	start_scale_y_;
		float	delta_x_;
		float	delta_y_;
	};


	// 缩放动作
	class ScaleTo
		: public ScaleBy
	{
	public:
		ScaleTo(
			Duration duration,		/* 持续时长 */
			float scale,			/* 目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		ScaleTo(
			Duration duration,		/* 持续时长 */
			float scale_x,			/* 横向缩放目标值 */
			float scale_y,			/* 纵向缩放目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in ScaleTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// 透明度相对渐变动作
	class OpacityBy
		: public ActionTween
	{
	public:
		OpacityBy(
			Duration duration,		/* 持续时长 */
			float opacity,			/* 相对变化值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 透明度渐变动作
	class OpacityTo
		: public OpacityBy
	{
	public:
		OpacityTo(
			Duration duration,		/* 持续时长 */
			float opacity,			/* 目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in OpacityTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// 淡入动作
	class FadeIn
		: public OpacityTo
	{
	public:
		// 创建淡入动作
		explicit FadeIn(
			Duration duration,		/* 持续时长 */
			EaseFunc func = nullptr	/* 速度变化 */
		);
	};


	// 淡出动作
	class FadeOut
		: public OpacityTo
	{
	public:
		// 创建淡出动作
		explicit FadeOut(
			Duration duration,				/* 持续时长 */
			EaseFunc func = Ease::Linear	/* 速度变化 */
		);
	};


	// 相对旋转动作
	class RotateBy
		: public ActionTween
	{
	public:
		RotateBy(
			Duration duration,		/* 持续时长 */
			float rotation,			/* 相对变化值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 旋转动作
	class RotateTo
		: public RotateBy
	{
	public:
		RotateTo(
			Duration duration,		/* 持续时长 */
			float rotation,			/* 目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			logs::Errorln(L"Reverse() not supported in RotateTo");
			return nullptr;
		}

	protected:
		void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// 路径动作
	class PathAction
		: public ActionTween
	{
	public:
		PathAction(
			Duration duration,		/* 持续时长 */
			GeometryPtr const& geo,	/* 几何图形 */
			bool rotating = false,	/* 沿路径切线方向旋转 */
			float start = 0.f,		/* 起点 */
			float end = 1.f,		/* 终点 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(Node* target) override;

		void UpdateStep(Node* target, float step) override;

	protected:
		bool		rotating_;
		float		start_;
		float		end_;
		Point		start_pos_;
		GeometryPtr	geo_;
	};
}
