#include "..\e2dbase.h"
#include "..\e2dtool.h"
#include "..\e2dmanager.h"
#pragma comment(lib, "dinput8.lib")


e2d::Input::Input()
	: _directInput(false)
	, _keyboardDevice(false)
	, _mouseDevice(false)
{
	CoInitialize(nullptr);

	ZeroMemory(_keyBuffer, sizeof(_keyBuffer));
	ZeroMemory(&_mouseState, sizeof(_mouseState));

	// 初始化接口对象
	HRESULT hr = DirectInput8Create(
		HINST_THISCOMPONENT,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&_directInput,
		nullptr
	);
}

e2d::Input::~Input()
{
	if (_keyboardDevice)
		_keyboardDevice->Unacquire();
	if (_mouseDevice)
		_mouseDevice->Unacquire();

	SafeRelease(_mouseDevice);
	SafeRelease(_keyboardDevice);
	SafeRelease(_directInput);

	CoUninitialize();
}

void e2d::Input::initWithWindow(Window * window)
{
	HWND hwnd = window->getHWnd();

	// 初始化键盘设备
	ThrowIfFailed(
		_directInput->CreateDevice(
			GUID_SysKeyboard,
			&_keyboardDevice,
			nullptr
		)
	);

	_keyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	_keyboardDevice->Acquire();
	_keyboardDevice->Poll();

	// 初始化鼠标设备
	ThrowIfFailed(
		_directInput->CreateDevice(
			GUID_SysMouse,
			&_mouseDevice,
			nullptr
		)
	);

	_mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	_mouseDevice->SetDataFormat(&c_dfDIMouse);
	_mouseDevice->Acquire();
	_mouseDevice->Poll();
}

void e2d::Input::update()
{
	if (_keyboardDevice)
	{
		HRESULT hr = _keyboardDevice->Poll();
		if (FAILED(hr))
		{
			hr = _keyboardDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = _keyboardDevice->Acquire();
		}
		else
		{
			_keyboardDevice->GetDeviceState(sizeof(_keyBuffer), (void**)&_keyBuffer);
		}
	}

	if (_mouseDevice)
	{
		HRESULT hr = _mouseDevice->Poll();
		if (FAILED(hr))
		{
			hr = _mouseDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = _mouseDevice->Acquire();
		}
		else
		{
			_mouseDevice->GetDeviceState(sizeof(_mouseState), (void**)&_mouseState);
		}
	}
}

bool e2d::Input::isDown(KeyCode key)
{
	if (_keyBuffer[static_cast<int>(key)] & 0x80)
		return true;
	return false;
}

bool e2d::Input::isDown(MouseCode code)
{
	if (_mouseState.rgbButtons[static_cast<int>(code)] & 0x80)
		return true;
	return false;
}

float e2d::Input::getMouseX()
{
	return getMousePos().x;
}

float e2d::Input::getMouseY()
{
	return getMousePos().y;
}

e2d::Point e2d::Input::getMousePos()
{
	auto window = Game::getInstance()->getWindow();

	POINT mousePos;
	::GetCursorPos(&mousePos);
	::ScreenToClient(window->getHWnd(), &mousePos);

	float dpi = window->getDpi();
	return Point(mousePos.x * 96.f / dpi, mousePos.y * 96.f / dpi);
}

float e2d::Input::getMouseDeltaX()
{
	return (float)_mouseState.lX;
}

float e2d::Input::getMouseDeltaY()
{
	return (float)_mouseState.lY;
}

float e2d::Input::getMouseDeltaZ()
{
	return (float)_mouseState.lZ;
}
