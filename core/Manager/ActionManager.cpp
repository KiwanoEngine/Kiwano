#include "..\emanagers.h"
#include "..\eactions.h"

static std::vector<e2d::Action*> s_vActions;


void e2d::ActionManager::_add(Action * pAction, Node * pTargetNode)
{
	WARN_IF(pAction == nullptr, "Action NULL pointer exception!");

	if (pAction)
	{
		pAction->startWith(pTargetNode);
		pAction->retain();
		s_vActions.push_back(pAction);
	}
}

void e2d::ActionManager::resumeAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->resume();
			}
		}
		for (auto child : pTargetNode->getChildren())
		{
			ActionManager::resumeAllActionsBindedWith(child);
		}
	}
}

void e2d::ActionManager::pauseAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->pause();
			}
		}
		for (auto child : pTargetNode->getChildren())
		{
			ActionManager::pauseAllActionsBindedWith(child);
		}
	}
}

void e2d::ActionManager::stopAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->stop();
			}
		}
		for (auto child : pTargetNode->getChildren())
		{
			ActionManager::stopAllActionsBindedWith(child);
		}
	}
}

void e2d::ActionManager::__clearAllActionsBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		for (size_t i = 0; i < s_vActions.size();)
		{
			auto a = s_vActions[i];
			if (a->getTarget() == pTargetNode)
			{
				SafeRelease(&a);
				s_vActions.erase(s_vActions.begin() + i);
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

void e2d::ActionManager::__resetAllActions()
{
	for (auto action : s_vActions)
	{
		action->_resetTime();
	}
}

void e2d::ActionManager::__update()
{
	if (s_vActions.empty() || Game::isPaused())
		return;
	
	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		auto &action = s_vActions[i];
		// 获取动作运行状态
		if (action->isRunning() &&
			action->getTarget() && 
			action->getTarget()->getParentScene() == SceneManager::getCurrentScene())
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
				s_vActions.erase(s_vActions.begin() + i);
			}
		}
	}
}
