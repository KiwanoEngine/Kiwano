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
#include <kiwano/macros.h>
#include <kiwano/common/common.h>
#include <kiwano/math/math.h>
#include <kiwano/core/keys.h>
#include <kiwano/core/Component.h>

namespace kiwano
{
	class KGE_API Input
		: public Singleton<Input>
		, public UpdateComponent
		, public EventComponent
	{
		KGE_DECLARE_SINGLETON(Input);

	public:
		// 检测键盘或鼠标按键是否正被按下
		bool IsDown(
			int key_or_btn
		);

		// 检测键盘或鼠标按键是否刚被点击
		bool WasPressed(
			int key_or_btn
		);

		// 检测键盘或鼠标按键是否刚抬起
		bool WasReleased(
			int key_or_btn
		);

		// 获得鼠标 x 坐标
		float GetMouseX();

		// 获得鼠标 y 坐标
		float GetMouseY();

		// 获得鼠标坐标
		Point GetMousePos();

	public:
		void SetupComponent() override {}

		void DestroyComponent() override {}

		void AfterUpdate() override;

		void HandleMessage(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam) override;

		void UpdateKey(int, bool);

		void UpdateMousePos(float, float);

	protected:
		Input();

		~Input();

	protected:
		static const int KEY_NUM = 256;

		bool want_update_;
		bool keys_[KEY_NUM];
		bool keys_pressed_[KEY_NUM];
		bool keys_released_[KEY_NUM];
		float mouse_pos_x_;
		float mouse_pos_y_;
	};
}
