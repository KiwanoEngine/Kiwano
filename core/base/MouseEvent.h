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
#include "macros.h"
#include "BaseTypes.hpp"

namespace easy2d
{
	// 鼠标消息
	class MouseEvent
	{
	public:
		// 鼠标消息类型
		enum class Type : int
		{
			MoveBy = 0x0200,		// 鼠标移动
			LeftDown,			// 鼠标左键按下
			LeftUp,				// 鼠标左键抬起
			LeftDoubleClick,	// 鼠标左键双击
			RightDown,			// 鼠标右键按下
			RightUp,			// 鼠标右键抬起
			RightDoubleClick,	// 鼠标右键双击
			MiddleDown,			// 鼠标中键按下
			MiddleUp,			// 鼠标中键抬起
			MiddleDoubleClick,	// 鼠标中键双击
			Wheel				// 滑动滚轮
		};

	public:
		explicit MouseEvent(
			UINT message,
			WPARAM w_param,
			LPARAM l_param
		);

		// 获取鼠标横坐标
		float GetX() const;

		// 获取鼠标纵坐标
		float GetY() const;

		// 获取鼠标坐标
		Point GetPosition() const;

		// 获取事件类型
		MouseEvent::Type GetType() const;

		float GetWheelDelta() const;

		// 鼠标左键是否按下
		bool IsLButtonDown() const;

		// 鼠标右键是否按下
		bool IsRButtonDown() const;

		// 鼠标中键是否按下
		bool IsMButtonDown() const;

		// Shift 键是否按下
		bool IsShiftDown() const;

		// Ctrl 键是否按下
		bool IsCtrlDown() const;

	protected:
		UINT	message_;
		WPARAM	w_param_;
		LPARAM	l_param_;
	};


	// 鼠标消息处理接口
	class MouseEventHandler
	{
	public:
		// 处理鼠标消息
		virtual void Handle(MouseEvent e) = 0;
	};
}
