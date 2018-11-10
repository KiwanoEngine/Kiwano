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
#include "Action.h"

namespace easy2d
{
    // 持续动作
	class FiniteTimeAction
		: public Action
	{
	public:
		// 创建特定时长的持续动作
		explicit FiniteTimeAction(
			float duration
		);

		// 重置动作
		virtual void Reset() override;

	protected:
		E2D_DISABLE_COPY(FiniteTimeAction);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

		// 重置动作时间
		virtual void ResetTime() override;

	protected:
		float duration_;
		float delta_;
	};


	// 相对位移动作
	class MoveBy
		: public FiniteTimeAction
	{
	public:
		explicit MoveBy(
			float duration,		/* 持续时长 */
			Point vector		/* 移动距离 */
		);

		// 获取该动作的拷贝对象
		virtual MoveBy * Clone() const override;

		// 获取该动作的倒转
		virtual MoveBy * Reverse() const override;

	protected:
		E2D_DISABLE_COPY(MoveBy);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

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
			float duration,		/* 持续时长 */
			Point pos			/* 目的坐标 */
		);

		// 获取该动作的拷贝对象
		virtual MoveTo * Clone() const override;

		// 获取该动作的倒转
		virtual MoveTo * Reverse() const override
		{
			E2D_WARNING("Reverse() not supported in MoveTo");
			return nullptr;
		}

	protected:
		E2D_DISABLE_COPY(MoveTo);

		// 初始化动作
		virtual void Initialize() override;

	protected:
		Point end_pos_;
	};


	// 相对跳跃动作
	class JumpBy
		: public FiniteTimeAction
	{
	public:
		explicit JumpBy(
			float duration,			/* 持续时长 */
			const Point& vec,		/* 跳跃距离 */
			float height,			/* 跳跃高度 */
			int jumps = 1			/* 跳跃次数 */
		);

		// 获取该动作的拷贝对象
		virtual JumpBy * Clone() const override;

		// 获取该动作的倒转
		virtual JumpBy * Reverse() const override;

	protected:
		E2D_DISABLE_COPY(JumpBy);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

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
			float duration,			/* 持续时长 */
			const Point& pos,		/* 目的坐标 */
			float height,			/* 跳跃高度 */
			int jumps = 1			/* 跳跃次数 */
		);

		// 获取该动作的拷贝对象
		virtual JumpTo * Clone() const override;

		// 获取该动作的倒转
		virtual JumpTo * Reverse() const override
		{
			E2D_WARNING("Reverse() not supported in JumpTo");
			return nullptr;
		}

	protected:
		E2D_DISABLE_COPY(JumpTo);

		// 初始化动作
		virtual void Initialize() override;

	protected:
		Point end_pos_;
	};


	// 相对缩放动作
	class ScaleBy
		: public FiniteTimeAction
	{
	public:
		explicit ScaleBy(
			float duration,		/* 持续时长 */
			float scale			/* 相对变化值 */
		);

		explicit ScaleBy(
			float duration,		/* 持续时长 */
			float scale_x,		/* 横向缩放相对变化值 */
			float scale_y		/* 纵向缩放相对变化值 */
		);

		// 获取该动作的拷贝对象
		virtual ScaleBy * Clone() const override;

		// 获取该动作的倒转
		virtual ScaleBy * Reverse() const override;

	protected:
		E2D_DISABLE_COPY(ScaleBy);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

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
			float duration,		/* 持续时长 */
			float scale			/* 目标值 */
		);

		explicit ScaleTo(
			float duration,		/* 持续时长 */
			float scale_x,		/* 横向缩放目标值 */
			float scale_y		/* 纵向缩放目标值 */
		);

		// 获取该动作的拷贝对象
		virtual ScaleTo * Clone() const override;

		// 获取该动作的倒转
		virtual ScaleTo * Reverse() const override
		{
			E2D_WARNING("Reverse() not supported in ScaleTo");
			return nullptr;
		}

	protected:
		E2D_DISABLE_COPY(ScaleTo);

		// 初始化动作
		virtual void Initialize() override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// 透明度相对渐变动作
	class OpacityBy
		: public FiniteTimeAction
	{
	public:
		explicit OpacityBy(
			float duration,		/* 持续时长 */
			float opacity		/* 相对变化值 */
		);

		// 获取该动作的拷贝对象
		virtual OpacityBy * Clone() const override;

		// 获取该动作的倒转
		virtual OpacityBy * Reverse() const override;

	protected:
		E2D_DISABLE_COPY(OpacityBy);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

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
			float duration,		/* 持续时长 */
			float opacity		/* 目标值 */
		);

		// 获取该动作的拷贝对象
		virtual OpacityTo * Clone() const override;

		// 获取该动作的倒转
		virtual OpacityTo * Reverse() const override
		{
			E2D_WARNING("Reverse() not supported in OpacityTo");
			return nullptr;
		}

	protected:
		E2D_DISABLE_COPY(OpacityTo);

		// 初始化动作
		virtual void Initialize() override;

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
			float duration		/* 持续时长 */
		);

	protected:
		E2D_DISABLE_COPY(FadeIn);
	};


	// 淡出动作
	class FadeOut
		: public OpacityTo
	{
	public:
		// 创建淡出动作
		explicit FadeOut(
			float duration		/* 持续时长 */
		);

	protected:
		E2D_DISABLE_COPY(FadeOut);
	};


	// 相对旋转动作
	class RotateBy
		: public FiniteTimeAction
	{
	public:
		explicit RotateBy(
			float duration,		/* 持续时长 */
			float rotation		/* 相对变化值 */
		);

		// 获取该动作的拷贝对象
		virtual RotateBy * Clone() const override;

		// 获取该动作的倒转
		virtual RotateBy * Reverse() const override;

	protected:
		E2D_DISABLE_COPY(RotateBy);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

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
			float duration,		/* 持续时长 */
			float rotation		/* 目标值 */
		);

		// 获取该动作的拷贝对象
		virtual RotateTo * Clone() const override;

		// 获取该动作的倒转
		virtual RotateTo * Reverse() const override
		{
			E2D_WARNING("Reverse() not supported in RotateTo");
			return nullptr;
		}

	protected:
		E2D_DISABLE_COPY(RotateTo);

		// 初始化动作
		virtual void Initialize() override;

	protected:
		float end_val_;
	};


	// 延时动作
	class Delay
		: public Action
	{
	public:
		explicit Delay(
			float duration	/* 延迟时长（秒） */
		);

		// 获取该动作的拷贝对象
		virtual Delay * Clone() const override;

		// 获取该动作的倒转
		virtual Delay * Reverse() const override;

		// 重置动作
		virtual void Reset() override;

	protected:
		E2D_DISABLE_COPY(Delay);

		// 初始化动作
		virtual void Initialize() override;

		// 更新动作
		virtual void Update() override;

		// 重置动作时间
		virtual void ResetTime() override;

	protected:
		float delay_;
		float delta_;
	};
}
