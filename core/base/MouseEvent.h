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
	// �����Ϣ
	class MouseEvent
	{
	public:
		// �����Ϣ����
		enum class Type : int
		{
			MoveBy = 0x0200,		// ����ƶ�
			LeftDown,			// ����������
			LeftUp,				// ������̧��
			LeftDoubleClick,	// ������˫��
			RightDown,			// ����Ҽ�����
			RightUp,			// ����Ҽ�̧��
			RightDoubleClick,	// ����Ҽ�˫��
			MiddleDown,			// ����м�����
			MiddleUp,			// ����м�̧��
			MiddleDoubleClick,	// ����м�˫��
			Wheel				// ��������
		};

	public:
		explicit MouseEvent(
			UINT message,
			WPARAM w_param,
			LPARAM l_param
		);

		// ��ȡ��������
		float GetX() const;

		// ��ȡ���������
		float GetY() const;

		// ��ȡ�������
		Point GetPosition() const;

		// ��ȡ�¼�����
		MouseEvent::Type GetType() const;

		float GetWheelDelta() const;

		// �������Ƿ���
		bool IsLButtonDown() const;

		// ����Ҽ��Ƿ���
		bool IsRButtonDown() const;

		// ����м��Ƿ���
		bool IsMButtonDown() const;

		// Shift ���Ƿ���
		bool IsShiftDown() const;

		// Ctrl ���Ƿ���
		bool IsCtrlDown() const;

	protected:
		UINT	message_;
		WPARAM	w_param_;
		LPARAM	l_param_;
	};


	// �����Ϣ����ӿ�
	class MouseEventHandler
	{
	public:
		// ���������Ϣ
		virtual void Handle(MouseEvent e) = 0;
	};
}
