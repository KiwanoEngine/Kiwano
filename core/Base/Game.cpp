#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dtransition.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <thread>


e2d::Game::Game()
	: _quit(true)
	, _paused(false)
	, _config()
	, _window(nullptr)
	, _input(nullptr)
	, _renderer(nullptr)
	, _currScene(nullptr)
	, _nextScene(nullptr)
	, _transition(nullptr)
	, _scenes()
{
	CoInitialize(nullptr);

	_input = new (std::nothrow) Input;
	_renderer = new (std::nothrow) Renderer;
}

e2d::Game::~Game()
{
	if (_renderer)
		delete _renderer;

	if (_input)
		delete _input;

	if (_window)
		delete _window;

	CoUninitialize();
}

e2d::Game * e2d::Game::getInstance()
{
	static Game instance;
	return &instance;
}

void e2d::Game::start()
{
	_quit = false;

	const int minInterval = 5;
	Time last = Time::now();
	HWND hWnd = _window->getHWnd();
	
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);
	_window->poll();
	updateScene();
	
	while (!_quit)
	{
		auto now = Time::now();
		auto dur = now - last;

		if (dur.milliseconds() > minInterval)
		{
			last = now;
			_input->update();
			Timer::getInstance()->update();
			ActionManager::getInstance()->update();
			updateScene();
			drawScene();
			_window->poll();
			GC::getInstance()->flush();
		}
		else
		{
			// ID2D1HwndRenderTarget 开启了垂直同步，在渲染时会等待显示器刷新，
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

const e2d::Config& e2d::Game::getConfig() const
{
	return _config;
}

void e2d::Game::setWindow(Window * window)
{
	_window = window;
	_renderer->init(_window);
	_input->init(_window);
}

void e2d::Game::quit()
{
	_quit = true;
}

void e2d::Game::pushScene(Scene * scene, bool saveCurrentScene)
{
	if (!scene)
		return;

	// 保存下一场景的指针
	if (_nextScene) _nextScene->release();
	_nextScene = scene;
	_nextScene->retain();

	if (saveCurrentScene && _currScene)
	{
		_scenes.push(_currScene);
	}
}

void e2d::Game::pushScene(Transition * transition, bool saveCurrentScene)
{
	if (!transition)
		return;

	pushScene(transition->_inScene, saveCurrentScene);

	if (_transition)
	{
		_transition->_stop();
		_transition->release();
	}
	_transition = transition;
	_transition->retain();

	// 初始化场景切换动画
	if (!_transition->_init(_currScene))
	{
		WARN("Transition initialize failed!");
		_transition->release();
		_transition = nullptr;
	}
}

e2d::Scene* e2d::Game::popScene()
{
	// 栈为空时，调用返回场景函数失败
	if (_scenes.size() == 0)
	{
		WARN("Scene stack is empty!");
		return nullptr;
	}

	_nextScene = _scenes.top();
	_nextScene->release();
	_scenes.pop();

	return _nextScene;
}

e2d::Scene * e2d::Game::popScene(Transition * transition)
{
	if (!transition)
		return nullptr;

	auto scene = popScene();
	if (scene)
	{
		if (_transition)
		{
			_transition->_stop();
			_transition->release();
		}
		_transition = transition;
		_transition->retain();

		_transition->_inScene = scene;
		_transition->_inScene->retain();

		// 初始化场景切换动画
		if (!_transition->_init(_currScene))
		{
			WARN("Transition initialize failed!");
			_transition->release();
			_transition = nullptr;
		}
	}

	return scene;
}

void e2d::Game::clearAllScenes()
{
	while (!_scenes.empty())
	{
		_scenes.top()->release();
		_scenes.pop();
	}
}

e2d::Scene * e2d::Game::getCurrentScene()
{
	return _currScene;
}

const std::stack<e2d::Scene*>& e2d::Game::getSceneStack()
{
	return _scenes;
}

bool e2d::Game::isTransitioning() const
{
	return _transition != nullptr;
}

void e2d::Game::updateScene()
{
	if (_transition)
	{
		_transition->_update();

		if (_transition->isDone())
		{
			_transition->release();
			_transition = nullptr;
		}
		else
		{
			return;
		}
	}

	if (_nextScene)
	{
		if (_currScene)
		{
			_currScene->onExit();
			if (_scenes.empty() || _scenes.top() != _currScene)
			{
				_currScene->release();
			}
		}

		_nextScene->onEnter();

		_currScene = _nextScene;
		_nextScene = nullptr;
	}
}

void e2d::Game::drawScene()
{
	// 渲染画面
	_renderer->beginDraw();
	{
		if (_transition)
		{
			_transition->_render();
		}
		else if (_currScene)
		{
			_currScene->visit(_renderer);

			if (_config.isOutlineVisible())
			{
				auto brush = _renderer->getSolidColorBrush();
				brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 0.6f));
				brush->SetOpacity(1.f);
				_currScene->drawOutline(_renderer);
			}
			if (_config.isColliderVisible())
			{
				_renderer->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
				_currScene->drawCollider();
			}
		}

		if (_config.isFpsShow())
		{
			_renderer->drawFps();
		}
	}
	_renderer->endDraw();
}
