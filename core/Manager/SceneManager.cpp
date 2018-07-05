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
	: _saveCurrScene(true)
	, _currScene(nullptr)
	, _nextScene(nullptr)
	, _transition(nullptr)
	, _scenes()
{
}

e2d::SceneManager::~SceneManager()
{
}

void e2d::SceneManager::enter(Scene * scene, Transition * transition /* = nullptr */, bool saveCurrentScene /* = true */)
{
	if (!scene)
		return;

	// 保存下一场景的指针
	_nextScene = scene;
	_nextScene->retain();
	
	// 设置切换场景动作
	if (transition)
	{
		if (_transition)
		{
			_transition->_stop();
			_transition->release();
		}
		_transition = transition;
		transition->retain();
		transition->_init(_currScene, _nextScene);
		transition->_update();
	}

	if (_currScene)
	{
		_saveCurrScene = saveCurrentScene;
	}
}

void e2d::SceneManager::back(Transition * transition /* = nullptr */)
{
	// 栈为空时，调用返回场景函数失败
	WARN_IF(_scenes.size() == 0, "Scene stack is empty!");
	if (_scenes.size() == 0) return;

	// 从栈顶取出场景指针，作为下一场景
	_nextScene = _scenes.top();
	_scenes.pop();

	// 返回上一场景时，不保存当前场景
	if (_currScene)
	{
		_saveCurrScene = false;
	}

	// 设置切换场景动作
	if (transition)
	{
		_transition = transition;
		transition->retain();
		transition->_init(_currScene, _nextScene);
		transition->_update();
	}
}

void e2d::SceneManager::clear()
{
	// 清空场景栈
	while (_scenes.size())
	{
		auto temp = _scenes.top();
		GC::release(temp);
		_scenes.pop();
	}
}

e2d::Scene * e2d::SceneManager::getCurrentScene()
{
	return _currScene;
}

std::stack<e2d::Scene*> e2d::SceneManager::getSceneStack()
{
	return _scenes;
}

bool e2d::SceneManager::isTransitioning()
{
	return _transition != nullptr;
}

void e2d::SceneManager::__update()
{
	if (_transition == nullptr)
	{
		// 更新场景内容
		if (_currScene)
		{
			_currScene->_update();
		}
	}
	else
	{
		// 更新场景动作
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

	// 下一场景指针不为空时，切换场景
	if (_nextScene)
	{
		if (_currScene)
		{
			// 执行当前场景的 onExit 函数
			_currScene->onExit();

			// 若要保存当前场景，把它放入栈中
			if (_saveCurrScene)
			{
				_scenes.push(_currScene);
			}
			else
			{
				_currScene->release();
			}
		}

		// 执行下一场景的 onEnter 函数
		_nextScene->onEnter();

		_currScene = _nextScene;		// 切换场景
		_nextScene = nullptr;				// 下一场景置空
	}
}

void e2d::SceneManager::__render()
{
	if (_transition)
	{
		_transition->_render();
	}
	else
	{
		// 绘制当前场景
		if (_currScene)
		{
			_currScene->_render();
		}
	}
}
