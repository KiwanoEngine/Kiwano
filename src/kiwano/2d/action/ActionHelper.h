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
#include "ActionWalk.h"
#include "ActionDelay.h"
#include "ActionGroup.h"
#include "Animation.h"

namespace kiwano
{
	struct ActionHelper
	{
		// 设置循环次数
		inline ActionHelper& SetLoops(Int32 loops)							{ base->SetLoops(loops); return (*this); }

		// 设置动作延迟
		inline ActionHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// 设置动作结束回调函数
		inline ActionHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// 设置动作循环结束时的回调函数
		inline ActionHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// 动作结束时移除目标角色
		inline ActionHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// 设置名称
		inline ActionHelper& SetName(String const& name)					{ base->SetName(name); return (*this); }

		// 获取指针
		inline ActionPtr Get() const										{ return base; }

		inline ActionHelper(ActionPtr base)									: base(base) {}

		inline operator ActionPtr() const									{ return base; }

	protected:
		ActionPtr base;
	};

	struct TweenHelper
	{
		// 设置动画持续时长
		inline TweenHelper& SetDuration(Duration dur)						{ base->SetDuration(dur); return (*this); }

		// 设置循环次数
		inline TweenHelper& SetLoops(Int32 loops)								{ base->SetLoops(loops); return (*this); }

		// 设置缓动函数
		inline TweenHelper& SetEaseFunc(EaseFunc ease)						{ base->SetEaseFunc(ease); return (*this); }

		// 设置动作延迟
		inline TweenHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// 设置动作结束回调函数
		inline TweenHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// 设置动作循环结束时的回调函数
		inline TweenHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// 动作结束时移除目标角色
		inline TweenHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// 设置名称
		inline TweenHelper& SetName(String const& name)						{ base->SetName(name); return (*this); }

		// 获取指针
		inline ActionTweenPtr Get() const									{ return base; }
		
		inline TweenHelper(ActionTweenPtr base)								: base(base) {}

		inline operator ActionPtr() const									{ return base; }

		inline operator ActionTweenPtr() const								{ return base; }

	protected:
		ActionTweenPtr base;
	};

	// Tween actions helper
	struct Tween
	{
	public:
		static inline TweenHelper
		MoveBy(Duration dur, Point const& vector)
		{
			return TweenHelper(new kiwano::ActionMoveBy(dur, vector));
		}

		static inline TweenHelper
		MoveTo(Duration dur, Point const& pos)
		{
			return TweenHelper(new kiwano::ActionMoveTo(dur, pos));
		}

		static inline TweenHelper
		JumpBy(
			Duration dur,
			Point const& pos,	/* 目的坐标 */
			Float32 height,		/* 跳跃高度 */
			Int32 jumps = 1)		/* 跳跃次数 */
		{
			return TweenHelper(new kiwano::ActionJumpBy(dur, pos, height, jumps));
		}

		static inline TweenHelper
		JumpTo(
			Duration dur,
			Point const& pos,	/* 目的坐标 */
			Float32 height,		/* 跳跃高度 */
			Int32 jumps = 1)		/* 跳跃次数 */
		{
			return TweenHelper(new kiwano::ActionJumpTo(dur, pos, height, jumps));
		}

		static inline TweenHelper
		ScaleBy(Duration dur, Float32 scale)
		{
			return TweenHelper(new kiwano::ActionScaleBy(dur, scale));
		}

		static inline TweenHelper
		ScaleBy(Duration dur, Float32 scale_x, Float32 scale_y)
		{
			return TweenHelper(new kiwano::ActionScaleBy(dur, scale_x, scale_y));
		}

		static inline TweenHelper
		ScaleTo(Duration dur, Float32 scale)
		{
			return TweenHelper(new kiwano::ActionScaleTo(dur, scale));
		}

		static inline TweenHelper
		ScaleTo(Duration dur, Float32 scale_x, Float32 scale_y)
		{
			return TweenHelper(new kiwano::ActionScaleTo(dur, scale_x, scale_y));
		}

		static inline TweenHelper
		FadeTo(Duration dur, Float32 opacity)
		{
			return TweenHelper(new kiwano::ActionFadeTo(dur, opacity));
		}

		static inline TweenHelper
		FadeIn(Duration dur)
		{
			return TweenHelper(new kiwano::ActionFadeIn(dur));
		}

		static inline TweenHelper
		FadeOut(Duration dur)
		{
			return TweenHelper(new kiwano::ActionFadeOut(dur));
		}

		static inline TweenHelper
		RotateBy(Duration dur, Float32 rotation)
		{
			return TweenHelper(new kiwano::ActionRotateBy(dur, rotation));
		}

		static inline TweenHelper
		RotateTo(Duration dur, Float32 rotation)
		{
			return TweenHelper(new kiwano::ActionRotateTo(dur, rotation));
		}

		static inline TweenHelper
		Walk(
			Duration duration,		/* 持续时长 */
			Geometry const& geo,	/* 路线 */
			bool rotating = false,	/* 沿路线切线方向旋转 */
			Float32 start = 0.f,		/* 起点 */
			Float32 end = 1.f,		/* 终点 */
			EaseFunc func = nullptr	/* 速度变化 */
		)
		{
			return TweenHelper(new kiwano::ActionWalk(duration, geo, rotating, start, end, func));
		}

		static inline TweenHelper
		Walk(
			Duration duration,			/* 持续时长 */
			GeometrySink& sink,			/* 路线生成器 */
			bool rotating = false,		/* 沿路线切线方向旋转 */
			Float32 start = 0.f,			/* 起点 */
			Float32 end = 1.f,			/* 终点 */
			EaseFunc func = nullptr		/* 速度变化 */
		)
		{
			return TweenHelper(new kiwano::ActionWalk(duration, sink.GetGeometry(), rotating, start, end, func));
		}

		static inline TweenHelper
		Animation(Duration dur, FrameSequencePtr frames)
		{
			return TweenHelper(new kiwano::Animation(dur, frames));
		}

		static inline TweenHelper
		Custom(Duration dur, ActionCustom::TweenFunc tween_func)
		{
			return TweenHelper(new kiwano::ActionCustom(dur, tween_func));
		}

		static inline ActionHelper
		Delay(Duration delay)
		{
			return ActionHelper(new kiwano::ActionDelay(delay));
		}

		static inline ActionHelper
		Group(Vector<ActionPtr> const& actions, bool sequence = true)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, sequence));
		}

		static inline ActionHelper
		Multiple(Vector<ActionPtr> const& actions)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, false));
		}

#pragma warning(push)
#pragma warning(disable : 4996)

		KGE_DEPRECATED("Tween::OpacityBy has been removed, use Tween::FadeTo instead")
		static inline TweenHelper
		OpacityBy(Float32 opacity)
		{
			KGE_ASSERT("Tween::OpacityBy has been removed, use Tween::FadeTo instead");
			return TweenHelper(nullptr);
		}

		KGE_DEPRECATED("Tween::OpacityTo is deprecated, use Tween::FadeTo instead")
		static inline TweenHelper
		OpacityTo(Duration dur, Float32 opacity)
		{
			return TweenHelper(new kiwano::ActionFadeTo(dur, opacity));
		}

		KGE_DEPRECATED("Tween::Sequence is deprecated, use Tween::Group instead")
		static inline ActionHelper
		Sequence(Vector<ActionPtr> const& actions)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, true));
		}

		KGE_DEPRECATED("Tween::Spawn is deprecated, use Tween::Multiple instead")
		static inline ActionHelper
		Spawn(Vector<ActionPtr> const& actions)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, false));
		}

#pragma warning(pop)
	};
}
