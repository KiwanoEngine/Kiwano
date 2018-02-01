#include "..\ebase.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace e2d;

static IDirectInput8* s_pDirectInput = nullptr;			// DirectInput 接口对象
static IDirectInputDevice8* s_KeyboardDevice = nullptr;	// 键盘设备接口
static char s_KeyBuffer[256] = { 0 };					// 用于保存键盘按键信息缓冲区
static char s_KeySecBuffer[256] = { 0 };				// 键盘消息二级缓冲区

static IDirectInputDevice8* s_MouseDevice = nullptr;	// 鼠标设备接口
static DIMOUSESTATE s_MouseState;						// 鼠标信息存储结构体
static DIMOUSESTATE s_MouseSecState;					// 鼠标信息二级缓冲
static POINT s_MousePosition;							// 鼠标位置存储结构体


void EInput::__uninit()
{
	if (s_KeyboardDevice)
		s_KeyboardDevice->Unacquire();
	if (s_MouseDevice)
		s_MouseDevice->Unacquire();

	SafeReleaseInterface(&s_MouseDevice);
	SafeReleaseInterface(&s_KeyboardDevice);
	SafeReleaseInterface(&s_pDirectInput);
}

bool EInput::__init()
{
	ZeroMemory(s_KeyBuffer, sizeof(s_KeyBuffer));
	ZeroMemory(s_KeySecBuffer, sizeof(s_KeySecBuffer));
	ZeroMemory(&s_MouseState, sizeof(s_MouseState));
	ZeroMemory(&s_MouseSecState, sizeof(s_MouseSecState));

	// 初始化接口对象
	HRESULT hr = DirectInput8Create(
		HINST_THISCOMPONENT,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pDirectInput,
		nullptr
	);

	if (SUCCEEDED(hr))
	{
		// 初始化键盘设备
		hr = s_pDirectInput->CreateDevice(
			GUID_SysKeyboard,
			&s_KeyboardDevice,
			nullptr
		);

		if (SUCCEEDED(hr))
		{
			s_KeyboardDevice->SetCooperativeLevel(
				EWindow::getHWnd(),
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);
			s_KeyboardDevice->SetDataFormat(
				&c_dfDIKeyboard);
			s_KeyboardDevice->Acquire();
			s_KeyboardDevice->Poll();
		}
		else
		{
			MessageBox(nullptr, L"Keyboard not found. The game will now exit.",
				L"Error",
				MB_ICONERROR | MB_OK);
			EGame::quit();
			return false;
		}
	}

	if (SUCCEEDED(hr))
	{
		// 初始化鼠标设备
		hr = s_pDirectInput->CreateDevice(GUID_SysMouse, &s_MouseDevice, nullptr);

		if (SUCCEEDED(hr))
		{
			s_MouseDevice->SetCooperativeLevel(EWindow::getHWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			s_MouseDevice->SetDataFormat(&c_dfDIMouse);
			s_MouseDevice->Acquire();
			s_MouseDevice->Poll();
		}
		else
		{
			MessageBox(nullptr, L"Mouse not found. The game will now exit.",
				L"Error",
				MB_ICONERROR | MB_OK);
			EGame::quit();
			return false;
		}
	}

	return SUCCEEDED(hr);
}

void EInput::__updateDeviceState()
{
	if (s_KeyboardDevice)
	{
		HRESULT hr = s_KeyboardDevice->Poll();
		if (FAILED(hr))
		{
			hr = s_KeyboardDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = s_KeyboardDevice->Acquire();
		}
		else
		{
			for (int i = 0; i < 256; i++)
				s_KeySecBuffer[i] = s_KeyBuffer[i];

			s_KeyboardDevice->GetDeviceState(sizeof(s_KeyBuffer), (void**)&s_KeyBuffer);
		}
	}

	if (s_MouseDevice)
	{
		HRESULT hr = s_MouseDevice->Poll();
		if (FAILED(hr))
		{
			hr = s_MouseDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = s_MouseDevice->Acquire();
		}
		else
		{
			s_MouseSecState = s_MouseState;
			s_MouseDevice->GetDeviceState(sizeof(s_MouseState), (void**)&s_MouseState);
		}
		DIK_0;
	}

	GetCursorPos(&s_MousePosition);
	ScreenToClient(EWindow::getHWnd(), &s_MousePosition);
}

bool EInput::isKeyDown(int nKeyCode)
{
	if (s_KeyBuffer[nKeyCode] & 0x80)
		return true;
	return false;
}

bool EInput::isKeyPress(int nKeyCode)
{
	if ((s_KeyBuffer[nKeyCode] & 0x80) && !(s_KeySecBuffer[nKeyCode] & 0x80))
		return true;
	return false;
}

bool EInput::isKeyRelease(int nKeyCode)
{
	if (!(s_KeyBuffer[nKeyCode] & 0x80) && (s_KeySecBuffer[nKeyCode] & 0x80))
		return true;
	return false;
}

bool EInput::isMouseLButtonDown()
{
	if (s_MouseState.rgbButtons[0] & 0x80)
		return true;
	return false;
}

bool EInput::isMouseRButtonDown()
{
	if (s_MouseState.rgbButtons[1] & 0x80)
		return true;
	return false;
}

bool EInput::isMouseMButtonDown()
{
	if (s_MouseState.rgbButtons[2] & 0x80)
		return true;
	return false;
}

bool EInput::isMouseLButtonPress()
{
	if ((s_MouseState.rgbButtons[0] & 0x80) && !(s_MouseSecState.rgbButtons[0] & 0x80))
		return true;
	return false;
}

bool EInput::isMouseRButtonPress()
{
	if ((s_MouseState.rgbButtons[1] & 0x80) && !(s_MouseSecState.rgbButtons[1] & 0x80))
		return true;
	return false;
}

bool EInput::isMouseMButtonPress()
{
	if ((s_MouseState.rgbButtons[2] & 0x80) && !(s_MouseSecState.rgbButtons[2] & 0x80))
		return true;
	return false;
}

bool EInput::isMouseLButtonRelease()
{
	if (!(s_MouseState.rgbButtons[0] & 0x80) && (s_MouseSecState.rgbButtons[0] & 0x80))
		return true;
	return false;
}

bool EInput::isMouseRButtonRelease()
{
	if (!(s_MouseState.rgbButtons[1] & 0x80) && (s_MouseSecState.rgbButtons[1] & 0x80))
		return true;
	return false;
}

bool EInput::isMouseMButtonRelease()
{
	if (!(s_MouseState.rgbButtons[2] & 0x80) && (s_MouseSecState.rgbButtons[2] & 0x80))
		return true;
	return false;
}

float EInput::getMouseX()
{
	return (float)s_MousePosition.x;
}

float EInput::getMouseY()
{
	return (float)s_MousePosition.y;
}

EPoint EInput::getMousePos()
{
	return EPoint((float)s_MousePosition.x, (float)s_MousePosition.y);
}

float EInput::getMouseDeltaX()
{
	return (float)s_MouseState.lX;
}

float EInput::getMouseDeltaY()
{
	return (float)s_MouseState.lY;
}

float EInput::getMouseDeltaZ()
{
	return (float)s_MouseState.lZ;
}