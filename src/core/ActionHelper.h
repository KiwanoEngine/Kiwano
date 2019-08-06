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
#include "Delay.h"

namespace easy2d
{
	struct ActionHelper
	{
		ActionHelper& SetLoopCount(int loop) { this->loop = loop; return (*this); }

		ActionHelper(ActionPtr const& base) : base(base), loop(0) {}

		operator ActionPtr() const
		{
			if (loop)
				return ActionPtr(new (std::nothrow) Loop(base));
			return base;
		}

	private:
		ActionPtr	base;
		int			loop;
	};

	struct TweenActionHelper
	{
		TweenActionHelper& SetDuration(Duration dur) { this->dur = dur; return (*this); }

		TweenActionHelper& SetLoopCount(int loop) { this->loop = loop; return (*this); }

		TweenActionHelper& SetEaseFunc(EaseFunc ease) { this->ease = ease; return (*this); }

		TweenActionHelper(ActionTweenPtr const& base) : base(base), dur(), loop(0), ease(nullptr)
		{
			dur = base->GetDuration();
		}

		operator ActionPtr() const
		{
			base->SetEaseFunc(ease);
			base->SetDuration(dur);

			if (loop)
				return ActionPtr(new (std::nothrow) Loop(base));
			return base;
		}

	private:
		ActionTweenPtr	base;
		Duration		dur;
		int				loop;
		EaseFunc		ease;
	};

	struct Tween
	{
	public:
		static inline TweenActionHelper
			MoveBy(Point const& vector)
		{
			return TweenActionHelper(new easy2d::MoveBy(0, vector));
		}

		static inline TweenActionHelper
			MoveTo(Point const& pos)
		{
			return TweenActionHelper(new easy2d::MoveTo(0, pos));
		}

		static inline TweenActionHelper
		JumpBy(
			Point const& pos,	/* Ŀ������ */
			float height,		/* ��Ծ�߶� */
			int jumps = 1)		/* ��Ծ���� */
		{
			return TweenActionHelper(new easy2d::JumpBy(0, pos, height, jumps));
		}

		static inline TweenActionHelper
		JumpTo(
			Point const& pos,	/* Ŀ������ */
			float height,		/* ��Ծ�߶� */
			int jumps = 1)		/* ��Ծ���� */
		{
			return TweenActionHelper(new easy2d::JumpTo(0, pos, height, jumps));
		}

		static inline TweenActionHelper
			ScaleBy(float scale)
		{
			return TweenActionHelper(new easy2d::ScaleBy(0, scale));
		}

		static inline TweenActionHelper
			ScaleBy(float scale_x, float scale_y)
		{
			return TweenActionHelper(new easy2d::ScaleBy(0, scale_x, scale_y));
		}

		static inline TweenActionHelper
			ScaleTo(float scale)
		{
			return TweenActionHelper(new easy2d::ScaleTo(0, scale));
		}

		static inline TweenActionHelper
			ScaleTo(float scale_x, float scale_y)
		{
			return TweenActionHelper(new easy2d::ScaleTo(0, scale_x, scale_y));
		}

		static inline TweenActionHelper
			OpacityBy(float opacity)
		{
			return TweenActionHelper(new easy2d::OpacityBy(0, opacity));
		}

		static inline TweenActionHelper
			OpacityTo(float opacity)
		{
			return TweenActionHelper(new easy2d::OpacityTo(0, opacity));
		}

		static inline TweenActionHelper
			FadeIn(Duration dur)
		{
			return TweenActionHelper(new easy2d::FadeIn(dur));
		}

		static inline TweenActionHelper
			FadeOut(Duration dur)
		{
			return TweenActionHelper(new easy2d::FadeOut(dur));
		}

		static inline TweenActionHelper
			RotateBy(float rotation)
		{
			return TweenActionHelper(new easy2d::RotateBy(0, rotation));
		}

		static inline TweenActionHelper
			RotateTo(float rotation)
		{
			return TweenActionHelper(new easy2d::RotateTo(0, rotation));
		}

		static inline TweenActionHelper
		Path(
			GeometryPtr const& geo,		/* ����ͼ�� */
			bool rotating = false,		/* ��·�����߷�����ת */
			float start = 0.f,			/* ��� */
			float end = 1.f)			/* �յ� */
		{
			return TweenActionHelper(new easy2d::PathAction(0, geo, rotating, start, end));
		}

		static inline TweenActionHelper
			Animation(FramesPtr const& frames)
		{
			return TweenActionHelper(new easy2d::Animation(0, frames));
		}

		static inline ActionHelper
			Delay(Duration dur)
		{
			return ActionHelper(new easy2d::Delay(dur));
		}

		static inline ActionHelper
			Sequence(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new easy2d::Sequence(actions));
		}

		static inline ActionHelper
			Spawn(Array<ActionPtr> const& actions)
		{
			return ActionHelper(new easy2d::Spawn(actions));
		}
	};
}
