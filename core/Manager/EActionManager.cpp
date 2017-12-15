#include "..\emanagers.h"
#include "..\eactions.h"
#include "..\Win\winbase.h"

static std::vector<e2d::EAction*> s_vActions;


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
		for (auto action = s_vActions.begin(); action != s_vActions.end(); action++)
		{
			if ((*action)->getTarget() == pTargetNode)
			{
				(*action)->start();
			}
		}
		for (auto child = pTargetNode->getChildren().begin(); child != pTargetNode->getChildren().end(); child++)
		{
			EActionManager::startAllActionsBindedWith((*child));
		}
	}
}

void e2d::EActionManager::pauseAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action = s_vActions.begin(); action != s_vActions.end(); action++)
		{
			if ((*action)->getTarget() == pTargetNode)
			{
				(*action)->pause();
			}
		}
		for (auto child = pTargetNode->getChildren().begin(); child != pTargetNode->getChildren().end(); child++)
		{
			EActionManager::pauseAllActionsBindedWith((*child));
		}
	}
}

void e2d::EActionManager::stopAllActionsBindedWith(ENode * pTargetNode)
{
	if (pTargetNode)
	{
		for (auto action = s_vActions.begin(); action != s_vActions.end(); action++)
		{
			if ((*action)->getTarget() == pTargetNode)
			{
				(*action)->stop();
			}
		}
		for (auto child = pTargetNode->getChildren().begin(); child != pTargetNode->getChildren().end(); child++)
		{
			EActionManager::stopAllActionsBindedWith((*child));
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

void e2d::EActionManager::startAllActions()
{
	for (auto child = EApp::getCurrentScene()->getChildren().begin(); child != EApp::getCurrentScene()->getChildren().end(); child++)
	{
		EActionManager::startAllActionsBindedWith((*child));
	}
}

void e2d::EActionManager::pauseAllActions()
{
	for (auto child = EApp::getCurrentScene()->getChildren().begin(); child != EApp::getCurrentScene()->getChildren().end(); child++)
	{
		EActionManager::pauseAllActionsBindedWith((*child));
	}
}

void e2d::EActionManager::stopAllActions()
{
	for (auto child = EApp::getCurrentScene()->getChildren().begin(); child != EApp::getCurrentScene()->getChildren().end(); child++)
	{
		EActionManager::stopAllActionsBindedWith((*child));
	}
}

void e2d::EActionManager::_clearManager()
{
	s_vActions.clear();
}

void e2d::EActionManager::_resetAllActions()
{
	for (auto action = s_vActions.begin(); action != s_vActions.end(); action++)
	{
		(*action)->_resetTime();
	}
}

void e2d::EActionManager::ActionProc()
{
	if (s_vActions.empty())
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
				SafeRelease(&action);
				s_vActions.erase(s_vActions.begin() + i);
			}
			else
			{
				// 执行动作
				action->_update();
			}
		}
	}
}
