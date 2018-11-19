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
	namespace devices
	{
		InputDevice::InputDevice()
			: initialized(false)
			, hwnd_(nullptr)
			, scale_x_(1.f)
			, scale_y_(1.f)
		{
			ZeroMemory(keys_, sizeof(keys_));
			ZeroMemory(keys_cache_, sizeof(keys_cache_));
		}

		InputDevice::~InputDevice()
		{
			E2D_LOG("Destroying input device");
		}

		void InputDevice::Init(HWND hwnd, float scale_x, float scale_y, bool debug)
		{
			if (initialized)
				return;

			E2D_LOG("Initing input device");

			hwnd_ = hwnd;
			scale_x_ = scale_x;
			scale_y_ = scale_y;

			initialized = true;
		}

		void InputDevice::Update()
		{
			memcpy(keys_cache_, keys_, sizeof(keys_cache_));
			GetKeyboardState(keys_);

			POINT client_cursor_pos;
			GetCursorPos(&client_cursor_pos);
			ScreenToClient(hwnd_, &client_cursor_pos);

			mouse_pos_ = Point(client_cursor_pos.x * scale_x_, client_cursor_pos.y * scale_y_);
		}

		bool InputDevice::IsDown(KeyCode code)
		{
			if (keys_[static_cast<int>(code)] & 0x80)
				return true;
			return false;
		}

		bool InputDevice::IsDown(MouseCode code)
		{
			if (keys_[static_cast<int>(code)] & 0x80)
				return true;
			return false;
		}

		bool InputDevice::WasPressed(KeyCode code)
		{
			if (keys_cache_[static_cast<int>(code)] & 0x80 &&
				!(keys_[static_cast<int>(code)] & 0x80))
				return true;
			return false;
		}

		bool InputDevice::WasPressed(MouseCode code)
		{
			if (keys_cache_[static_cast<int>(code)] & 0x80 &&
				!(keys_[static_cast<int>(code)] & 0x80))
				return true;
			return false;
		}

		float InputDevice::GetMouseX()
		{
			return mouse_pos_.x;
		}

		float InputDevice::GetMouseY()
		{
			return mouse_pos_.y;
		}

		Point InputDevice::GetMousePos()
		{
			return mouse_pos_;
		}
	}
}