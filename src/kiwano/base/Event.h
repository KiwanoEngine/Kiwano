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
#include <kiwano/core/core.h>
#include <kiwano/math/math.h>
#include <kiwano/base/keys.hpp>

namespace kiwano
{
	class Actor;

	// 事件类型
	struct EventType
	{
		inline EventType() : hash(0), type()
		{
		}

		inline EventType(String const& type) : hash(0), type(type)
		{
			hash = type.hash();
		}

		inline bool operator==(const EventType& rhs) const
		{
			return hash == rhs.hash && type == rhs.type;
		}

		size_t hash;
		String type;
	};


	// 事件
	class KGE_API Event
	{
	public:
		const EventType type;

		Event(EventType const& type);
		virtual ~Event();

		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline const _Ty* SafeCast() const
		{
			const _Ty* ptr = dynamic_cast<const _Ty*>(this);
			if (ptr)
			{
				return ptr;
			}
			return nullptr;
		}

		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline _Ty* SafeCast()
		{
			return const_cast<_Ty*>(const_cast<const Event*>(this)->SafeCast<_Ty>());
		}
	};

	// 鼠标事件
	class KGE_API MouseEvent
		: public Event
	{
	public:
		Point pos;
		bool left_btn_down;		// 左键是否按下
		bool right_btn_down;	// 右键是否按下
		Actor* target;

		MouseEvent(EventType const& type);
	};

	// 鼠标移动事件
	class KGE_API MouseMoveEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseMoveEvent();
	};

	// 鼠标按键按下事件
	class KGE_API MouseDownEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseDownEvent();
	};

	// 鼠标按键抬起事件
	class KGE_API MouseUpEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseUpEvent();
	};

	// 鼠标点击事件
	class KGE_API MouseClickEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseClickEvent();
	};

	// 鼠标移入事件
	class KGE_API MouseHoverEvent
		: public MouseEvent
	{
	public:
		MouseHoverEvent();
	};

	// 鼠标移出事件
	class KGE_API MouseOutEvent
		: public MouseEvent
	{
	public:
		MouseOutEvent();
	};

	// 鼠标滚轮事件
	class KGE_API MouseWheelEvent
		: public MouseEvent
	{
	public:
		float wheel;

		MouseWheelEvent();
	};

	// 键盘按下事件
	class KGE_API KeyDownEvent
		: public Event
	{
	public:
		KeyCode::Value code;
		int count;

		KeyDownEvent();
	};

	// 键盘抬起事件
	class KGE_API KeyUpEvent
		: public Event
	{
	public:
		KeyCode::Value code;
		int count;

		KeyUpEvent();
	};

	// 键盘字符事件
	class KGE_API KeyCharEvent
		: public Event
	{
	public:
		char value;
		int count;

		KeyCharEvent();
	};

	// 窗口移动事件
	class KGE_API WindowMovedEvent
		: public Event
	{
	public:
		int x;
		int y;

		WindowMovedEvent();
	};

	// 窗口大小变化事件
	class KGE_API WindowResizedEvent
		: public Event
	{
	public:
		int width;
		int height;

		WindowResizedEvent();
	};

	// 窗口焦点变化事件
	class KGE_API WindowFocusChangedEvent
		: public Event
	{
	public:
		bool focus;

		WindowFocusChangedEvent();
	};

	// 窗口标题更改事件
	class KGE_API WindowTitleChangedEvent
		: public Event
	{
	public:
		String title;

		WindowTitleChangedEvent();
	};

	// 窗口关闭事件
	class KGE_API WindowClosedEvent
		: public Event
	{
	public:
		WindowClosedEvent();
	};


	namespace event
	{
		// 鼠标事件
		extern EventType MouseMove;				// 移动
		extern EventType MouseDown;				// 鼠标按下
		extern EventType MouseUp;				// 鼠标抬起
		extern EventType MouseWheel;			// 滚轮滚动
		extern EventType MouseHover;			// 鼠标移入
		extern EventType MouseOut;				// 鼠标移出
		extern EventType MouseClick;			// 鼠标点击

		// 按键事件
		extern EventType KeyDown;				// 按键按下
		extern EventType KeyUp;					// 按键抬起
		extern EventType KeyChar;				// 输出字符

		// 窗口消息
		extern EventType WindowMoved;			// 窗口移动
		extern EventType WindowResized;			// 窗口大小变化
		extern EventType WindowFocusChanged;	// 获得或失去焦点
		extern EventType WindowTitleChanged;	// 标题变化
		extern EventType WindowClosed;			// 窗口被关闭
	}

}
