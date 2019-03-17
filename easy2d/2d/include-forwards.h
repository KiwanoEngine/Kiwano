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
#include "Color.h"
#include "../common/helper.h"
#include "../common/ComPtr.hpp"
#include "../common/Closure.hpp"
#include "../common/Singleton.hpp"
#include "../common/IntrusiveList.hpp"
#include "../base/time.h"
#include "../base/RefCounter.hpp"
#include "../base/SmartPtr.hpp"
#include "../base/Object.h"
#include "../math/helper.h"

namespace easy2d
{
	E2D_DECLARE_SMART_PTR(Image);
	E2D_DECLARE_SMART_PTR(Frames);

	E2D_DECLARE_SMART_PTR(Geometry);
	E2D_DECLARE_SMART_PTR(LineGeometry);
	E2D_DECLARE_SMART_PTR(RectangleGeometry);
	E2D_DECLARE_SMART_PTR(RoundedRectGeometry);
	E2D_DECLARE_SMART_PTR(CircleGeometry);
	E2D_DECLARE_SMART_PTR(EllipseGeometry);
	E2D_DECLARE_SMART_PTR(PathGeometry);

	E2D_DECLARE_SMART_PTR(Node);
	E2D_DECLARE_SMART_PTR(Scene);
	E2D_DECLARE_SMART_PTR(Layer);
	E2D_DECLARE_SMART_PTR(Sprite);
	E2D_DECLARE_SMART_PTR(Text);
	E2D_DECLARE_SMART_PTR(Canvas);
	E2D_DECLARE_SMART_PTR(GeometryNode);

	E2D_DECLARE_SMART_PTR(Action);
	E2D_DECLARE_SMART_PTR(ActionTween);
	E2D_DECLARE_SMART_PTR(ActionMoveBy);
	E2D_DECLARE_SMART_PTR(ActionMoveTo);
	E2D_DECLARE_SMART_PTR(ActionJumpBy);
	E2D_DECLARE_SMART_PTR(ActionJumpTo);
	E2D_DECLARE_SMART_PTR(ActionScaleBy);
	E2D_DECLARE_SMART_PTR(ActionScaleTo);
	E2D_DECLARE_SMART_PTR(ActionOpacityBy);
	E2D_DECLARE_SMART_PTR(ActionOpacityTo);
	E2D_DECLARE_SMART_PTR(ActionFadeIn);
	E2D_DECLARE_SMART_PTR(ActionFadeOut);
	E2D_DECLARE_SMART_PTR(ActionRotateBy);
	E2D_DECLARE_SMART_PTR(ActionRotateTo);
	E2D_DECLARE_SMART_PTR(ActionPath);
	E2D_DECLARE_SMART_PTR(Animation);
	E2D_DECLARE_SMART_PTR(ActionGroup);
	E2D_DECLARE_SMART_PTR(ActionSpawn);

	E2D_DECLARE_SMART_PTR(Transition);
	E2D_DECLARE_SMART_PTR(FadeTransition);
	E2D_DECLARE_SMART_PTR(EmergeTransition);
	E2D_DECLARE_SMART_PTR(BoxTransition);
	E2D_DECLARE_SMART_PTR(MoveTransition);
	E2D_DECLARE_SMART_PTR(RotationTransition);

	E2D_DECLARE_SMART_PTR(Button);
	E2D_DECLARE_SMART_PTR(Menu);
}

namespace easy2d
{
	// 画笔样式
	enum class StrokeStyle : int
	{
		Miter = 0,	/* 斜切 */
		Bevel = 1,	/* 斜角 */
		Round = 2	/* 圆角 */
	};

	// 方向
	enum class Direction : int
	{
		Up,		/* 上 */
		Down,	/* 下 */
		Left,	/* 左 */
		Right	/* 右 */
	};

	// 鼠标指针
	enum class MouseCursor : int
	{
		Arrow,		/* 指针 */
		TextInput,	/* 输入文本 */
		Hand,		/* 手指 */
		SizeAll,
		SizeNESW,
		SizeNS,
		SizeNWSE,
		SizeWE,
	};

	// 文字抗锯齿属性
	enum class TextAntialias
	{
		Default,	// 系统默认
		ClearType,	// ClearType 抗锯齿
		GrayScale,	// 灰度抗锯齿
		None		// 不启用抗锯齿
	};

	// 图层属性
	struct LayerProperties
	{
		math::Rect area;
		float opacity;
	};
}
