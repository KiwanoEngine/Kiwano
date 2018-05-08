#include "..\e2dmanager.h"
#include "..\e2daction.h"

static std::vector<e2d::ActionBase*> s_vActions;
static std::vector<e2d::ActionBase*> s_vRunningActions;


void e2d::ActionManager::__update()
{
	if (s_vRunningActions.empty() || Game::isPaused())
		return;

	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vRunningActions.size(); i++)
	{
		auto action = s_vRunningActions[i];
		// 获取动作运行状态
		if (action->_isDone())
		{
			// 动作已经结束
			action->release();
			action->_pTarget = nullptr;
			s_vRunningActions.erase(s_vRunningActions.begin() + i);
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

void e2d::ActionManager::__add(ActionBase * action)
{
	if (action)
	{
		s_vActions.push_back(action);
	}
}

void e2d::ActionManager::__remove(ActionBase * action)
{
	for (size_t i = 0; i < s_vActions.size();)
	{
		if (s_vActions[i] == action)
		{
			s_vActions.erase(s_vActions.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void e2d::ActionManager::__resumeAllBindedWith(Node * target)
{
	if (target)
	{
		for (auto action : s_vRunningActions)
		{
			if (action->getTarget() == target)
			{
				action->resume();
			}
		}
	}
}

void e2d::ActionManager::__pauseAllBindedWith(Node * target)
{
	if (target)
	{
		for (auto action : s_vRunningActions)
		{
			if (action->getTarget() == target)
			{
				action->pause();
			}
		}
	}
}

void e2d::ActionManager::__stopAllBindedWith(Node * target)
{
	if (target)
	{
		for (auto action : s_vRunningActions)
		{
			if (action->getTarget() == target)
			{
				action->stop();
			}
		}
	}
}

void e2d::ActionManager::start(ActionBase * action, Node * target, bool paused)
{
	WARN_IF(action == nullptr, "ActionBase NULL pointer exception!");
	WARN_IF(target == nullptr, "Target node NULL pointer exception!");

	if (action && target)
	{
		action->_startWithTarget(target);
		action->retain();
		action->_bRunning = !paused;
		s_vRunningActions.push_back(action);
	}
}

void e2d::ActionManager::resume(const String& strActionName)
{
	for (auto action : s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::pause(const String& strActionName)
{
	for (auto action : s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::stop(const String& strActionName)
{
	for (auto action : s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->stop();
		}
	}
}

void e2d::ActionManager::__clearAllBindedWith(Node * target)
{
	if (target)
	{
		for (size_t i = 0; i < s_vRunningActions.size();)
		{
			auto a = s_vRunningActions[i];
			if (a->getTarget() == target)
			{
				SafeRelease(&a);
				s_vRunningActions.erase(s_vRunningActions.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}
}

void e2d::ActionManager::__uninit()
{
	for (auto action : s_vRunningActions)
	{
		SafeRelease(&action);
	}
	s_vActions.clear();
	s_vRunningActions.clear();
}

void e2d::ActionManager::resumeAll()
{
	for (auto child : SceneManager::getCurrentScene()->getRoot()->getAllChildren())
	{
		ActionManager::__resumeAllBindedWith(child);
	}
}

void e2d::ActionManager::pauseAll()
{
	for (auto child : SceneManager::getCurrentScene()->getRoot()->getAllChildren())
	{
		ActionManager::__pauseAllBindedWith(child);
	}
}

void e2d::ActionManager::stopAll()
{
	for (auto child : SceneManager::getCurrentScene()->getRoot()->getAllChildren())
	{
		ActionManager::__stopAllBindedWith(child);
	}
}

std::vector<e2d::ActionBase*> e2d::ActionManager::get(const String& strActionName)
{
	std::vector<ActionBase*> vActions;
	for (auto action : s_vActions)
	{
		if (action->getName() == strActionName)
		{
			vActions.push_back(action);
		}
	}
	return std::move(vActions);
}

std::vector<e2d::ActionBase*> e2d::ActionManager::getAll()
{
	return s_vActions;
}

void e2d::ActionManager::__resetAll()
{
	for (auto action : s_vRunningActions)
	{
		action->_resetTime();
	}
}
