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
		: want_update_keys_(false)
		, want_update_buttons_(false)
		, buttons_{}
		, keys_{}
	{
	}

	Input::~Input()
	{
	}

	void Input::AfterUpdate()
	{
		if (want_update_keys_)
		{
			want_update_keys_ = false;
			::memcpy(keys_[Prev].data(), keys_[Current].data(), KEY_NUM * sizeof(bool));
		}

		if (want_update_buttons_)
		{
			want_update_buttons_ = false;
			buttons_[Prev] = buttons_[Current];
		}
	}

	bool Input::IsDown(KeyCode key) const
	{
		if (key == KeyCode::Unknown || key == KeyCode::Last)
			return false;
		return keys_[Current][size_t(key)];
	}

	bool Input::WasPressed(KeyCode key) const
	{
		if (key == KeyCode::Unknown || key == KeyCode::Last)
			return false;
		return keys_[Current][size_t(key)] && !keys_[Prev][size_t(key)];
	}

	bool Input::WasReleased(KeyCode key) const
	{
		if (key == KeyCode::Unknown || key == KeyCode::Last)
			return false;
		return !keys_[Current][size_t(key)] && keys_[Prev][size_t(key)];
	}

	bool Input::IsDown(MouseButton btn) const
	{
		if (btn == MouseButton::Last)
			return false;
		return buttons_[Current][size_t(btn)];
	}

	bool Input::WasPressed(MouseButton btn) const
	{
		if (btn == MouseButton::Last)
			return false;
		return buttons_[Current][size_t(btn)] && !buttons_[Prev][size_t(btn)];
	}

	bool Input::WasReleased(MouseButton btn) const
	{
		if (btn == MouseButton::Last)
			return false;
		return !buttons_[Current][size_t(btn)] && buttons_[Prev][size_t(btn)];
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

	void Input::UpdateKey(KeyCode key, bool down)
	{
		if (key == KeyCode::Unknown || key == KeyCode::Last)
			return;

		keys_[Current][size_t(key)] = down;
		want_update_keys_ = true;
	}

	void Input::UpdateButton(MouseButton btn, bool down)
	{
		if (btn == MouseButton::Last)
			return;

		buttons_[Current][size_t(btn)] = down;
		want_update_buttons_ = true;
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
				UpdateButton(dynamic_cast<MouseDownEvent*>(evt)->button, true);
			}
			else if (evt->IsType<MouseUpEvent>())
			{
				UpdateButton(dynamic_cast<MouseUpEvent*>(evt)->button, false);
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
