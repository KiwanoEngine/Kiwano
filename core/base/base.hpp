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
#include "BaseTypes.hpp"
#include "RefCounter.hpp"
#include "intrusive/SmartPointer.hpp"
#include "d2dres.hpp"

#ifndef E2D_DECLARE_SMART_PTR
#define E2D_DECLARE_SMART_PTR(class_name)\
	class class_name;\
	using sp##class_name = ::easy2d::intrusive::SmartPointer< class_name >

#define E2D_DECLARE_NS_SMART_PTR(ns_name, class_name)\
	namespace ns_name\
	{\
		class class_name; \
		using sp##class_name = ::easy2d::intrusive::SmartPointer< class_name >;\
	}
#endif

namespace easy2d
{
	E2D_DECLARE_SMART_PTR(Image);
	E2D_DECLARE_SMART_PTR(Music);
	E2D_DECLARE_SMART_PTR(Task);
	
	E2D_DECLARE_SMART_PTR(Node);
	E2D_DECLARE_SMART_PTR(Scene);
	E2D_DECLARE_SMART_PTR(Sprite);
	E2D_DECLARE_SMART_PTR(Text);
	E2D_DECLARE_SMART_PTR(Canvas);

	E2D_DECLARE_SMART_PTR(Action);
	E2D_DECLARE_SMART_PTR(MoveBy);
	E2D_DECLARE_SMART_PTR(MoveTo);
	E2D_DECLARE_SMART_PTR(JumpBy);
	E2D_DECLARE_SMART_PTR(JumpTo);
	E2D_DECLARE_SMART_PTR(ScaleBy);
	E2D_DECLARE_SMART_PTR(ScaleTo);
	E2D_DECLARE_SMART_PTR(OpacityBy);
	E2D_DECLARE_SMART_PTR(OpacityTo);
	E2D_DECLARE_SMART_PTR(FadeIn);
	E2D_DECLARE_SMART_PTR(FadeOut);
	E2D_DECLARE_SMART_PTR(RotateBy);
	E2D_DECLARE_SMART_PTR(RotateTo);
	E2D_DECLARE_SMART_PTR(Delay);
	E2D_DECLARE_SMART_PTR(Animation);
	E2D_DECLARE_SMART_PTR(Animate);
	E2D_DECLARE_SMART_PTR(CallFunc);
	E2D_DECLARE_SMART_PTR(Loop);
	E2D_DECLARE_SMART_PTR(Sequence);
	E2D_DECLARE_SMART_PTR(Spawn);

	E2D_DECLARE_SMART_PTR(Transition);
	E2D_DECLARE_SMART_PTR(FadeTransition);
	E2D_DECLARE_SMART_PTR(EmergeTransition);
	E2D_DECLARE_SMART_PTR(BoxTransition);
	E2D_DECLARE_SMART_PTR(MoveTransition);
	E2D_DECLARE_SMART_PTR(RotationTransition);

	E2D_DECLARE_NS_SMART_PTR(ui, Button);
	E2D_DECLARE_NS_SMART_PTR(ui, Menu);
}
