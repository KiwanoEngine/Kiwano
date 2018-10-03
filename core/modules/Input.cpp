#include "..\e2dmodule.h"
#include "..\e2dtool.h"


e2d::Input::Input(HWND hwnd)
	: direct_input_(nullptr)
	, keyboard_device_(nullptr)
	, mouse_device_(nullptr)
{
	ZeroMemory(key_buffer_, sizeof(key_buffer_));
	ZeroMemory(&mouse_state_, sizeof(mouse_state_));

	// 初始化接口对象
	ThrowIfFailed(
		DirectInput8Create(
			HINST_THISCOMPONENT,
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

e2d::Input::~Input()
{
	if (keyboard_device_)
		keyboard_device_->Unacquire();
	if (mouse_device_)
		mouse_device_->Unacquire();

	SafeRelease(mouse_device_);
	SafeRelease(keyboard_device_);
	SafeRelease(direct_input_);
}

void e2d::Input::Flush()
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

bool e2d::Input::IsDown(KeyCode key)
{
	if (key_buffer_[static_cast<int>(key)] & 0x80)
		return true;
	return false;
}

bool e2d::Input::IsDown(MouseCode code)
{
	if (mouse_state_.rgbButtons[static_cast<int>(code)] & 0x80)
		return true;
	return false;
}

float e2d::Input::GetMouseX()
{
	return GetMousePos().x;
}

float e2d::Input::GetMouseY()
{
	return GetMousePos().y;
}

e2d::Point e2d::Input::GetMousePos()
{
	HDC hdc = ::GetDC(0);
	int dpi_x = GetDeviceCaps(hdc, LOGPIXELSX);
	int dpi_y = GetDeviceCaps(hdc, LOGPIXELSY);

	POINT mousePos;
	::GetCursorPos(&mousePos);
	::ScreenToClient(Game::GetInstance()->GetHWnd(), &mousePos);
	return Point(mousePos.x * 96.f / dpi_x, mousePos.y * 96.f / dpi_y);
}

float e2d::Input::GetMouseDeltaX()
{
	return (float)mouse_state_.lX;
}

float e2d::Input::GetMouseDeltaY()
{
	return (float)mouse_state_.lY;
}

float e2d::Input::GetMouseDeltaZ()
{
	return (float)mouse_state_.lZ;
}
