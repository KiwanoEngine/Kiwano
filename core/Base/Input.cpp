#include "..\e2dbase.h"
#include "..\e2dtool.h"
#include "..\e2dmanager.h"
#pragma comment(lib, "dinput8.lib")

#define BUFFER_SIZE	256

static char s_KeyBuffer[BUFFER_SIZE] = { 0 };			// 用于保存键盘按键信息缓冲区
static char s_KeyRecordBuffer[BUFFER_SIZE] = { 0 };		// 键盘消息二级缓冲区

e2d::Input * e2d::Input::_instance = nullptr;

e2d::Input::Input()
	: _directInput(false)
	, _keyboardDevice(false)
	, _mouseDevice(false)
	, _mouseState()
	, _mouseStateRecord()
	, _mousePos()
{
	CoInitialize(nullptr);

	ZeroMemory(s_KeyBuffer, sizeof(s_KeyBuffer));
	ZeroMemory(s_KeyRecordBuffer, sizeof(s_KeyRecordBuffer));
	ZeroMemory(&_mouseState, sizeof(_mouseState));
	ZeroMemory(&_mouseStateRecord, sizeof(_mouseStateRecord));

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
			for (int i = 0; i < BUFFER_SIZE; ++i)
				s_KeyRecordBuffer[i] = s_KeyBuffer[i];

			_keyboardDevice->GetDeviceState(sizeof(s_KeyBuffer), (void**)&s_KeyBuffer);
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
			_mouseStateRecord = _mouseState;
			_mouseDevice->GetDeviceState(sizeof(_mouseState), (void**)&_mouseState);
		}
	}

	GetCursorPos(&_mousePos);
	ScreenToClient(Window::getInstance()->getHWnd(), &_mousePos);
}

bool e2d::Input::isDown(KeyCode key)
{
	if (s_KeyBuffer[static_cast<int>(key)] & 0x80)
		return true;
	return false;
}

bool e2d::Input::isPress(KeyCode key)
{
	if ((s_KeyBuffer[static_cast<int>(key)] & 0x80) && 
		!(s_KeyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isRelease(KeyCode key)
{
	if (!(s_KeyBuffer[static_cast<int>(key)] & 0x80) && 
		(s_KeyRecordBuffer[static_cast<int>(key)] & 0x80))
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
		!(_mouseStateRecord.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isRelease(MouseCode code)
{
	if (!(_mouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		(_mouseStateRecord.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

double e2d::Input::getMouseX()
{
	return (double)_mousePos.x;
}

double e2d::Input::getMouseY()
{
	return (double)_mousePos.y;
}

e2d::Point e2d::Input::getMousePos()
{
	return Point((double)_mousePos.x, (double)_mousePos.y);
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
