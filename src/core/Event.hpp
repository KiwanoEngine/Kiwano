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

	// ����¼�
	struct MouseEvent
	{
		enum Type : EventType
		{
			First = WM_MOUSEFIRST,

			Move,	// �ƶ�
			Down,	// ����
			Up,		// ̧��
			Wheel,	// ���ֹ���

			Hover,	// �������
			Out,	// ����Ƴ�
			Click,	// �����

			Last	// ������־
		};

		float x;
		float y;
		bool left_btn_down;		// ����Ƿ���
		bool right_btn_down;	// �Ҽ��Ƿ���

		struct
		{
			MouseButton button;	// ������Ϣ����Ϊ Down | Up | Click ʱ��Ч
		};

		struct
		{
			float wheel_delta;	// ������Ϣ����Ϊ Wheel ʱ��Ч
		};

		static inline bool Check(EventType type)
		{
			return type > Type::First && type < Type::Last;
		}
	};

	// �����¼�
	struct KeyboardEvent
	{
		enum Type : UINT
		{
			First = WM_KEYFIRST,

			Down,	// ������
			Up,		// ��̧��

			Last
		};

		KeyCode code;
		int count;

		static inline bool Check(UINT type)
		{
			return type > Type::First && type < Type::Last;
		}
	};

	// �����¼�
	struct WindowEvent
	{
	public:
		enum Type : EventType
		{
			First = WM_NULL,

			Moved,			// �����ƶ�
			Resized,		// ���ڴ�С�仯
			FocusChanged,	// ��û�ʧȥ����
			TitleChanged,	// ����仯
			Closed,			// ���ڱ��ر�

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

	// �¼�
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
