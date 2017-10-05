#include "..\easy2d.h"
#include <assert.h>

static std::vector<Action*> s_vActions;

void ActionManager::__exec()
{
	// 获取当前时间
	static LARGE_INTEGER nNow;
	QueryPerformanceCounter(&nNow);
	// 循环遍历所有正在运行的动作
	for (size_t i = 0; i < s_vActions.size(); i++)
	{
		auto a = s_vActions[i];
		if (a->isRunning() && a->_exec(nNow))
		{
			// _exec 返回 true 时说明动作已经结束
			a->release();
			s_vActions.erase(s_vActions.begin() + i);
		}
	}
}

void ActionManager::addAction(Action * action)
{
	if (action)
	{
#ifdef _DEBUG
		for (auto a : s_vActions)
		{
			assert(a != action);
		}
#endif
		action->m_pParentScene = App::getCurrentScene();
		action->_init();
		s_vActions.push_back(action);
	}
}

void ActionManager::bindActionsWithScene(Scene * scene)
{
	for (auto action : s_vActions)
	{
		if (!action->m_pParentScene)
		{
			action->m_pParentScene = App::getCurrentScene();
		}
	}
}

void ActionManager::stopAllSceneActions(Scene * scene)
{
	for (auto action : s_vActions)
	{
		if (action->m_pParentScene == scene)
		{
			action->stop();
		}
	}
}

void ActionManager::startAction(Action * action)
{
	resumeAction(action);
}

void ActionManager::resumeAction(Action * action)
{
	for (auto act : s_vActions)
	{
		if (act == action)
		{
			act->resume();
		}
	}
}

void ActionManager::pauseAction(Action * action)
{
	for (auto act : s_vActions)
	{
		if (act == action)
		{
			act->pause();
		}
	}
}

void ActionManager::stopAction(Action * action)
{
	for (auto act : s_vActions)
	{
		if (act == action)
		{
			act->stop();
		}
	}
}

void ActionManager::startSpriteAllActions(Sprite * sprite)
{
	resumeSpriteAllActions(sprite);
}

void ActionManager::resumeSpriteAllActions(Sprite * sprite)
{
	for (auto action : s_vActions)
	{
		if (action->m_pTargetSprite == sprite)
		{
			action->resume();
		}
	}
}

void ActionManager::pauseSpriteAllActions(Sprite * sprite)
{
	for (auto action : s_vActions)
	{
		if (action->m_pTargetSprite == sprite)
		{
			action->pause();
		}
	}
}

void ActionManager::stopSpriteAllActions(Sprite * sprite)
{
	for (auto action : s_vActions)
	{
		if (action->m_pTargetSprite == sprite)
		{
			action->stop();
		}
	}
}

void ActionManager::startAllActions()
{
	resumeAllActions();
}

void ActionManager::resumeAllActions()
{
	for (auto action : s_vActions)
	{
		action->resume();
	}
}

void ActionManager::pauseAllActions()
{
	for (auto action : s_vActions)
	{
		action->pause();
	}
}

void ActionManager::stopAllActions()
{
	for (auto action : s_vActions)
	{
		action->stop();
	}
}

void ActionManager::clearAllActions()
{
	for (auto action : s_vActions)
	{
		action->autoRelease();
		action->release();
	}
	s_vActions.clear();
}
