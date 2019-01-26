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
#include "keys.hpp"

namespace easy2d
{
	typedef UINT EventType;

	// 鼠标事件
	struct MouseEvent
	{
		enum Type : EventType
		{
			First = WM_MOUSEFIRST,

			Move,	// 移动
			Down,	// 按下
			Up,		// 抬起
			Wheel,	// 滚轮滚动

			Hover,	// 鼠标移入
			Out,	// 鼠标移出
			Click,	// 鼠标点击

			Last	// 结束标志
		};

		float x;
		float y;
		bool left_btn_down;		// 左键是否按下
		bool right_btn_down;	// 右键是否按下

		struct
		{
			MouseButton button;	// 仅当消息类型为 Down | Up | Click 时有效
		};

		struct
		{
			float wheel_delta;	// 仅当消息类型为 Wheel 时有效
		};

		static inline bool Check(EventType type)
		{
			return type > Type::First && type < Type::Last;
		}
	};

	// 键盘事件
	struct KeyboardEvent
	{
		enum Type : UINT
		{
			First = WM_KEYFIRST,

			Down,	// 键按下
			Up,		// 键抬起

			Last
		};

		KeyCode code;
		int count;

		static inline bool Check(UINT type)
		{
			return type > Type::First && type < Type::Last;
		}
	};

	// 窗口事件
	struct WindowEvent
	{
	public:
		enum Type : EventType
		{
			First = WM_NULL,

			Moved,			// 窗口移动
			Resized,		// 窗口大小变化
			FocusChanged,	// 获得或失去焦点
			TitleChanged,	// 标题变化
			Closed,			// 窗口被关闭

			Last
		};

		union
		{
			struct		// WindowEvent::Moved
			{
				int x;
				int y;
			};

			struct		// WindowEvent::Resized
			{
				int width;
				int height;
			};

			struct		// WindowEvent::FocusChanged
			{
				bool focus;
			};

			struct		// WindowEvent::TitleChanged
			{
				const wchar_t* title;
			};
		};

		static inline bool Check(EventType type)
		{
			return type > Type::First && type < Type::Last;
		}
	};

	// 事件
	struct Event
	{
		EventType type;
		Node* target;

		union
		{
			MouseEvent mouse;
			KeyboardEvent key;
			WindowEvent win;
		};

		Event(EventType type = 0) : type(type), target(nullptr) {}
	};
}
