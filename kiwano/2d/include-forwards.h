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

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Image);
	KGE_DECLARE_SMART_PTR(Frames);

	KGE_DECLARE_SMART_PTR(Geometry);
	KGE_DECLARE_SMART_PTR(LineGeometry);
	KGE_DECLARE_SMART_PTR(RectangleGeometry);
	KGE_DECLARE_SMART_PTR(RoundedRectGeometry);
	KGE_DECLARE_SMART_PTR(CircleGeometry);
	KGE_DECLARE_SMART_PTR(EllipseGeometry);
	KGE_DECLARE_SMART_PTR(PathGeometry);

	KGE_DECLARE_SMART_PTR(Node);
	KGE_DECLARE_SMART_PTR(Scene);
	KGE_DECLARE_SMART_PTR(Layer);
	KGE_DECLARE_SMART_PTR(Sprite);
	KGE_DECLARE_SMART_PTR(Text);
	KGE_DECLARE_SMART_PTR(Canvas);
	KGE_DECLARE_SMART_PTR(GeometryNode);

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
	KGE_DECLARE_SMART_PTR(ActionPath);
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

namespace kiwano
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
		Rect area;
		float opacity;
	};
}
