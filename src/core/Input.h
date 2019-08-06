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
#include "include-forwards.h"
#include "keys.hpp"
#include "Singleton.hpp"

namespace easy2d
{
	// ����ֵ
	enum class MouseButton : int
	{
		Left = VK_LBUTTON,	// ������
		Right = VK_RBUTTON,	// ����Ҽ�
		Middle = VK_MBUTTON	// ����м�
	};


	class InputDevice
		: protected Noncopyable
	{
		E2D_DECLARE_SINGLETON(InputDevice);

	public:
		HRESULT Init(HWND hwnd, float scale_x, float scale_y, bool debug);

		// �����̰����Ƿ���������
		bool IsDown(
			KeyCode code
		);

		// �����갴���Ƿ���������
		bool IsDown(
			MouseButton btn
		);

		// �����̰����Ƿ�ձ����
		bool WasPressed(
			KeyCode code
		);

		// �����갴���Ƿ�ձ����
		bool WasPressed(
			MouseButton btn
		);

		// �����̰����Ƿ��̧��
		bool WasReleased(
			KeyCode code
		);

		// �����갴���Ƿ��̧��
		bool WasReleased(
			MouseButton btn
		);

		// ������ x ����
		float GetMouseX();

		// ������ y ����
		float GetMouseY();

		// ����������
		Point GetMousePos();

		void Update();

	protected:
		InputDevice();

		~InputDevice();

	protected:
		HWND	hwnd_;
		float	scale_x_;
		float	scale_y_;
		BYTE	keys_[256];
		BYTE	keys_cache_[256];
	};

	E2D_DECLARE_SINGLETON_TYPE(InputDevice, Input);
}
