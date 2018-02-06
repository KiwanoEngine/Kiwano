#include "..\emanagers.h"
#include "..\eactions.h"

static std::vector<e2d::EAction*> s_vActions;


void e2d::EActionManager::addAction(EAction * pAction, ENode * pTargetNode)
{
	WARN_IF(pAction == nullptr, "EAction NULL pointer exception!");

	if (pAction)
	{
		pAction->startWith(pTargetNode);
		pAction->retain();
		s_vActions.push_back(pAction);
	}
}

void e2d::EActionManager::resumeAllActionsBindedWith(ENode * pTargetNode)
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
			EActionManager::resumeAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::pauseAllActionsBindedWith(ENode * pTargetNode)
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
			EActionManager::pauseAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::stopAllActionsBindedWith(ENode * pTargetNode)
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
			EActionManager::stopAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::__clearAllActionsBindedWith(ENode * pTargetNode)
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

void e2d::EActionManager::__destroyAction(EAction * pAction)
{
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		if (pAction == s_vActions[i])
		{
			s_vActions.erase(s_vActions.begin() + i);
			return;
		}
	}
}

void e2d::EActionManager::resumeAllActions()
{
	for (auto child : ESceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		EActionManager::resumeAllActionsBindedWith(child);
	}
}

void e2d::EActionManager::pauseAllActions()
{
	for (auto child : ESceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		EActionManager::pauseAllActionsBindedWith(child);
	}
}

void e2d::EActionManager::stopAllActions()
{
	for (auto child : ESceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		EActionManager::stopAllActionsBindedWith(child);
	}
}

void e2d::EActionManager::__resetAllActions()
{
	for (auto action : s_vActions)
	{
		action->_resetTime();
	}
}

void e2d::EActionManager::__update()
{
	if (s_vActions.empty())
		return;
	
	// 循环遍历所有正在运行的动作
	for (auto &action : s_vActions)
	{
		// 获取动作运行状态
		if (action->isRunning() &&
			action->getTarget() && 
			action->getTarget()->getParentScene() == ESceneManager::getCurrentScene())
		{
			if (action->_isEnding())
			{
				// 动作已经结束
				action->release();
			}
			else
			{
				// 执行动作
				action->_update();
			}
		}
	}
}
