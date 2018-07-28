#include "..\e2dbase.h"
#include "..\e2dtool.h"
#include "..\e2dmanager.h"
#pragma comment(lib, "dinput8.lib")

e2d::Input * e2d::Input::_instance = nullptr;

e2d::Input::Input()
	: _directInput(false)
	, _keyboardDevice(false)
	, _mouseDevice(false)
{
	CoInitialize(nullptr);

	ZeroMemory(_keyBuffer, sizeof(_keyBuffer));
	ZeroMemory(_keyRecordBuffer, sizeof(_keyRecordBuffer));
	ZeroMemory(&_mouseState, sizeof(_mouseState));
	ZeroMemory(&_mouseRecordState, sizeof(_mouseRecordState));

	// 初始化接口对象
	HRESULT hr = DirectInput8Create(
		HINST_THISCOMPONENT,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&_directInput,
		nullptr
	);

	auto window = Window::getInstance();

	if (SUCCEEDED(hr))
	{
		// 初始化键盘设备
		hr = _directInput->CreateDevice(
			GUID_SysKeyboard,
			&_keyboardDevice,
			nullptr
		);

		if (SUCCEEDED(hr))
		{
			_keyboardDevice->SetCooperativeLevel(
				window->getHWnd(),
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);
			_keyboardDevice->SetDataFormat(
				&c_dfDIKeyboard);
			_keyboardDevice->Acquire();
			_keyboardDevice->Poll();
		}
		else
		{
			throw SystemException(L"Keyboard not found!");
		}
	}

	if (SUCCEEDED(hr))
	{
		// 初始化鼠标设备
		hr = _directInput->CreateDevice(GUID_SysMouse, &_mouseDevice, nullptr);

		if (SUCCEEDED(hr))
		{
			_mouseDevice->SetCooperativeLevel(window->getHWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			_mouseDevice->SetDataFormat(&c_dfDIMouse);
			_mouseDevice->Acquire();
			_mouseDevice->Poll();
		}
		else
		{
			throw SystemException(L"Mouse not found!");
		}
	}
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

e2d::Input * e2d::Input::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) Input;
	return _instance;
}

void e2d::Input::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
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
			strcpy_s(_keyRecordBuffer, 256, _keyBuffer);
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
			_mouseRecordState = _mouseState;
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

bool e2d::Input::isPress(KeyCode key)
{
	if ((_keyBuffer[static_cast<int>(key)] & 0x80) && 
		!(_keyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isRelease(KeyCode key)
{
	if (!(_keyBuffer[static_cast<int>(key)] & 0x80) && 
		(_keyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isDown(MouseCode code)
{
	if (_mouseState.rgbButtons[static_cast<int>(code)] & 0x80)
		return true;
	return false;
}

bool e2d::Input::isPress(MouseCode code)
{
	if ((_mouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		!(_mouseRecordState.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isRelease(MouseCode code)
{
	if (!(_mouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		(_mouseRecordState.rgbButtons[static_cast<int>(code)] & 0x80))
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
	HWND hWnd = Window::getInstance()->getHWnd();

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);

	UINT ret = ::GetDpiForWindow(hWnd);
	return Point(mousePos.x * 96.f / ret, mousePos.y * 96.f / ret);
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
