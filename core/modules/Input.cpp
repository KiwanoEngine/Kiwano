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
		: direct_input_(nullptr)
		, keyboard_device_(nullptr)
		, mouse_device_(nullptr)
	{
		ZeroMemory(key_buffer_, sizeof(key_buffer_));
		ZeroMemory(&mouse_state_, sizeof(mouse_state_));

		HINSTANCE hinstance = GetModuleHandle(nullptr);

		// 初始化接口对象
		ThrowIfFailed(
			DirectInput8Create(
				hinstance,
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(void**)&direct_input_,
				nullptr
			)
		);

		// 初始化键盘设备
		ThrowIfFailed(
			direct_input_->CreateDevice(
				GUID_SysKeyboard,
				&keyboard_device_,
				nullptr
			)
		);

		keyboard_device_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		keyboard_device_->SetDataFormat(&c_dfDIKeyboard);
		keyboard_device_->Acquire();
		keyboard_device_->Poll();

		// 初始化鼠标设备
		ThrowIfFailed(
			direct_input_->CreateDevice(
				GUID_SysMouse,
				&mouse_device_,
				nullptr
			)
		);

		mouse_device_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		mouse_device_->SetDataFormat(&c_dfDIMouse);
		mouse_device_->Acquire();
		mouse_device_->Poll();
	}

	Input::~Input()
	{
		if (keyboard_device_)
			keyboard_device_->Unacquire();
		if (mouse_device_)
			mouse_device_->Unacquire();

		SafeRelease(mouse_device_);
		SafeRelease(keyboard_device_);
		SafeRelease(direct_input_);
	}

	void Input::Flush()
	{
		if (keyboard_device_)
		{
			HRESULT hr = keyboard_device_->Poll();
			if (FAILED(hr))
			{
				hr = keyboard_device_->Acquire();
				while (hr == DIERR_INPUTLOST)
					hr = keyboard_device_->Acquire();
			}
			else
			{
				keyboard_device_->GetDeviceState(
					sizeof(key_buffer_),
					(void**)&key_buffer_
				);
			}
		}

		if (mouse_device_)
		{
			HRESULT hr = mouse_device_->Poll();
			if (FAILED(hr))
			{
				hr = mouse_device_->Acquire();
				while (hr == DIERR_INPUTLOST)
					hr = mouse_device_->Acquire();
			}
			else
			{
				mouse_device_->GetDeviceState(
					sizeof(mouse_state_),
					(void**)&mouse_state_
				);
			}
		}
	}

	bool Input::IsDown(KeyCode key)
	{
		if (key_buffer_[static_cast<int>(key)] & 0x80)
			return true;
		return false;
	}

	bool Input::IsDown(MouseCode code)
	{
		if (mouse_state_.rgbButtons[static_cast<int>(code)] & 0x80)
			return true;
		return false;
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
		POINT mousePos;
		::GetCursorPos(&mousePos);
		::ScreenToClient(Game::GetInstance()->GetHWnd(), &mousePos);
		float dpi = Graphics::GetDpi();
		return Point(mousePos.x * 96.f / dpi, mousePos.y * 96.f / dpi);
	}

	float Input::GetMouseDeltaX()
	{
		return (float)mouse_state_.lX;
	}

	float Input::GetMouseDeltaY()
	{
		return (float)mouse_state_.lY;
	}

	float Input::GetMouseDeltaZ()
	{
		return (float)mouse_state_.lZ;
	}
}