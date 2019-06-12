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
#include "ActionGroup.h"
#include "ActionTween.h"
#include "Animation.h"

namespace kiwano
{
	struct ActionHelper
	{
		// 设置循环次数
		inline ActionHelper& SetLoops(int loops)							{ base->SetLoops(loops); return (*this); }

		// 设置动作延迟
		inline ActionHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// 设置动作结束回调函数
		inline ActionHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// 设置动作循环结束时的回调函数
		inline ActionHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// 动作结束时移除目标节点
		inline ActionHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// 设置名称
		inline ActionHelper& SetName(String const& name)					{ base->SetName(name); return (*this); }

		// 获取指针
		inline ActionPtr Get() const									{ return base; }

		inline ActionHelper(ActionPtr base)							: base(base) {}

		inline operator ActionPtr() const									{ return base; }

	protected:
		ActionPtr base;
	};

	struct TweenHelper
	{
		// 设置动画持续时长
		inline TweenHelper& SetDuration(Duration dur)						{ base->SetDuration(dur); return (*this); }

		// 设置循环次数
		inline TweenHelper& SetLoops(int loops)								{ base->SetLoops(loops); return (*this); }

		// 设置缓动函数
		inline TweenHelper& SetEaseFunc(EaseFunc ease)						{ base->SetEaseFunc(ease); return (*this); }

		// 设置动作延迟
		inline TweenHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// 设置动作结束回调函数
		inline TweenHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// 设置动作循环结束时的回调函数
		inline TweenHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// 动作结束时移除目标节点
		inline TweenHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// 设置名称
		inline TweenHelper& SetName(String const& name)						{ base->SetName(name); return (*this); }

		// 获取指针
		inline ActionTweenPtr Get() const							{ return base; }
		
		inline TweenHelper(ActionTweenPtr base)						: base(base) {}

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
			MoveBy(Point const& vector)
		{
			return TweenHelper(new kiwano::ActionMoveBy(0, vector));
		}

		static inline TweenHelper
			MoveTo(Point const& pos)
		{
			return TweenHelper(new kiwano::ActionMoveTo(0, pos));
		}

		static inline TweenHelper
		JumpBy(
			Point const& pos,	/* 目的坐标 */
			float height,		/* 跳跃高度 */
			int jumps = 1)		/* 跳跃次数 */
		{
			return TweenHelper(new kiwano::ActionJumpBy(0, pos, height, jumps));
		}

		static inline TweenHelper
		JumpTo(
			Point const& pos,	/* 目的坐标 */
			float height,		/* 跳跃高度 */
			int jumps = 1)		/* 跳跃次数 */
		{
			return TweenHelper(new kiwano::ActionJumpTo(0, pos, height, jumps));
		}

		static inline TweenHelper
			ScaleBy(float scale)
		{
			return TweenHelper(new kiwano::ActionScaleBy(0, scale));
		}

		static inline TweenHelper
			ScaleBy(float scale_x, float scale_y)
		{
			return TweenHelper(new kiwano::ActionScaleBy(0, scale_x, scale_y));
		}

		static inline TweenHelper
			ScaleTo(float scale)
		{
			return TweenHelper(new kiwano::ActionScaleTo(0, scale));
		}

		static inline TweenHelper
			ScaleTo(float scale_x, float scale_y)
		{
			return TweenHelper(new kiwano::ActionScaleTo(0, scale_x, scale_y));
		}

		static inline TweenHelper
			OpacityBy(float opacity)
		{
			return TweenHelper(new kiwano::ActionOpacityBy(0, opacity));
		}

		static inline TweenHelper
			OpacityTo(float opacity)
		{
			return TweenHelper(new kiwano::ActionOpacityTo(0, opacity));
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
			RotateBy(float rotation)
		{
			return TweenHelper(new kiwano::ActionRotateBy(0, rotation));
		}

		static inline TweenHelper
			RotateTo(float rotation)
		{
			return TweenHelper(new kiwano::ActionRotateTo(0, rotation));
		}

		static inline TweenHelper
		Path(
			GeometryPtr geo,			/* 几何图形 */
			bool rotating = false,		/* 沿路径切线方向旋转 */
			float start = 0.f,			/* 起点 */
			float end = 1.f)			/* 终点 */
		{
			return TweenHelper(new kiwano::ActionPath(0, geo, rotating, start, end));
		}

		static inline TweenHelper
			Animation(FramesPtr frames)
		{
			return TweenHelper(new kiwano::Animation(0, frames));
		}

		static inline TweenHelper
			Custom(kiwano::ActionCustom::TweenFunc tween_func)
		{
			return TweenHelper(new kiwano::ActionCustom(0, tween_func));
		}

		static inline ActionHelper
			Delay(Duration delay)
		{
			return ActionHelper(new kiwano::ActionDelay(delay));
		}

		static inline ActionHelper
			Group(Array<ActionPtr> const& actions, bool sequence = true)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, sequence));
		}

		static inline ActionHelper
			Multiple(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, false));
		}

#pragma warning(push)
#pragma warning(disable : 4996)

		KGE_DEPRECATED("Tween::Sequence is deprecated, use Tween::Group instead")
		static inline ActionHelper
			Sequence(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, true));
		}

		KGE_DEPRECATED("Tween::Spawn is deprecated, use Tween::Multiple instead")
		static inline ActionHelper
			Spawn(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new kiwano::ActionGroup(actions, false));
		}

#pragma warning(pop)
	};
}
