#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <thread>


e2d::Game * e2d::Game::_instance = nullptr;

e2d::Game::Game()
	: _quit(true)
	, _paused(false)
	, _config()
{
	CoInitialize(nullptr);
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
	_quit = false;

	const int minInterval = 5;
	Time last = Time::now();
	HWND hWnd = Window::getInstance()->getHWnd();
	
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);
	Window::getInstance()->poll();
	SceneManager::getInstance()->update();
	
	while (!_quit)
	{
		auto now = Time::now();
		auto dur = now - last;

		if (dur.milliseconds() > minInterval)
		{
			last = now;
			Input::getInstance()->update();
			Timer::getInstance()->update();
			ActionManager::getInstance()->update();
			SceneManager::getInstance()->update();
			Renderer::getInstance()->render();
			Window::getInstance()->poll();
			GC::getInstance()->flush();
		}
		else
		{
			// ID2D1HwndRenderTarget 开启了垂直同步,在渲染时会等待显示器刷新，
			// 它起到了非常稳定的延时作用，所以大部分时候不需要手动挂起线程进行延时。
			// 下面的代码仅在一些情况下（例如窗口最小化时）挂起线程，防止占用过高 CPU 。
			int wait = minInterval - dur.milliseconds();
			if (wait > 1)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(wait));
			}
		}
	}
}

void e2d::Game::pause()
{
	_paused = true;
}

void e2d::Game::resume()
{
	if (_paused && !_quit)
	{
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
	_config = config;
}

const e2d::Config& e2d::Game::getConfig()
{
	return _config;
}

void e2d::Game::quit()
{
	_quit = true;
}

void e2d::Game::cleanup()
{
	GC::getInstance()->clear();
	Image::clearCache();
}
