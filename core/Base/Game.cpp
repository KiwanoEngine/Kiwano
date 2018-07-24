#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <thread>

using namespace std::chrono;


e2d::Game * e2d::Game::_instance = nullptr;

e2d::Game::Game()
	: _quit(true)
	, _paused(false)
	, _config()
	, _frameInterval(_config.getFrameInterval())
{
	CoInitialize(nullptr);

	_start = _last = _now = Time::now();
}

e2d::Game::~Game()
{
	CoUninitialize();
}

e2d::Game * e2d::Game::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) Game;
	return _instance;
}

void e2d::Game::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

void e2d::Game::start()
{
	HWND hWnd = Window::getInstance()->getHWnd();
	if (hWnd == nullptr)
	{
		throw SystemException(L"无法创建窗口");
	}

	// 显示窗口
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);

	SceneManager::getInstance()->update();
	Window::getInstance()->poll();

	// 开始游戏
	int wait = 0;
	Duration interval;

	_quit = false;
	_last = _now = Time::now();

	while (!_quit)
	{
		_now = Time::now();
		interval = _now - _last;

		if (_config.isVSyncEnabled() || _frameInterval < interval)
		{
			_last += _frameInterval;
			__update();
		}
		else
		{
			wait = (_frameInterval - interval).milliseconds() - 1;
			if (wait > 1)
			{
				std::this_thread::sleep_for(milliseconds(wait));
			}
		}
	}
}

void e2d::Game::__update()
{
	Input::getInstance()->update();
	Timer::getInstance()->update();
	ActionManager::getInstance()->update();
	SceneManager::getInstance()->update();
	Renderer::getInstance()->render();
	Window::getInstance()->poll();
	GC::getInstance()->flush();
}

void e2d::Game::pause()
{
	_paused = true;
}

void e2d::Game::resume()
{
	if (_paused && !_quit)
	{
		_last = _now = Time::now();
		Timer::getInstance()->updateTime();
		ActionManager::getInstance()->updateTime();
	}
	_paused = false;
}

bool e2d::Game::isPaused()
{
	return _paused;
}

void e2d::Game::setConfig(const Config& config)
{
	if (_config.isSoundEnabled() != config.isSoundEnabled())
	{
		if (config.isSoundEnabled())
			Player::getInstance()->getXAudio2()->StartEngine();
		else
			Player::getInstance()->getXAudio2()->StopEngine();
	}

	if (_config.getFrameInterval() != config.getFrameInterval())
	{
		_frameInterval = Duration(config.getFrameInterval());
	}

	if (_config.isVSyncEnabled() != config.isVSyncEnabled())
	{
		Renderer::getInstance()->discardDeviceResources();
		_last = _now;
	}

	_config = config;
}

const e2d::Config& e2d::Game::getConfig()
{
	return _config;
}

e2d::Duration e2d::Game::getTotalDuration() const
{
	return std::move(_now - _start);
}

void e2d::Game::quit()
{
	_quit = true;
}

void e2d::Game::cleanup()
{
	GC::getInstance()->clear();
	Image::clearCache();
	Player::getInstance()->clearCache();
}
