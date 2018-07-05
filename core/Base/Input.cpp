#include "..\e2dbase.h"
#include "..\e2dtool.h"
#include "..\e2dmanager.h"
#pragma comment(lib, "dinput8.lib")

#define BUFFER_SIZE	256

static char s_KeyBuffer[BUFFER_SIZE] = { 0 };			// 用于保存键盘按键信息缓冲区
static char s_KeyRecordBuffer[BUFFER_SIZE] = { 0 };		// 键盘消息二级缓冲区
static std::vector<e2d::Listener*> s_vListeners;		// 监听器容器

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
	Input::__updateDeviceState();
	Input::__updateListeners();
}

void e2d::Input::__updateDeviceState()
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

bool e2d::Input::isDown(Key key)
{
	if (s_KeyBuffer[static_cast<int>(key)] & 0x80)
		return true;
	return false;
}

bool e2d::Input::isPress(Key key)
{
	if ((s_KeyBuffer[static_cast<int>(key)] & 0x80) && 
		!(s_KeyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isRelease(Key key)
{
	if (!(s_KeyBuffer[static_cast<int>(key)] & 0x80) && 
		(s_KeyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isDown(Mouse code)
{
	if (_mouseState.rgbButtons[static_cast<int>(code)] & 0x80)
		return true;
	return false;
}

bool e2d::Input::isPress(Mouse code)
{
	if ((_mouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		!(_mouseStateRecord.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

bool e2d::Input::isRelease(Mouse code)
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

e2d::Listener * e2d::Input::addListener(const Function& func, const String& name, bool paused)
{
	auto listener = Create<Listener>(func, name, paused);
	GC::retain(listener);
	s_vListeners.push_back(listener);
	return listener;
}

void e2d::Input::addListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(s_vListeners.begin(), s_vListeners.end(), listener);
		if (iter == s_vListeners.end())
		{
			GC::retain(listener);
			s_vListeners.push_back(listener);
		}
	}
}

void e2d::Input::removeListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(s_vListeners.begin(), s_vListeners.end(), listener);
		if (iter != s_vListeners.end())
		{
			GC::release(listener);
			s_vListeners.erase(iter);
		}
	}
}

void e2d::Input::stopListener(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->stop();
		}
	}
}

void e2d::Input::startListener(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->start();
		}
	}
}

void e2d::Input::removeListener(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->_stopped = true;
		}
	}
}

void e2d::Input::stopAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->stop();
	}
}

void e2d::Input::startAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->start();
	}
}

void e2d::Input::removeAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->_stopped = true;
	}
}

void e2d::Input::clearAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->release();
	}
	s_vListeners.clear();
}

void e2d::Input::__updateListeners()
{
	if (s_vListeners.empty() || Game::getInstance()->isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); ++i)
	{
		auto listener = s_vListeners[i];
		// 清除已停止的监听器
		if (listener->_stopped)
		{
			GC::release(listener);
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// 更新监听器
			listener->_update();
			++i;
		}
	}
}
