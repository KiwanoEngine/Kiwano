#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dtransition.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include <thread>


e2d::Game::Game()
	: _quit(true)
	, _paused(false)
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
	_timer = Timer::instance();
	_actionManager = ActionManager::instance();
}

e2d::Game::~Game()
{
	if (_renderer)
		delete _renderer;

	if (_input)
		delete _input;

	CoUninitialize();
}

e2d::Game * e2d::Game::instance()
{
	static Game instance;
	return &instance;
}

void e2d::Game::initWithWindow(Window * window)
{
	_window = window;
	_renderer->initWithWindow(_window);
	_input->initWithWindow(_window);
}

void e2d::Game::start()
{
	_quit = false;

	const int minInterval = 5;
	Time last = Time::now();
	HWND hWnd = _window->hWnd();
	
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

			if (!_paused)
			{
				_timer->update();
				_actionManager->update();
				updateScene();
			}
			
			drawScene();
			_window->poll();
			GC::instance()->flush();
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
		_timer->updateTime();
		_actionManager->updateTime();
	}
	_paused = false;
}

bool e2d::Game::paused()
{
	return _paused;
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
	if (!_transition->_init(this, _currScene))
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
		if (!_transition->_init(this, _currScene))
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

e2d::Scene * e2d::Game::currentScene()
{
	return _currScene;
}

const std::stack<e2d::Scene*>& e2d::Game::sceneStack()
{
	return _scenes;
}

bool e2d::Game::transitioning() const
{
	return _transition != nullptr;
}

void e2d::Game::updateScene()
{
	if (_transition)
	{
		_transition->_update();

		if (_transition->done())
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
	_renderer->beginDraw();
	{
		if (_transition)
		{
			_transition->_render(this);
		}
		else if (_currScene)
		{
			_currScene->visit(this);
		}
	}
	_renderer->endDraw();
}
