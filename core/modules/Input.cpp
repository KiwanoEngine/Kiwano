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

#include "..\e2dmodule.h"
#include "..\e2dtool.h"


namespace easy2d
{
	Input::Input(HWND hwnd)
	{
		ZeroMemory(keys_, sizeof(keys_));
	}

	Input::~Input()
	{
	}

	void Input::Update()
	{
		::GetKeyboardState(keys_);

		float dpi = Graphics::GetDpi();
		POINT client_cursor_pos;
		::GetCursorPos(&client_cursor_pos);
		::ScreenToClient(Game::GetInstance()->GetHWnd(), &client_cursor_pos);
		mouse_pos_ = Point(client_cursor_pos.x * 96.f / dpi, client_cursor_pos.y * 96.f / dpi);
	}

	bool Input::IsDown(KeyCode code)
	{
		if (keys_[static_cast<int>(code)] & 0x80)
			return true;
		return false;
	}

	bool Input::IsDown(MouseCode code)
	{
		if (keys_[static_cast<int>(code)] & 0x80)
			return true;
		return false;
	}

	float Input::GetMouseX()
	{
		return mouse_pos_.x;
	}

	float Input::GetMouseY()
	{
		return mouse_pos_.y;
	}

	Point Input::GetMousePos()
	{
		return mouse_pos_;
	}
}