#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"


e2d::InputManager * e2d::InputManager::_instance = nullptr;

e2d::InputManager * e2d::InputManager::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) InputManager;
	return _instance;
}

void e2d::InputManager::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

e2d::InputManager::InputManager()
{
}

e2d::InputManager::~InputManager()
{
}

e2d::Listener * e2d::InputManager::addListener(const Function& func, const String& name, bool paused)
{
	auto listener = new (e2d::autorelease) Listener(func, name, paused);
	GC::retain(listener);
	_listeners.push_back(listener);
	return listener;
}

void e2d::InputManager::addListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(_listeners.begin(), _listeners.end(), listener);
		if (iter == _listeners.end())
		{
			GC::retain(listener);
			_listeners.push_back(listener);
		}
	}
}

void e2d::InputManager::removeListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(_listeners.begin(), _listeners.end(), listener);
		if (iter != _listeners.end())
		{
			GC::safeRelease(listener);
			_listeners.erase(iter);
		}
	}
}

void e2d::InputManager::stopListener(const String& name)
{
	if (_listeners.empty() || name.isEmpty())
		return;

	for (auto listener : _listeners)
	{
		if (listener->_name == name)
		{
			listener->stop();
		}
	}
}

void e2d::InputManager::startListener(const String& name)
{
	if (_listeners.empty() || name.isEmpty())
		return;

	for (auto listener : _listeners)
	{
		if (listener->_name == name)
		{
			listener->start();
		}
	}
}

void e2d::InputManager::removeListener(const String& name)
{
	if (_listeners.empty() || name.isEmpty())
		return;

	for (auto listener : _listeners)
	{
		if (listener->_name == name)
		{
			listener->_stopped = true;
		}
	}
}

void e2d::InputManager::stopAllListeners()
{
	for (auto listener : _listeners)
	{
		listener->stop();
	}
}

void e2d::InputManager::startAllListeners()
{
	for (auto listener : _listeners)
	{
		listener->start();
	}
}

void e2d::InputManager::removeAllListeners()
{
	for (auto listener : _listeners)
	{
		listener->_stopped = true;
	}
}

void e2d::InputManager::clearAllListeners()
{
	for (auto listener : _listeners)
	{
		GC::release(listener);
	}
	_listeners.clear();
}

void e2d::InputManager::update()
{
	if (_listeners.empty() || Game::getInstance()->isPaused())
		return;

	for (size_t i = 0; i < _listeners.size(); ++i)
	{
		auto listener = _listeners[i];
		// Çå³ýÒÑÍ£Ö¹µÄ¼àÌýÆ÷
		if (listener->_stopped)
		{
			GC::safeRelease(listener);
			_listeners.erase(_listeners.begin() + i);
		}
		else
		{
			// ¸üÐÂ¼àÌýÆ÷
			listener->_update();
			++i;
		}
	}
}
