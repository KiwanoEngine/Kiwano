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
#include "Action.h"
#include "../../base/Logger.h"

namespace kiwano
{
	// 缓动函数
	using EaseFunc = Function<float(float)>;

	// 缓动函数枚举
	// See https://easings.net for more information
	struct Ease
	{
		static KGE_API EaseFunc Linear;			// 线性
		static KGE_API EaseFunc EaseIn;			// 由慢变快
		static KGE_API EaseFunc EaseOut;		// 由快变慢
		static KGE_API EaseFunc EaseInOut;		// 由慢变快, 再由快变慢
		static KGE_API EaseFunc ExpoIn;			// 由慢变极快
		static KGE_API EaseFunc ExpoOut;		// 由极快变慢
		static KGE_API EaseFunc ExpoInOut;		// 由慢至极快, 再由极快边慢
		static KGE_API EaseFunc ElasticIn;		// 自起点赋予弹性
		static KGE_API EaseFunc ElasticOut;		// 自终点赋予弹性
		static KGE_API EaseFunc ElasticInOut;	// 再起点和终点赋予弹性
		static KGE_API EaseFunc BounceIn;		// 自起点赋予反弹力
		static KGE_API EaseFunc BounceOut;		// 自终点赋予反弹力
		static KGE_API EaseFunc BounceInOut;	// 在起点和终点赋予反弹力
		static KGE_API EaseFunc BackIn;
		static KGE_API EaseFunc BackOut;
		static KGE_API EaseFunc BackInOut;
		static KGE_API EaseFunc QuadIn;
		static KGE_API EaseFunc QuadOut;
		static KGE_API EaseFunc QuadInOut;
		static KGE_API EaseFunc CubicIn;
		static KGE_API EaseFunc CubicOut;
		static KGE_API EaseFunc CubicInOut;
		static KGE_API EaseFunc QuartIn;
		static KGE_API EaseFunc QuartOut;
		static KGE_API EaseFunc QuartInOut;
		static KGE_API EaseFunc QuintIn;
		static KGE_API EaseFunc QuintOut;
		static KGE_API EaseFunc QuintInOut;
		static KGE_API EaseFunc SineIn;
		static KGE_API EaseFunc SineOut;
		static KGE_API EaseFunc SineInOut;
	};


	// 补间动画
	class KGE_API ActionTween
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

		Duration GetDuration() const;

		void SetDuration(Duration duration);

	protected:
		void Update(ActorPtr target, Duration dt) override;

		virtual void UpdateTween(ActorPtr target, float percent) = 0;

	protected:
		Duration dur_;
		EaseFunc ease_func_;
	};


	// 相对位移动作
	class KGE_API ActionMoveBy
		: public ActionTween
	{
	public:
		ActionMoveBy(
			Duration duration,		/* 持续时长 */
			Point const& vector,	/* 移动距离 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		Point	start_pos_;
		Point	prev_pos_;
		Point	delta_pos_;
	};


	// 位移动作
	class KGE_API ActionMoveTo
		: public ActionMoveBy
	{
	public:
		ActionMoveTo(
			Duration duration,		/* 持续时长 */
			Point const& pos,		/* 目的坐标 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionMoveTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		Point end_pos_;
	};


	// 相对跳跃动作
	class KGE_API ActionJumpBy
		: public ActionTween
	{
	public:
		ActionJumpBy(
			Duration duration,		/* 持续时长 */
			Point const& vec,		/* 跳跃距离 */
			float height,			/* 跳跃高度 */
			int jumps = 1,		/* 跳跃次数 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		Point	start_pos_;
		Point	delta_pos_;
		float	height_;
		int		jumps_;
		Point	prev_pos_;
	};


	// 跳跃动作
	class KGE_API ActionJumpTo
		: public ActionJumpBy
	{
	public:
		ActionJumpTo(
			Duration duration,		/* 持续时长 */
			Point const& pos,		/* 目的坐标 */
			float height,			/* 跳跃高度 */
			int jumps = 1,		/* 跳跃次数 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionJumpTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		Point end_pos_;
	};


	// 相对缩放动作
	class KGE_API ActionScaleBy
		: public ActionTween
	{
	public:
		ActionScaleBy(
			Duration duration,		/* 持续时长 */
			float scale,			/* 相对变化值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		ActionScaleBy(
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
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		float	start_scale_x_;
		float	start_scale_y_;
		float	delta_x_;
		float	delta_y_;
	};


	// 缩放动作
	class KGE_API ActionScaleTo
		: public ActionScaleBy
	{
	public:
		ActionScaleTo(
			Duration duration,		/* 持续时长 */
			float scale,			/* 目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		ActionScaleTo(
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
			KGE_ERROR_LOG(L"Reverse() not supported in ActionScaleTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		float	end_scale_x_;
		float	end_scale_y_;
	};


	// 透明度渐变动作
	class KGE_API ActionFadeTo
		: public ActionTween
	{
	public:
		ActionFadeTo(
			Duration duration,		/* 持续时长 */
			float opacity,			/* 目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionFadeTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		float start_val_;
		float delta_val_;
		float end_val_;
	};


	// 淡入动作
	class KGE_API ActionFadeIn
		: public ActionFadeTo
	{
	public:
		// 创建淡入动作
		explicit ActionFadeIn(
			Duration duration,		/* 持续时长 */
			EaseFunc func = nullptr	/* 速度变化 */
		);
	};


	// 淡出动作
	class KGE_API ActionFadeOut
		: public ActionFadeTo
	{
	public:
		// 创建淡出动作
		explicit ActionFadeOut(
			Duration duration,		/* 持续时长 */
			EaseFunc func = nullptr	/* 速度变化 */
		);
	};


	// 相对旋转动作
	class KGE_API ActionRotateBy
		: public ActionTween
	{
	public:
		ActionRotateBy(
			Duration duration,		/* 持续时长 */
			float rotation,			/* 相对变化值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		float start_val_;
		float delta_val_;
	};


	// 旋转动作
	class KGE_API ActionRotateTo
		: public ActionRotateBy
	{
	public:
		ActionRotateTo(
			Duration duration,		/* 持续时长 */
			float rotation,			/* 目标值 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		virtual ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionRotateTo");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

	protected:
		float end_val_;
	};


	// 自定义动作
	class KGE_API ActionCustom
		: public ActionTween
	{
	public:
		using TweenFunc = Function<void(ActorPtr, float)>;

		ActionCustom(
			Duration duration,		/* 持续时长 */
			TweenFunc tween_func,	/* 过程函数 */
			EaseFunc func = nullptr	/* 速度变化 */
		);

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override
		{
			KGE_ERROR_LOG(L"Reverse() not supported in ActionCustom");
			return nullptr;
		}

	protected:
		void Init(ActorPtr target) override;

		void UpdateTween(ActorPtr target, float percent) override;

	protected:
		TweenFunc tween_func_;
	};

}
