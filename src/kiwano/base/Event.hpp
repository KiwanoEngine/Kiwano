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
#include "keys.hpp"

namespace kiwano
{

	// ����¼�
	struct MouseEvent
	{
		float x;
		float y;
		bool left_btn_down;		// ����Ƿ���
		bool right_btn_down;	// �Ҽ��Ƿ���

		union
		{
			struct	// Events::MouseDown | Events::MouseUp | Events::MouseClick
			{
				int button;
			};

			struct	// Events::MouseWheel
			{
				float wheel;
			};
		};

		static bool Check(UINT type);
	};

	// �����¼�
	struct KeyboardEvent
	{
		int count;
		union
		{
			struct	// Events::KeyDown | Events::KeyUp
			{
				int code;		// enum KeyCode
			};

			struct	// Events::Char
			{
				char c;
			};
		};

		static bool Check(UINT type);
	};

	// �����¼�
	struct WindowEvent
	{
		union
		{
			struct		// Events::WindowMoved
			{
				int x;
				int y;
			};

			struct		// Events::WindowResized
			{
				int width;
				int height;
			};

			struct		// Events::WindowFocusChanged
			{
				bool focus;
			};

			struct		// Events::WindowTitleChanged
			{
				const wchar_t* title;
			};
		};

		static bool Check(UINT type);
	};

	// �Զ����¼�
	struct CustomEvent
	{
		void* data;
	};

	class Actor;

	// �¼�
	struct KGE_API Event
	{
		enum Type : UINT
		{
			First,

			// ����¼�
			MouseFirst,
			MouseMove,			// �ƶ�
			MouseBtnDown,		// ��갴��
			MouseBtnUp,			// ���̧��
			MouseWheel,			// ���ֹ���
			MouseHover,			// �������
			MouseOut,			// ����Ƴ�
			Click,				// �����
			MouseLast,

			// �����¼�
			KeyFirst,
			KeyDown,			// ��������
			KeyUp,				// ����̧��
			Char,				// ����ַ�
			KeyLast,

			// ������Ϣ
			WindowFirst,
			WindowMoved,		// �����ƶ�
			WindowResized,		// ���ڴ�С�仯
			WindowFocusChanged,	// ��û�ʧȥ����
			WindowTitleChanged,	// ����仯
			WindowClosed,		// ���ڱ��ر�
			WindowLast,

			Last
		};

		UINT type;
		Actor* target;

		union
		{
			MouseEvent mouse;
			KeyboardEvent key;
			WindowEvent win;
			CustomEvent custom;
		};

		Event(UINT type = Type::First) : type(type), target(nullptr) {}
	};


	// Check-functions

	inline bool MouseEvent::Check(UINT type)
	{
		return type > Event::MouseFirst && type < Event::MouseLast;
	}

	inline bool KeyboardEvent::Check(UINT type)
	{
		return type > Event::KeyFirst && type < Event::KeyLast;
	}

	inline bool WindowEvent::Check(UINT type)
	{
		return type > Event::WindowFirst && type < Event::WindowLast;
	}

}
