#include "..\e2dmanager.h"
#include "..\e2dbase.h"
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
	
	// 初始化场景切换动画
	if (_transition && !_transition->init(_currScene, _nextScene))
	{
		WARN("Transition initialize failed!");
		_transition->release();
		_transition = nullptr;
	}

	if (saveCurrentScene && _currScene)
	{
		_scenes.push(_currScene);
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
	_scenes.pop();

	// 初始化场景切换动画
	if (_transition && !_transition->init(_currScene, _nextScene))
	{
		WARN("Transition initialize failed!");
		_transition->release();
		_transition = nullptr;
	}

	return _nextScene;
}

void e2d::SceneManager::setTransition(Transition * transition)
{
	if (transition)
	{
		if (_transition)
		{
			_transition->stop();
			_transition->release();
		}
		_transition = transition;
		_transition->retain();
	}
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
		_transition->update();
		
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
		_transition->render();
	}
	else if (_currScene)
	{
		_currScene->render();
	}
}

void e2d::SceneManager::dispatch(const MouseEvent & e)
{
	if (_transition != nullptr)
		return;

	if (_currScene)
	{
		_currScene->dispatch(e);
	}
}

void e2d::SceneManager::dispatch(const KeyEvent & e)
{
	if (_transition != nullptr)
		return;

	if (_currScene)
	{
		_currScene->dispatch(e);
	}
}
