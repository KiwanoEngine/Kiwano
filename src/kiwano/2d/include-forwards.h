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
#include "../core/core.h"
#include "../base/time.h"
#include "../base/RefCounter.hpp"
#include "../base/SmartPtr.hpp"
#include "../base/ObjectBase.h"
#include "../base/types.h"
#include "../math/math.h"
#include "../renderer/Color.h"

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Frame);
	KGE_DECLARE_SMART_PTR(FrameSequence);

	KGE_DECLARE_SMART_PTR(Actor);
	KGE_DECLARE_SMART_PTR(Stage);
	KGE_DECLARE_SMART_PTR(Layer);
	KGE_DECLARE_SMART_PTR(Sprite);
	KGE_DECLARE_SMART_PTR(GifSprite);
	KGE_DECLARE_SMART_PTR(Text);
	KGE_DECLARE_SMART_PTR(Canvas);
	KGE_DECLARE_SMART_PTR(ShapeActor);
	KGE_DECLARE_SMART_PTR(LineActor);
	KGE_DECLARE_SMART_PTR(RectActor);
	KGE_DECLARE_SMART_PTR(RoundRectActor);
	KGE_DECLARE_SMART_PTR(CircleActor);
	KGE_DECLARE_SMART_PTR(EllipseActor);
	KGE_DECLARE_SMART_PTR(PathActor);

	KGE_DECLARE_SMART_PTR(Action);
	KGE_DECLARE_SMART_PTR(ActionTween);
	KGE_DECLARE_SMART_PTR(ActionMoveBy);
	KGE_DECLARE_SMART_PTR(ActionMoveTo);
	KGE_DECLARE_SMART_PTR(ActionJumpBy);
	KGE_DECLARE_SMART_PTR(ActionJumpTo);
	KGE_DECLARE_SMART_PTR(ActionScaleBy);
	KGE_DECLARE_SMART_PTR(ActionScaleTo);
	KGE_DECLARE_SMART_PTR(ActionFadeTo);
	KGE_DECLARE_SMART_PTR(ActionFadeIn);
	KGE_DECLARE_SMART_PTR(ActionFadeOut);
	KGE_DECLARE_SMART_PTR(ActionRotateBy);
	KGE_DECLARE_SMART_PTR(ActionRotateTo);
	KGE_DECLARE_SMART_PTR(ActionWalk);
	KGE_DECLARE_SMART_PTR(Animation);
	KGE_DECLARE_SMART_PTR(ActionGroup);
	KGE_DECLARE_SMART_PTR(ActionSpawn);

	KGE_DECLARE_SMART_PTR(Transition);
	KGE_DECLARE_SMART_PTR(FadeTransition);
	KGE_DECLARE_SMART_PTR(EmergeTransition);
	KGE_DECLARE_SMART_PTR(BoxTransition);
	KGE_DECLARE_SMART_PTR(MoveTransition);
	KGE_DECLARE_SMART_PTR(RotationTransition);

	KGE_DECLARE_SMART_PTR(Button);
	KGE_DECLARE_SMART_PTR(Menu);
}
