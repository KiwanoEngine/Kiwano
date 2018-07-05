#include "..\e2dmanager.h"
#include "..\e2daction.h"
#include "..\e2dnode.h"


e2d::ActionManager * e2d::ActionManager::_instance = nullptr;

e2d::ActionManager * e2d::ActionManager::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) ActionManager;
	return _instance;
}

void e2d::ActionManager::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

e2d::ActionManager::ActionManager()
	: _actions()
	, _runningActions()
{
}

e2d::ActionManager::~ActionManager()
{
}

void e2d::ActionManager::update()
{
	if (_runningActions.empty() || Game::getInstance()->isPaused())
		return;

	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < _runningActions.size(); ++i)
	{
		auto action = _runningActions[i];
		// 获取动作运行状态
		if (action->_isDone() || action->_target->getRefCount() == 0)
		{
			action->release();
			action->_target = nullptr;
			_runningActions.erase(_runningActions.begin() + i);
		}
		else
		{
			if (action->isRunning())
			{
				// 执行动作
				action->_update();
			}
		}
	}
}

void e2d::ActionManager::__add(Action * action)
{
	if (action)
	{
		auto iter = std::find(_actions.begin(), _actions.end(), action);
		if (iter == _actions.end())
		{
			_actions.push_back(action);
		}
	}
}

void e2d::ActionManager::__remove(Action * action)
{
	if (_actions.empty() || action == nullptr)
		return;

	auto iter = std::find(_actions.begin(), _actions.end(), action);
	if (iter != _actions.end())
	{
		_actions.erase(iter);
	}
}

void e2d::ActionManager::resumeAllBindedWith(Node * target)
{
	if (_runningActions.empty() || target == nullptr)
		return;

	for (auto action : _runningActions)
	{
		if (action->getTarget() == target)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::pauseAllBindedWith(Node * target)
{
	if (_runningActions.empty() || target == nullptr)
		return;

	for (auto action : _runningActions)
	{
		if (action->getTarget() == target)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::stopAllBindedWith(Node * target)
{
	if (_runningActions.empty() || target == nullptr)
		return;

	for (auto action : _runningActions)
	{
		if (action->getTarget() == target)
		{
			action->stop();
		}
	}
}

void e2d::ActionManager::start(Action * action, Node * target, bool paused)
{
	WARN_IF(action == nullptr, "Action NULL pointer exception!");
	WARN_IF(target == nullptr, "Target node NULL pointer exception!");

	if (action && target)
	{
		if (action->_target == nullptr)
		{
			auto iter = std::find(_runningActions.begin(), _runningActions.end(), action);
			if (iter == _runningActions.end())
			{
				action->_startWithTarget(target);
				action->retain();
				action->_running = !paused;
				_runningActions.push_back(action);
			}
		}
		else
		{
			throw Exception(L"该 Action 已有执行目标");
		}
	}
}

void e2d::ActionManager::resume(const String& name)
{
	if (_runningActions.empty() || name.isEmpty())
		return;

	for (auto action : _runningActions)
	{
		if (action->getName() == name)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::pause(const String& name)
{
	if (_runningActions.empty() || name.isEmpty())
		return;

	for (auto action : _runningActions)
	{
		if (action->getName() == name)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::stop(const String& name)
{
	if (_runningActions.empty() || name.isEmpty())
		return;

	for (auto action : _runningActions)
	{
		if (action->getName() == name)
		{
			action->stop();
		}
	}
}

void e2d::ActionManager::clearAllBindedWith(Node * target)
{
	if (target)
	{
		for (size_t i = 0; i < _runningActions.size();)
		{
			auto a = _runningActions[i];
			if (a->getTarget() == target)
			{
				GC::safeRelease(a);
				_runningActions.erase(_runningActions.begin() + i);
			}
			else
			{
				++i;
			}
		}
	}
}

void e2d::ActionManager::clearAll()
{
	for (auto action : _runningActions)
	{
		action->release();
	}
	_actions.clear();
	_runningActions.clear();
}

std::vector<e2d::Action*> e2d::ActionManager::get(const String& name)
{
	std::vector<Action*> actions;
	for (auto action : _actions)
	{
		if (action->getName() == name)
		{
			actions.push_back(action);
		}
	}
	return std::move(actions);
}

const std::vector<e2d::Action*>& e2d::ActionManager::getAll()
{
	return _actions;
}

void e2d::ActionManager::updateTime()
{
	for (auto action : _runningActions)
	{
		action->_resetTime();
	}
}
