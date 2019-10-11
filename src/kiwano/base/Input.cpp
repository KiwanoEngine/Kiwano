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

#include <kiwano/base/input.h>
#include <kiwano/base/Logger.h>
#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM

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

	void Input::AfterUpdate()
	{
		if (want_update_)
		{
			want_update_ = false;

			ZeroMemory(keys_pressed_, sizeof(keys_pressed_));
			ZeroMemory(keys_released_, sizeof(keys_released_));
		}
	}

	void Input::HandleMessage(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		//case WM_LBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		//case WM_MBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		//case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		{
			if		(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) { UpdateKey(VK_LBUTTON, (msg == WM_LBUTTONDOWN) ? true : false); }
			else if	(msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) { UpdateKey(VK_RBUTTON, (msg == WM_RBUTTONDOWN) ? true : false); }
			else if	(msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) { UpdateKey(VK_MBUTTON, (msg == WM_MBUTTONDOWN) ? true : false); }
			else if (msg == WM_MOUSEMOVE) { UpdateMousePos(static_cast<float>(GET_X_LPARAM(lparam)), static_cast<float>(GET_Y_LPARAM(lparam))); }

			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			bool down = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
			UpdateKey((int)wparam, down);
		}
		}
	}

	bool Input::IsDown(int key_or_btn)
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		if (key_or_btn >= 0 && key_or_btn < KEY_NUM)
			return keys_[key_or_btn];
		return false;
	}

	bool Input::WasPressed(int key_or_btn)
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		if (key_or_btn >= 0 && key_or_btn < KEY_NUM)
			return keys_pressed_[key_or_btn];
		return false;
	}

	bool Input::WasReleased(int key_or_btn)
	{
		KGE_ASSERT(key_or_btn >= 0 && key_or_btn < KEY_NUM);
		if (key_or_btn >= 0 && key_or_btn < KEY_NUM)
			return keys_released_[key_or_btn];
		return false;
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
