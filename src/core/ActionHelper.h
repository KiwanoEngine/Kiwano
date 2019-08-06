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
#include "ActionGroup.h"
#include "ActionTween.h"
#include "Animation.h"

namespace easy2d
{
	struct ActionHelper
	{
		// ����ѭ������
		inline ActionHelper& SetLoops(int loops)							{ base->SetLoops(loops); return (*this); }

		// ���ö����ӳ�
		inline ActionHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// ���ö��������ص�����
		inline ActionHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// ���ö���ѭ������ʱ�Ļص�����
		inline ActionHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// ��������ʱ�Ƴ�Ŀ��ڵ�
		inline ActionHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// ��������
		inline ActionHelper& SetName(String const& name)					{ base->SetName(name); return (*this); }

		// ��ȡָ��
		inline ActionPtr const& Get() const									{ return base; }

		inline ActionHelper(ActionPtr const& base)							: base(base) {}

		inline operator ActionPtr() const									{ return base; }

	protected:
		ActionPtr base;
	};

	struct TweenHelper
	{
		// ���ö�������ʱ��
		inline TweenHelper& SetDuration(Duration dur)						{ base->SetDuration(dur); return (*this); }

		// ����ѭ������
		inline TweenHelper& SetLoops(int loops)								{ base->SetLoops(loops); return (*this); }

		// ���û�������
		inline TweenHelper& SetEaseFunc(EaseFunc ease)						{ base->SetEaseFunc(ease); return (*this); }

		// ���ö����ӳ�
		inline TweenHelper& SetDelay(Duration delay)						{ base->SetDelay(delay); return (*this); }

		// ���ö��������ص�����
		inline TweenHelper& SetDoneCallback(ActionCallback const& cb)		{ base->SetDoneCallback(cb); return (*this); }

		// ���ö���ѭ������ʱ�Ļص�����
		inline TweenHelper& SetLoopDoneCallback(ActionCallback const& cb)	{ base->SetLoopDoneCallback(cb); return (*this); }

		// ��������ʱ�Ƴ�Ŀ��ڵ�
		inline TweenHelper& RemoveTargetWhenDone()							{ base->RemoveTargetWhenDone(); return (*this); }

		// ��������
		inline TweenHelper& SetName(String const& name)						{ base->SetName(name); return (*this); }

		// ��ȡָ��
		inline ActionTweenPtr const& Get() const							{ return base; }
		
		inline TweenHelper(ActionTweenPtr const& base)						: base(base) {}

		inline operator ActionPtr() const									{ return base; }

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
			return TweenHelper(new easy2d::ActionMoveBy(0, vector));
		}

		static inline TweenHelper
			MoveTo(Point const& pos)
		{
			return TweenHelper(new easy2d::ActionMoveTo(0, pos));
		}

		static inline TweenHelper
		JumpBy(
			Point const& pos,	/* Ŀ������ */
			float height,		/* ��Ծ�߶� */
			int jumps = 1)		/* ��Ծ���� */
		{
			return TweenHelper(new easy2d::ActionJumpBy(0, pos, height, jumps));
		}

		static inline TweenHelper
		JumpTo(
			Point const& pos,	/* Ŀ������ */
			float height,		/* ��Ծ�߶� */
			int jumps = 1)		/* ��Ծ���� */
		{
			return TweenHelper(new easy2d::ActionJumpTo(0, pos, height, jumps));
		}

		static inline TweenHelper
			ScaleBy(float scale)
		{
			return TweenHelper(new easy2d::ActionScaleBy(0, scale));
		}

		static inline TweenHelper
			ScaleBy(float scale_x, float scale_y)
		{
			return TweenHelper(new easy2d::ActionScaleBy(0, scale_x, scale_y));
		}

		static inline TweenHelper
			ScaleTo(float scale)
		{
			return TweenHelper(new easy2d::ActionScaleTo(0, scale));
		}

		static inline TweenHelper
			ScaleTo(float scale_x, float scale_y)
		{
			return TweenHelper(new easy2d::ActionScaleTo(0, scale_x, scale_y));
		}

		static inline TweenHelper
			OpacityBy(float opacity)
		{
			return TweenHelper(new easy2d::ActionOpacityBy(0, opacity));
		}

		static inline TweenHelper
			OpacityTo(float opacity)
		{
			return TweenHelper(new easy2d::ActionOpacityTo(0, opacity));
		}

		static inline TweenHelper
			FadeIn(Duration dur)
		{
			return TweenHelper(new easy2d::ActionFadeIn(dur));
		}

		static inline TweenHelper
			FadeOut(Duration dur)
		{
			return TweenHelper(new easy2d::ActionFadeOut(dur));
		}

		static inline TweenHelper
			RotateBy(float rotation)
		{
			return TweenHelper(new easy2d::ActionRotateBy(0, rotation));
		}

		static inline TweenHelper
			RotateTo(float rotation)
		{
			return TweenHelper(new easy2d::ActionRotateTo(0, rotation));
		}

		static inline TweenHelper
		Path(
			GeometryPtr const& geo,		/* ����ͼ�� */
			bool rotating = false,		/* ��·�����߷�����ת */
			float start = 0.f,			/* ��� */
			float end = 1.f)			/* �յ� */
		{
			return TweenHelper(new easy2d::ActionPath(0, geo, rotating, start, end));
		}

		static inline TweenHelper
			Animation(FramesPtr const& frames)
		{
			return TweenHelper(new easy2d::Animation(0, frames));
		}

		static inline ActionHelper
			Group(Array<ActionPtr> const& actions, bool sequence = true)
		{
			return ActionHelper(new easy2d::ActionGroup(actions, sequence));
		}

		static inline ActionHelper
			Multiple(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new easy2d::ActionGroup(actions, false));
		}

		E2D_DEPRECATED("Tween::Sequence is deprecated, use Tween::Group instead")
		static inline ActionHelper
			Sequence(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new easy2d::ActionGroup(actions, true));
		}

		E2D_DEPRECATED("Tween::Spawn is deprecated, use Tween::Multiple instead")
		static inline ActionHelper
			Spawn(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new easy2d::ActionGroup(actions, false));
		}
	};
}
