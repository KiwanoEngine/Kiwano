#include "..\emanagers.h"
#include "..\eactions.h"

static std::vector<e2d::Action*> s_vActions;
static std::vector<e2d::Action*> s_vRunningActions;


void e2d::ActionManager::__update()
{
	if (s_vRunningActions.empty() || Game::isPaused())
		return;

	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vRunningActions.size(); i++)
	{
		auto action = s_vRunningActions[i];
		// 获取动作运行状态
		if (action->isRunning())
		{
			if (!action->_isEnding())
			{
				// 执行动作
				action->_update();
			}
			else
			{
				// 动作已经结束
				action->release();
				action->m_pTarget = nullptr;
				s_vRunningActions.erase(s_vRunningActions.begin() + i);
			}
		}
	}
}

void e2d::ActionManager::__add(Action * pAction)
{
	if (pAction)
	{
		for (const auto action : s_vActions)
		{
			if (action == pAction)
			{
				WARN_IF(true, "ActionManager::add Failed!The action is already added.");
				return;
			}
		}
		s_vActions.push_back(pAction);
	}
}

void e2d::ActionManager::__remove(Action * pAction)
{
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		if (s_vActions[i] == pAction)
		{
			s_vActions.erase(s_vActions.begin() + i);
		}
	}
}

void e2d::ActionManager::__startAction(Action * pAction, Node * pTargetNode)
{
	WARN_IF(pAction == nullptr, "Action NULL pointer exception!");

	if (pAction)
	{
		pAction->setTarget(pTargetNode);
		pAction->retain();
		s_vRunningActions.push_back(pAction);
	}
}

void e2d::ActionManager::resumeAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action : s_vRunningActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->resume();
			}
		}
	}
}

void e2d::ActionManager::pauseAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action : s_vRunningActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->pause();
			}
		}
	}
}

void e2d::ActionManager::stopAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action : s_vRunningActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->stop();
			}
		}
	}
}

void e2d::ActionManager::resumeAllActions(const String & strActionName)
{
	for (auto action : s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::pauseAllActions(const String & strActionName)
{
	for (auto action : s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::stopAllActions(const String & strActionName)
{
	for (auto action : s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->stop();
		}
	}
}

void e2d::ActionManager::__clearAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (size_t i = 0; i < s_vRunningActions.size();)
		{
			auto a = s_vRunningActions[i];
			if (a->getTarget() == pTargetNode)
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

void e2d::ActionManager::resumeAllActions()
{
	for (auto child : SceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		ActionManager::resumeAllActionsBindedWith(child);
	}
}

void e2d::ActionManager::pauseAllActions()
{
	for (auto child : SceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		ActionManager::pauseAllActionsBindedWith(child);
	}
}

void e2d::ActionManager::stopAllActions()
{
	for (auto child : SceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		ActionManager::stopAllActionsBindedWith(child);
	}
}

std::vector<e2d::Action*> e2d::ActionManager::getActions(const String & strActionName)
{
	std::vector<Action*> vActions;
	for (const auto action : s_vActions)
	{
		if (action->getName() == strActionName)
		{
			vActions.push_back(action);
		}
	}
	return std::move(vActions);
}

std::vector<e2d::Action*> e2d::ActionManager::getAllActions()
{
	return s_vActions;
}

void e2d::ActionManager::__resetAllActions()
{
	for (auto action : s_vRunningActions)
	{
		action->_resetTime();
	}
}
