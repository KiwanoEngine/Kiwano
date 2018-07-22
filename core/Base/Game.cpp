#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <thread>

using namespace std::chrono;


e2d::Game * e2d::Game::_instance = nullptr;

e2d::Game::Game()
	: _ended(true)
	, _paused(false)
	, _config(nullptr)
{
	CoInitialize(nullptr);

	_start = _last = _now = Time::now();
}

e2d::Game::~Game()
{
	GC::getInstance()->safeRelease(_config);

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
	auto gc = GC::getInstance();
	auto input = Input::getInstance();
	auto window = Window::getInstance();
	auto renderer = Renderer::getInstance();
	auto timer = Timer::getInstance();
	auto sceneManager = SceneManager::getInstance();
	auto actionManager = ActionManager::getInstance();

	if (!input || !window || !renderer || !timer || !sceneManager || !actionManager)
	{
		throw SystemException(L"初始化失败");
	}

	HWND hWnd = window->getHWnd();
	if (hWnd == nullptr)
	{
		throw SystemException(L"无法创建窗口");
	}

	// 显示窗口
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);
	window->poll();

	// 开始游戏
	Duration frameInterval(15), interval;
	int wait = 0;
	
	_ended = false;
	_last = _now = Time::now();

	while (!_ended)
	{
		_now = Time::now();
		interval = _now - _last;

		if (frameInterval < interval)
		{
			_last += interval;

			input->update();
			timer->update();
			actionManager->update();
			sceneManager->update();
			_config->_update();
			renderer->render();
			window->poll();
			gc->flush();
		}
		else
		{
			wait = (frameInterval - interval).milliseconds() - 1;
			if (wait > 1)
			{
				std::this_thread::sleep_for(milliseconds(wait));
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
	if (_paused && !_ended)
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

void e2d::Game::setConfig(Config* config)
{
	if (config && _config != config)
	{
		if (_config) _config->release();
		_config = config;
		_config->_unconfigured = true;
		_config->retain();
	}
}

e2d::Config* e2d::Game::getConfig()
{
	if (!_config)
		_config = new (e2d::autorelease) Config();
	return _config;
}

e2d::Duration e2d::Game::getTotalDuration() const
{
	return std::move(_now - _start);
}

void e2d::Game::quit()
{
	_ended = true;	// 这个变量将控制游戏是否结束
}

void e2d::Game::cleanup()
{
	GC::getInstance()->clear();
	Image::clearCache();
	Player::getInstance()->clearCache();
}
