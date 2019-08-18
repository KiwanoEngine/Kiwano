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
		// ����ѭ������
		inline ActionHelper& SetLoops(Int32 loops)							{ base->SetLoops(loops); return (*this); }

		// ���ö����ӳ�
		inline ActionHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// ���ö��������ص�����
		inline ActionHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// ���ö���ѭ������ʱ�Ļص�����
		inline ActionHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// ��������ʱ�Ƴ�Ŀ���ɫ
		inline ActionHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// ��������
		inline ActionHelper& SetName(String const& name)					{ base->SetName(name); return (*this); }

		// ��ȡָ��
		inline ActionPtr Get() const										{ return base; }

		inline ActionHelper(ActionPtr base)									: base(base) {}

		inline operator ActionPtr() const									{ return base; }

	protected:
		ActionPtr base;
	};

	struct TweenHelper
	{
		// ���ö�������ʱ��
		inline TweenHelper& SetDuration(Duration dur)						{ base->SetDuration(dur); return (*this); }

		// ����ѭ������
		inline TweenHelper& SetLoops(Int32 loops)								{ base->SetLoops(loops); return (*this); }

		// ���û�������
		inline TweenHelper& SetEaseFunc(EaseFunc ease)						{ base->SetEaseFunc(ease); return (*this); }

		// ���ö����ӳ�
		inline TweenHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// ���ö��������ص�����
		inline TweenHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// ���ö���ѭ������ʱ�Ļص�����
		inline TweenHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// ��������ʱ�Ƴ�Ŀ���ɫ
		inline TweenHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// ��������
		inline TweenHelper& SetName(String const& name)						{ base->SetName(name); return (*this); }

		// ��ȡָ��
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
			Point const& pos,	/* Ŀ������ */
			Float32 height,		/* ��Ծ�߶� */
			Int32 jumps = 1)		/* ��Ծ���� */
		{
			return TweenHelper(new kiwano::ActionJumpBy(dur, pos, height, jumps));
		}

		static inline TweenHelper
		JumpTo(
			Duration dur,
			Point const& pos,	/* Ŀ������ */
			Float32 height,		/* ��Ծ�߶� */
			Int32 jumps = 1)		/* ��Ծ���� */
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
			Duration duration,		/* ����ʱ�� */
			Geometry const& geo,	/* ·�� */
			bool rotating = false,	/* ��·�����߷�����ת */
			Float32 start = 0.f,		/* ��� */
			Float32 end = 1.f,		/* �յ� */
			EaseFunc func = nullptr	/* �ٶȱ仯 */
		)
		{
			return TweenHelper(new kiwano::ActionWalk(duration, geo, rotating, start, end, func));
		}

		static inline TweenHelper
		Walk(
			Duration duration,			/* ����ʱ�� */
			GeometrySink& sink,			/* ·�������� */
			bool rotating = false,		/* ��·�����߷�����ת */
			Float32 start = 0.f,			/* ��� */
			Float32 end = 1.f,			/* �յ� */
			EaseFunc func = nullptr		/* �ٶȱ仯 */
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
