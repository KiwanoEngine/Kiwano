#include "..\e2dmanager.h"
#include "..\e2devent.h"
#include "..\e2dnode.h"
#include "..\e2dtransition.h"


e2d::SceneManager * e2d::SceneManager::_instance = nullptr;

e2d::SceneManager * e2d::SceneManager::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) SceneManager;
	return _instance;
}

void e2d::SceneManager::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

e2d::SceneManager::SceneManager()
	: _currScene(nullptr)
	, _nextScene(nullptr)
	, _transition(nullptr)
	, _scenes()
{
}

e2d::SceneManager::~SceneManager()
{
}

void e2d::SceneManager::push(Scene * scene, bool saveCurrentScene)
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

void e2d::SceneManager::push(Transition * transition, bool saveCurrentScene)
{
	if (!transition)
		return;

	SceneManager::push(transition->_inScene, saveCurrentScene);

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

e2d::Scene* e2d::SceneManager::pop()
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

e2d::Scene * e2d::SceneManager::pop(Transition * transition)
{
	if (!transition)
		return nullptr;

	auto scene = SceneManager::pop();
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

void e2d::SceneManager::clear()
{
	while (!_scenes.empty())
	{
		_scenes.top()->release();
		_scenes.pop();
	}
}

e2d::Scene * e2d::SceneManager::getCurrentScene()
{
	return _currScene;
}

const std::stack<e2d::Scene*>& e2d::SceneManager::getSceneStack()
{
	return _scenes;
}

bool e2d::SceneManager::isTransitioning()
{
	return _transition != nullptr;
}

void e2d::SceneManager::update()
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

void e2d::SceneManager::render()
{
	if (_transition)
	{
		_transition->_render();
	}
	else if (_currScene)
	{
		_currScene->visit();
	}
}

void e2d::SceneManager::dispatch(const MouseEvent & e)
{
	if (_transition != nullptr)
		return;

	if (_currScene)
	{
		_currScene->dispatch(e, false);
	}
}

void e2d::SceneManager::dispatch(const KeyEvent & e)
{
	if (_transition != nullptr)
		return;

	if (_currScene)
	{
		_currScene->dispatch(e, false);
	}
}
