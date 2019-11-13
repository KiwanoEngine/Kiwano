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

	// �¼�����
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


	// �¼�
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

	// ����¼�
	class KGE_API MouseEvent
		: public Event
	{
	public:
		Point pos;
		bool left_btn_down;		// ����Ƿ���
		bool right_btn_down;	// �Ҽ��Ƿ���
		Actor* target;

		MouseEvent(EventType const& type);
	};

	// ����ƶ��¼�
	class KGE_API MouseMoveEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseMoveEvent();
	};

	// ��갴�������¼�
	class KGE_API MouseDownEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseDownEvent();
	};

	// ��갴��̧���¼�
	class KGE_API MouseUpEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseUpEvent();
	};

	// ������¼�
	class KGE_API MouseClickEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;

		MouseClickEvent();
	};

	// ��������¼�
	class KGE_API MouseHoverEvent
		: public MouseEvent
	{
	public:
		MouseHoverEvent();
	};

	// ����Ƴ��¼�
	class KGE_API MouseOutEvent
		: public MouseEvent
	{
	public:
		MouseOutEvent();
	};

	// �������¼�
	class KGE_API MouseWheelEvent
		: public MouseEvent
	{
	public:
		float wheel;

		MouseWheelEvent();
	};

	// ���̰����¼�
	class KGE_API KeyDownEvent
		: public Event
	{
	public:
		KeyCode::Value code;
		int count;

		KeyDownEvent();
	};

	// ����̧���¼�
	class KGE_API KeyUpEvent
		: public Event
	{
	public:
		KeyCode::Value code;
		int count;

		KeyUpEvent();
	};

	// �����ַ��¼�
	class KGE_API KeyCharEvent
		: public Event
	{
	public:
		char value;
		int count;

		KeyCharEvent();
	};

	// �����ƶ��¼�
	class KGE_API WindowMovedEvent
		: public Event
	{
	public:
		int x;
		int y;

		WindowMovedEvent();
	};

	// ���ڴ�С�仯�¼�
	class KGE_API WindowResizedEvent
		: public Event
	{
	public:
		int width;
		int height;

		WindowResizedEvent();
	};

	// ���ڽ���仯�¼�
	class KGE_API WindowFocusChangedEvent
		: public Event
	{
	public:
		bool focus;

		WindowFocusChangedEvent();
	};

	// ���ڱ�������¼�
	class KGE_API WindowTitleChangedEvent
		: public Event
	{
	public:
		String title;

		WindowTitleChangedEvent();
	};

	// ���ڹر��¼�
	class KGE_API WindowClosedEvent
		: public Event
	{
	public:
		WindowClosedEvent();
	};


	namespace event
	{
		// ����¼�
		extern EventType MouseMove;				// �ƶ�
		extern EventType MouseDown;				// ��갴��
		extern EventType MouseUp;				// ���̧��
		extern EventType MouseWheel;			// ���ֹ���
		extern EventType MouseHover;			// �������
		extern EventType MouseOut;				// ����Ƴ�
		extern EventType MouseClick;			// �����

		// �����¼�
		extern EventType KeyDown;				// ��������
		extern EventType KeyUp;					// ����̧��
		extern EventType KeyChar;				// ����ַ�

		// ������Ϣ
		extern EventType WindowMoved;			// �����ƶ�
		extern EventType WindowResized;			// ���ڴ�С�仯
		extern EventType WindowFocusChanged;	// ��û�ʧȥ����
		extern EventType WindowTitleChanged;	// ����仯
		extern EventType WindowClosed;			// ���ڱ��ر�
	}

}
