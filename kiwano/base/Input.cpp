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

#include "input.h"
#include "logs.h"
#include <cstring>

namespace kiwano
{
	Input::Input()
		: want_update_(false)
		, mouse_pos_x_(0.f)
		, mouse_pos_y_(0.f)
	{
		ZeroMemory(keys_, sizeof(keys_));
		ZeroMemory(keys_pressed_, sizeof(keys_pressed_));
		ZeroMemory(keys_released_, sizeof(keys_released_));
	}

	Input::~Input()
	{
	}

	void Input::Update()
	{
		if (want_update_)
		{
			want_update_ = false;

			ZeroMemory(keys_pressed_, sizeof(keys_pressed_));
			ZeroMemory(keys_released_, sizeof(keys_released_));
		}
	}

	void Input::UpdateKey(int key, bool down)
	{
		if (down && !keys_[key])
			keys_pressed_[key] = true;
		if (!down && keys_[key])
			keys_released_[key] = true;

		keys_[key] = down;

		want_update_ = true;
	}

	void Input::UpdateMousePos(float x, float y)
	{
		mouse_pos_x_ = x;
		mouse_pos_y_ = y;
	}

	bool Input::IsDown(int key_or_btn)
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		return keys_[key_or_btn];
	}

	bool Input::WasPressed(int key_or_btn)
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		return keys_pressed_[key_or_btn];
	}

	bool Input::WasReleased(int key_or_btn)
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		return keys_released_[key_or_btn];
	}

	float Input::GetMouseX()
	{
		return mouse_pos_x_;
	}

	float Input::GetMouseY()
	{
		return mouse_pos_y_;
	}

	Point Input::GetMousePos()
	{
		return Point{ mouse_pos_x_, mouse_pos_y_ };
	}
}