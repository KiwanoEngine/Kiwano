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
	{
		ZeroMemory(keys_, sizeof(keys_));
		ZeroMemory(keys_cache_, sizeof(keys_cache_));
	}

	Input::~Input()
	{
		E2D_LOG(L"Destroying input device");
	}

	HRESULT Input::Init(HWND hwnd, bool debug)
	{
		E2D_LOG(L"Initing input device");

		hwnd_ = hwnd;

		return S_OK;
	}

	void Input::Update()
	{
		memcpy(keys_cache_, keys_, sizeof(keys_cache_));
		::GetKeyboardState(keys_);
	}

	bool Input::IsDown(KeyCode code)
	{
		return !!(keys_[static_cast<int>(code)] & 0x80);
	}

	bool Input::IsDown(MouseButton btn)
	{
		return !!(keys_[static_cast<int>(btn)] & 0x80);
	}

	bool Input::WasPressed(KeyCode code)
	{
		return !(keys_cache_[static_cast<int>(code)] & 0x80)
			&& (keys_[static_cast<int>(code)] & 0x80);
	}

	bool Input::WasPressed(MouseButton btn)
	{
		return !(keys_cache_[static_cast<int>(btn)] & 0x80)
			&& (keys_[static_cast<int>(btn)] & 0x80);
	}

	bool Input::WasReleased(KeyCode code)
	{
		return (keys_cache_[static_cast<int>(code)] & 0x80)
			&& !(keys_[static_cast<int>(code)] & 0x80);
	}

	bool Input::WasReleased(MouseButton btn)
	{
		return (keys_cache_[static_cast<int>(btn)] & 0x80)
			&& !(keys_[static_cast<int>(btn)] & 0x80);
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
		POINT pos;
		::GetCursorPos(&pos);
		::ScreenToClient(hwnd_, &pos);
		return Point{ static_cast<float>(pos.x), static_cast<float>(pos.y) };
	}
}