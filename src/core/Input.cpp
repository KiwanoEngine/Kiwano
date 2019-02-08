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

#include "input.h"
#include "logs.h"
#include <cstring>

namespace easy2d
{
	Input::Input()
		: hwnd_(nullptr)
		, want_update_(false)
	{
		ZeroMemory(keys_, sizeof(keys_));
		ZeroMemory(keys_pressed_, sizeof(keys_pressed_));
		ZeroMemory(keys_released_, sizeof(keys_released_));
	}

	Input::~Input()
	{
	}

	HRESULT Input::Init(HWND hwnd, bool debug)
	{
		E2D_NOT_USED(debug);

		hwnd_ = hwnd;

		return S_OK;
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

	bool Input::IsDown(int code_or_btn)
	{
		E2D_ASSERT(code_or_btn >= 0 && code_or_btn < 256);
		return keys_[code_or_btn];
	}

	bool Input::WasPressed(int code_or_btn)
	{
		E2D_ASSERT(code_or_btn >= 0 && code_or_btn < 256);
		return keys_pressed_[code_or_btn];
	}

	bool Input::WasReleased(int code_or_btn)
	{
		E2D_ASSERT(code_or_btn >= 0 && code_or_btn < 256);
		return keys_released_[code_or_btn];
	}

	float Input::GetMouseX()
	{
		return GetMousePos().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePos().y;
	}

	Point Input::GetMousePos()
	{
		Point mouse_pos = Point{};
		if (HWND active_window = ::GetForegroundWindow())
		{
			if (active_window == hwnd_ || ::IsChild(active_window, hwnd_))
			{
				POINT pos;
				if (::GetCursorPos(&pos) && ::ScreenToClient(hwnd_, &pos))
					mouse_pos = Point((float)pos.x, (float)pos.y);
			}
		}
		return mouse_pos;
	}
}