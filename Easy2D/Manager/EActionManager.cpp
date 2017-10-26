#include "..\emanagers.h"
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

void e2d::EActionManager::pauseAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (const auto &action : s_vActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->pause();
			}
		}
		for (const auto &child : pTargetNode->getChildren())
		{
			EActionManager::pauseAllActionsBindedWith(child);
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

void e2d::EActionManager::_clearAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (size_t i = 0; i < s_vActions.size();)
		{
			auto a = s_vActions[i];
			if (a->getTarget() == pTargetNode)
			{
				SafeReleaseAndClear(&a);
				s_vActions.erase(s_vActions.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}
}

void e2d::EActionManager::startAllActions()
{
	for (auto child : EApp::getCurrentScene()->getChildren())
	{
		EActionManager::startAllActionsBindedWith(child);
	}
}

void e2d::EActionManager::pauseAllActions()
{
	for (auto child : EApp::getCurrentScene()->getChildren())
	{
		EActionManager::pauseAllActionsBindedWith(child);
	}
}

void e2d::EActionManager::stopAllActions()
{
	for (auto child : EApp::getCurrentScene()->getChildren())
	{
		EActionManager::stopAllActionsBindedWith(child);
	}
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
	
	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		auto &action = s_vActions[i];
		// 获取动作运行状态
		if (action->isRunning() ||
			(action->getTarget() && action->getTarget()->getParentScene() == EApp::getCurrentScene()))
		{
			if (action->_isEnding())
			{
				// 动作已经结束
				SafeReleaseAndClear(&action);
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
