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

namespace easy2d
{
	enum class EaseFunc
	{
		Linear,					// 线性
		EaseIn,					// 由慢变快
		EaseOut,				// 由快变慢
		EaseInOut,				// 由慢变快, 再由快变慢
		EaseExponentialIn,		// 由慢变极快
		EaseExponentialOut,		// 由极快变慢
		EaseExponentialInOut,	// 由慢至极快, 再由极快边慢
		EaseSineIn,				// 由快变慢, 采用正弦变换速度
		EaseSineOut,			// 由慢变快, 采用正弦变换速度
		EaseSineInOut			// 由慢至快, 再由快至慢, 采用正弦变换速度
	};

	class Tween
		: public Action
	{
	public:
		Tween();

		explicit Tween(
			Duration const& duration,
			EaseFunc func
		);

		// 设置速度变化曲线
		void SetEaseFunction(
			EaseFunc func
		);

		// 自定义速度变化曲线
		void SetEaseFunction(
			std::function<float(float)> func
		);

		virtual void Reset() override;

		Duration const& GetDuration() const;

		void SetDuration(Duration const& duration);

	protected:
		virtual void Init(Node* target) override;

		virtual void Update(Node* target, Duration const& dt) override;

		virtual void UpdateStep(Node* target, float step) = 0;

	protected:
		Duration duration_;
		Duration elapsed_;
		EaseFunc ease_type_;
		std::function<float(float)> ease_func_;
	};


	// 相对位移动作
	class MoveBy
		: public Tween
	{
	public:
		explicit MoveBy(
			Duration const& duration,			/* 持续时长 */
			Point const& vector,				/* 移动距离 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

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
		explicit MoveTo(
			Duration const& duration,			/* 持续时长 */
			Point const& pos,					/* 目的坐标 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in MoveTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// 相对跳跃动作
	class JumpBy
		: public Tween
	{
	public:
		explicit JumpBy(
			Duration const& duration,			/* 持续时长 */
			Point const& vec,					/* 跳跃距离 */
			float height,						/* 跳跃高度 */
			int jumps = 1,						/* 跳跃次数 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

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
		explicit JumpTo(
			Duration const& duration,			/* 持续时长 */
			Point const& pos,					/* 目的坐标 */
			float height,						/* 跳跃高度 */
			int jumps = 1,						/* 跳跃次数 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in JumpTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// 相对缩放动作
	class ScaleBy
		: public Tween
	{
	public:
		explicit ScaleBy(
			Duration const& duration,			/* 持续时长 */
			float scale,						/* 相对变化值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		explicit ScaleBy(
			Duration const& duration,			/* 持续时长 */
			float scale_x,						/* 横向缩放相对变化值 */
			float scale_y,						/* 纵向缩放相对变化值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

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
		explicit ScaleTo(
			Duration const& duration,			/* 持续时长 */
			float scale,						/* 目标值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		explicit ScaleTo(
			Duration const& duration,			/* 持续时长 */
			float scale_x,						/* 横向缩放目标值 */
			float scale_y,						/* 纵向缩放目标值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in ScaleTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// 透明度相对渐变动作
	class OpacityBy
		: public Tween
	{
	public:
		explicit OpacityBy(
			Duration const& duration,			/* 持续时长 */
			float opacity,						/* 相对变化值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 透明度渐变动作
	class OpacityTo
		: public OpacityBy
	{
	public:
		explicit OpacityTo(
			Duration const& duration,			/* 持续时长 */
			float opacity,						/* 目标值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in OpacityTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

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
			Duration const& duration,			/* 持续时长 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);
	};


	// 淡出动作
	class FadeOut
		: public OpacityTo
	{
	public:
		// 创建淡出动作
		explicit FadeOut(
			Duration const& duration,			/* 持续时长 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);
	};


	// 相对旋转动作
	class RotateBy
		: public Tween
	{
	public:
		explicit RotateBy(
			Duration const& duration,			/* 持续时长 */
			float rotation,						/* 相对变化值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		virtual void Init(Node* target) override;

		virtual void UpdateStep(Node* target, float step) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 旋转动作
	class RotateTo
		: public RotateBy
	{
	public:
		explicit RotateTo(
			Duration const& duration,			/* 持续时长 */
			float rotation,						/* 目标值 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override
		{
			logs::Errorln("Reverse() not supported in RotateTo");
			return nullptr;
		}

	protected:
		virtual void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// 路径动作
	class PathAction
		: public Tween
	{
	public:
		explicit PathAction(
			Duration const& duration,			/* 持续时长 */
			spGeometry const& geo,				/* 几何图形 */
			bool rotating = false,				/* 沿路径切线方向旋转 */
			float start = 0.f,					/* 起点 */
			float end = 1.f,					/* 终点 */
			EaseFunc func = EaseFunc::Linear	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		virtual void UpdateStep(Node* target, float step) override;

	protected:
		bool		rotating_;
		float		start_;
		float		end_;
		spGeometry	geo_;
	};
}
