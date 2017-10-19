#include "..\etools.h"
#include "..\eactions.h"
#include "..\Win\winbase.h"

static e2d::EVector<e2d::EAction*> s_vActions;


void e2d::EActionManager::addAction(EAction * action)
{
	WARN_IF(action == nullptr, "EAction NULL pointer exception!");

	if (action)
	{
		action->start();
		action->retain();
		s_vActions.push_back(action);
	}
}

void e2d::EActionManager::startAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto &child : pParentScene->getChildren())
		{
			EActionManager::startAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::stopAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto &child : pParentScene->getChildren())
		{
			EActionManager::stopAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::clearAllActionsBindedWith(EScene * pParentScene)
{
	if (pParentScene)
	{
		for (const auto & child : pParentScene->getChildren())
		{
			ETimerManager::clearAllTimersBindedWith(child);
		}
	}
}

void e2d::EActionManager::startAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->start();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::startAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::stopAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->stop();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::stopAllActionsBindedWith(child);
		}
	}
}

void e2d::EActionManager::clearAllActionsBindedWith(ENode * pTargetNode)
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
		for (auto child : pTargetNode->getChildren())
		{
			ETimerManager::clearAllTimersBindedWith(child);
		}
	}
}

void e2d::EActionManager::startAllActions()
{
	EActionManager::startAllActionsBindedWith(EApp::getCurrentScene());
}

void e2d::EActionManager::stopAllActions()
{
	EActionManager::stopAllActionsBindedWith(EApp::getCurrentScene());
}

void e2d::EActionManager::clearAllActions()
{
	EActionManager::clearAllActionsBindedWith(EApp::getCurrentScene());
}

void e2d::EActionManager::_clearManager()
{
	s_vActions.clear();
}

void e2d::EActionManager::_resetAllActions()
{
	for (const auto & action : s_vActions)
	{
		action->m_tLast = GetNow();
	}
}

void e2d::EActionManager::ActionProc()
{
	if (EApp::isPaused() || s_vActions.empty())
		return;
	
	EAction * action;
	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		action = s_vActions[i];
		// 获取动作运行状态
		if (action->isRunning() ||
			(action->getTarget() && action->getTarget()->getParentScene() == EApp::getCurrentScene()))
		{
			if (action->_isEnding())
			{
				// 动作已经结束
				SafeRelease(&action);
				s_vActions.erase(s_vActions.begin() + i);
			}
			else
			{
				// 初始化动作
				if (!action->m_bInit)
				{
					action->_init();
				}
				// 执行动作
				action->_callOn();
			}
		}
	}
}
