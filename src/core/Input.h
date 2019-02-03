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
	class Input
		: public ISingleton<Input>
	{
		E2D_DECLARE_SINGLETON(Input);

	public:
		// 检测键盘或鼠标按键是否正被按下
		bool IsDown(
			int code_or_btn
		);

		// 检测键盘或鼠标按键是否刚被点击
		bool WasPressed(
			int code_or_btn
		);

		// 检测键盘或鼠标按键是否刚抬起
		bool WasReleased(
			int code_or_btn
		);

		// 获得鼠标 x 坐标
		float GetMouseX();

		// 获得鼠标 y 坐标
		float GetMouseY();

		// 获得鼠标坐标
		Point GetMousePos();

		HRESULT Init(HWND hwnd, bool debug);

		void Update();

		void UpdateKey(int, bool);

	protected:
		Input();

		~Input();

	protected:
		HWND hwnd_;
		bool want_update_;
		bool keys_[256];
		bool keys_pressed_[256];
		bool keys_released_[256];
	};
}
