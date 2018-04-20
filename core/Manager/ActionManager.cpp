#include "..\emanager.h"
#include "..\eaction.h"

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
		if (action->_isEnding())
		{
			// 动作已经结束
			action->release();
			action->m_pTarget = nullptr;
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

void e2d::ActionManager::__add(Action * pAction)
{
	if (pAction)
	{
		FOR_LOOP(action, s_vActions)
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

void e2d::ActionManager::__resumeAllBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		FOR_LOOP(action, s_vRunningActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->resume();
			}
		}
	}
}

void e2d::ActionManager::__pauseAllBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		FOR_LOOP(action, s_vRunningActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->pause();
			}
		}
	}
}

void e2d::ActionManager::__stopAllBindedWith(Node * pTargetNode)
{
	if (pTargetNode)
	{
		FOR_LOOP(action, s_vRunningActions)
		{
			if (action->getTarget() == pTargetNode)
			{
				action->stop();
			}
		}
	}
}

void e2d::ActionManager::resume(String strActionName)
{
	FOR_LOOP(action, s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->resume();
		}
	}
}

void e2d::ActionManager::pause(String strActionName)
{
	FOR_LOOP(action, s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->pause();
		}
	}
}

void e2d::ActionManager::stop(String strActionName)
{
	FOR_LOOP(action, s_vRunningActions)
	{
		if (action->getName() == strActionName)
		{
			action->stop();
		}
	}
}

void e2d::ActionManager::__clearAllBindedWith(Node * pTargetNode)
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

void e2d::ActionManager::__uninit()
{
	FOR_LOOP(child, s_vActions)
	{
		SafeRelease(&child);
	}
	s_vActions.clear();
	s_vRunningActions.clear();
}

void e2d::ActionManager::resumeAll()
{
	FOR_LOOP(child, SceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		ActionManager::__resumeAllBindedWith(child);
	}
}

void e2d::ActionManager::pauseAll()
{
	FOR_LOOP(child, SceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		ActionManager::__pauseAllBindedWith(child);
	}
}

void e2d::ActionManager::stopAll()
{
	FOR_LOOP(child, SceneManager::getCurrentScene()->getRoot()->getChildren())
	{
		ActionManager::__stopAllBindedWith(child);
	}
}

std::vector<e2d::Action*> e2d::ActionManager::get(String strActionName)
{
	std::vector<Action*> vActions;
	FOR_LOOP(action, s_vActions)
	{
		if (action->getName() == strActionName)
		{
			vActions.push_back(action);
		}
	}
	return std::move(vActions);
}

std::vector<e2d::Action*> e2d::ActionManager::getAll()
{
	return s_vActions;
}

void e2d::ActionManager::__resetAllActions()
{
	FOR_LOOP(action, s_vRunningActions)
	{
		action->_resetTime();
	}
}
