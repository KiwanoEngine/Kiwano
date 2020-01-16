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

#include <kiwano/platform/Input.h>
#include <kiwano/core/Logger.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/MouseEvent.h>

namespace kiwano
{
	Input::Input()
		: want_update_(false)
		, keys_{}
		, keys_pressed_{}
		, keys_released_{}
	{
	}

	Input::~Input()
	{
	}

	void Input::AfterUpdate()
	{
		if (want_update_)
		{
			want_update_ = false;

			keys_pressed_.fill(false);
			keys_released_.fill(false);
		}
	}

	bool Input::IsDown(int key_or_btn) const
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		if (key_or_btn >= 0 && key_or_btn < KEY_NUM)
			return keys_[key_or_btn];
		return false;
	}

	bool Input::WasPressed(int key_or_btn) const
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		if (key_or_btn >= 0 && key_or_btn < KEY_NUM)
			return keys_pressed_[key_or_btn];
		return false;
	}

	bool Input::WasReleased(int key_or_btn) const
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		if (key_or_btn >= 0 && key_or_btn < KEY_NUM)
			return keys_released_[key_or_btn];
		return false;
	}

	float Input::GetMouseX() const
	{
		return mouse_pos_.x;
	}

	float Input::GetMouseY() const
	{
		return mouse_pos_.y;
	}

	Point Input::GetMousePos() const
	{
		return mouse_pos_;
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

	void Input::UpdateMousePos(const Point& pos)
	{
		mouse_pos_ = pos;
	}

	void Input::HandleEvent(Event* evt)
	{
		if (evt->IsType<MouseEvent>())
		{
			if (evt->IsType<MouseMoveEvent>())
			{
				UpdateMousePos(dynamic_cast<MouseMoveEvent*>(evt)->pos);
			}
			else if (evt->IsType<MouseDownEvent>())
			{
				UpdateKey(dynamic_cast<MouseDownEvent*>(evt)->button, true);
			}
			else if (evt->IsType<MouseUpEvent>())
			{
				UpdateKey(dynamic_cast<MouseUpEvent*>(evt)->button, false);
			}
		}
		else if (evt->IsType<KeyEvent>())
		{
			if (evt->IsType<KeyDownEvent>())
			{
				UpdateKey(dynamic_cast<KeyDownEvent*>(evt)->code, true);
			}
			else if (evt->IsType<KeyUpEvent>())
			{
				UpdateKey(dynamic_cast<KeyUpEvent*>(evt)->code, false);
			}
		}
	}
}
