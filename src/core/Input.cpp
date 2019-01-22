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
	InputDevice::InputDevice()
		: hwnd_(nullptr)
		, scale_x_(1.f)
		, scale_y_(1.f)
	{
		ZeroMemory(keys_, sizeof(keys_));
		ZeroMemory(keys_cache_, sizeof(keys_cache_));
	}

	InputDevice::~InputDevice()
	{
		E2D_LOG(L"Destroying input device");
	}

	HRESULT InputDevice::Init(HWND hwnd, float scale_x, float scale_y, bool debug)
	{
		E2D_LOG(L"Initing input device");

		hwnd_ = hwnd;
		scale_x_ = scale_x;
		scale_y_ = scale_y;

		return S_OK;
	}

	void InputDevice::Update()
	{
		memcpy(keys_cache_, keys_, sizeof(keys_cache_));
		GetKeyboardState(keys_);
	}

	bool InputDevice::IsDown(KeyCode code)
	{
		return !!(keys_[static_cast<int>(code)] & 0x80);
	}

	bool InputDevice::IsDown(MouseButton btn)
	{
		return !!(keys_[static_cast<int>(btn)] & 0x80);
	}

	bool InputDevice::WasPressed(KeyCode code)
	{
		return !(keys_cache_[static_cast<int>(code)] & 0x80)
			&& (keys_[static_cast<int>(code)] & 0x80);
	}

	bool InputDevice::WasPressed(MouseButton btn)
	{
		return !(keys_cache_[static_cast<int>(btn)] & 0x80)
			&& (keys_[static_cast<int>(btn)] & 0x80);
	}

	bool InputDevice::WasReleased(KeyCode code)
	{
		return (keys_cache_[static_cast<int>(code)] & 0x80)
			&& !(keys_[static_cast<int>(code)] & 0x80);
	}

	bool InputDevice::WasReleased(MouseButton btn)
	{
		return (keys_cache_[static_cast<int>(btn)] & 0x80)
			&& !(keys_[static_cast<int>(btn)] & 0x80);
	}

	float InputDevice::GetMouseX()
	{
		POINT pos;
		::GetCursorPos(&pos);
		::ScreenToClient(hwnd_, &pos);
		return pos.x * scale_x_;
	}

	float InputDevice::GetMouseY()
	{
		POINT pos;
		::GetCursorPos(&pos);
		::ScreenToClient(hwnd_, &pos);
		return pos.y * scale_y_;
	}

	Point InputDevice::GetMousePos()
	{
		POINT pos;
		::GetCursorPos(&pos);
		::ScreenToClient(hwnd_, &pos);
		return Point{ pos.x * scale_x_, pos.y * scale_y_ };
	}
}