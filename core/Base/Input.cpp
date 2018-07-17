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

double e2d::Input::getMouseX()
{
	return getMousePos().x;
}

double e2d::Input::getMouseY()
{
	return getMousePos().y;
}

e2d::Point e2d::Input::getMousePos()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(Window::getInstance()->getHWnd(), &mousePos);

	float dpiScaleX = 0.f, dpiScaleY = 0.f;
	Renderer::getFactory()->GetDesktopDpi(&dpiScaleX, &dpiScaleY);

	return Point(static_cast<double>(mousePos.x * 96.f / dpiScaleX), 
		static_cast<double>(mousePos.y * 96.f / dpiScaleX));
}

double e2d::Input::getMouseDeltaX()
{
	return (double)_mouseState.lX;
}

double e2d::Input::getMouseDeltaY()
{
	return (double)_mouseState.lY;
}

double e2d::Input::getMouseDeltaZ()
{
	return (double)_mouseState.lZ;
}
