#include "..\e2dmanager.h"
#include "..\e2daction.h"
#include "..\e2dnode.h"

static std::vector<e2d::Action*> s_vActions;
static std::vector<e2d::Action*> s_vRunningActions;


void e2d::ActionManager::__update()
{
	if (s_vRunningActions.empty() || Game::isPaused())
		return;

	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vRunningActions.size(); ++i)
	{
		auto action = s_vRunningActions[i];
		// 获取动作运行状态
		if (action->_isDone() || action->_target->getRefCount() == 0)
		{
			action->release();
			action->_target = nullptr;
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

void e2d::ActionManager::__add(Action * action)
{
	if (action)
	{
		s_vActions.push_back(action);
	}
}

void e2d::ActionManager::__remove(Action * action)
{
	if (s_vActions.empty() || action == nullptr)
		return;

	auto iter = std::find(s_vActions.begin(), s_vActions.end(), action);
	if (iter != s_vActions.end())
	{
		s_vActions.erase(iter);
	}
}

void e2d::ActionManager::__resumeAllBindedWith(Node * target)
{
	if (s_vRunningActions.empty() || target == nullptr)
		return;

	for (auto action : s_vRunningActions)
	{
		if (action->getTarget() == target)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::__pauseAllBindedWith(Node * target)
{
	if (s_vRunningActions.empty() || target == nullptr)
		return;

	for (auto action : s_vRunningActions)
	{
		if (action->getTarget() == target)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::__stopAllBindedWith(Node * target)
{
	if (s_vRunningActions.empty() || target == nullptr)
		return;

	for (auto action : s_vRunningActions)
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
			auto iter = std::find(s_vRunningActions.begin(), s_vRunningActions.end(), action);
			if (iter == s_vRunningActions.end())
			{
				action->_startWithTarget(target);
				action->retain();
				action->_running = !paused;
				s_vRunningActions.push_back(action);
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
	if (s_vRunningActions.empty() || name.isEmpty())
		return;

	for (auto action : s_vRunningActions)
	{
		if (action->getName() == name)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::pause(const String& name)
{
	if (s_vRunningActions.empty() || name.isEmpty())
		return;

	for (auto action : s_vRunningActions)
	{
		if (action->getName() == name)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::stop(const String& name)
{
	if (s_vRunningActions.empty() || name.isEmpty())
		return;

	for (auto action : s_vRunningActions)
	{
		if (action->getName() == name)
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
				GC::release(a);
				s_vRunningActions.erase(s_vRunningActions.begin() + i);
			}
			else
			{
				++i;
			}
		}
	}
}

void e2d::ActionManager::__uninit()
{
	for (auto action : s_vRunningActions)
	{
		GC::release(action);
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

std::vector<e2d::Action*> e2d::ActionManager::get(const String& name)
{
	std::vector<Action*> actions;
	for (auto action : s_vActions)
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
	return s_vActions;
}

void e2d::ActionManager::__resetAll()
{
	for (auto action : s_vRunningActions)
	{
		action->_resetTime();
	}
}
