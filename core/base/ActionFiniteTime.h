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
    // 持续动作
	class FiniteTimeAction
		: public Action
	{
		E2D_DISABLE_COPY(FiniteTimeAction);

	public:
		// 创建特定时长的持续动作
		explicit FiniteTimeAction(
			Duration const& duration
		);

		// 重置动作
		virtual void Reset() override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Duration duration_;
		float process_;
	};


	// 相对位移动作
	class MoveBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(MoveBy);

	public:
		explicit MoveBy(
			Duration const& duration,	/* 持续时长 */
			Point const& vector			/* 移动距离 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// 位移动作
	class MoveTo
		: public MoveBy
	{
		E2D_DISABLE_COPY(MoveTo);

	public:
		explicit MoveTo(
			Duration const& duration,	/* 持续时长 */
			Point const& pos			/* 目的坐标 */
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
		// 初始化动作
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// 相对跳跃动作
	class JumpBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(JumpBy);

	public:
		explicit JumpBy(
			Duration const& duration,	/* 持续时长 */
			Point const& vec,			/* 跳跃距离 */
			float height,				/* 跳跃高度 */
			int jumps = 1				/* 跳跃次数 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

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
		E2D_DISABLE_COPY(JumpTo);

	public:
		explicit JumpTo(
			Duration const& duration,	/* 持续时长 */
			Point const& pos,			/* 目的坐标 */
			float height,				/* 跳跃高度 */
			int jumps = 1				/* 跳跃次数 */
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
		// 初始化动作
		virtual void Init(Node* target) override;

	protected:
		Point end_pos_;
	};


	// 相对缩放动作
	class ScaleBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(ScaleBy);

	public:
		explicit ScaleBy(
			Duration const& duration,	/* 持续时长 */
			float scale					/* 相对变化值 */
		);

		explicit ScaleBy(
			Duration const& duration,	/* 持续时长 */
			float scale_x,				/* 横向缩放相对变化值 */
			float scale_y				/* 纵向缩放相对变化值 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

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
		E2D_DISABLE_COPY(ScaleTo);

	public:
		explicit ScaleTo(
			Duration const& duration,	/* 持续时长 */
			float scale					/* 目标值 */
		);

		explicit ScaleTo(
			Duration const& duration,	/* 持续时长 */
			float scale_x,				/* 横向缩放目标值 */
			float scale_y				/* 纵向缩放目标值 */
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
		// 初始化动作
		virtual void Init(Node* target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// 透明度相对渐变动作
	class OpacityBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(OpacityBy);

	public:
		explicit OpacityBy(
			Duration const& duration,	/* 持续时长 */
			float opacity				/* 相对变化值 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 透明度渐变动作
	class OpacityTo
		: public OpacityBy
	{
		E2D_DISABLE_COPY(OpacityTo);

	public:
		explicit OpacityTo(
			Duration const& duration,	/* 持续时长 */
			float opacity				/* 目标值 */
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
		// 初始化动作
		virtual void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// 淡入动作
	class FadeIn
		: public OpacityTo
	{
		E2D_DISABLE_COPY(FadeIn);

	public:
		// 创建淡入动作
		explicit FadeIn(
			Duration const& duration	/* 持续时长 */
		);
	};


	// 淡出动作
	class FadeOut
		: public OpacityTo
	{
		E2D_DISABLE_COPY(FadeOut);

	public:
		// 创建淡出动作
		explicit FadeOut(
			Duration const& duration	/* 持续时长 */
		);
	};


	// 相对旋转动作
	class RotateBy
		: public FiniteTimeAction
	{
		E2D_DISABLE_COPY(RotateBy);

	public:
		explicit RotateBy(
			Duration const& duration,	/* 持续时长 */
			float rotation				/* 相对变化值 */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 旋转动作
	class RotateTo
		: public RotateBy
	{
		E2D_DISABLE_COPY(RotateTo);

	public:
		explicit RotateTo(
			Duration const& duration,	/* 持续时长 */
			float rotation				/* 目标值 */
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
		// 初始化动作
		virtual void Init(Node* target) override;

	protected:
		float end_val_;
	};


	// 延时动作
	class Delay
		: public Action
	{
		E2D_DISABLE_COPY(Delay);

	public:
		explicit Delay(
			Duration const& duration	/* 延迟时长（秒） */
		);

		// 获取该动作的拷贝对象
		virtual spAction Clone() const override;

		// 获取该动作的倒转
		virtual spAction Reverse() const override;

		// 重置动作
		virtual void Reset() override;

	protected:
		// 初始化动作
		virtual void Init(Node* target) override;

		// 更新动作
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		Duration delay_;
		Duration delta_;
	};
}
