#include "..\emanagers.h"
#include "..\etools.h"
#include "..\enodes.h"

static std::vector<e2d::Timer*> s_vTimers;


void e2d::TimerManager::__update()
{
	if (s_vTimers.empty() || Game::isPaused())
		return;

	for (auto &timer : s_vTimers)
	{
		if (timer->_isReady())
		{
			timer->_callOn();
		}
	}
}

void e2d::TimerManager::add(Timer * pTimer, Scene * pParentScene)
{
	TimerManager::add(pTimer, pParentScene->getRoot());
}

void e2d::TimerManager::add(Timer * pTimer, Node * pParentNode)
{
	WARN_IF(pTimer == nullptr, "Timer NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind Timer with a NULL Node pointer!");

	if (pTimer && pParentNode)
	{
		ASSERT(
			!pTimer->m_pParentNode,
			"The timer is already binded, cannot be binded again!"
		);

		pTimer->start();
		pTimer->retain();
		pTimer->m_pParentNode = pParentNode;
		s_vTimers.push_back(pTimer);
	}
}

void e2d::TimerManager::startTimers(const String & name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->start();
		}
	}
}

void e2d::TimerManager::stopTimers(const String & name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->stop();
		}
	}
}

void e2d::TimerManager::deleteTimers(const String & name)
{
	std::vector<Timer*>::iterator mIter;
	for (mIter = s_vTimers.begin(); mIter != s_vTimers.end();)
	{
		if ((*mIter)->getName() == name)
		{
			SafeRelease(&(*mIter));
			mIter = s_vTimers.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
}

void e2d::TimerManager::startAllTimersBindedWith(Scene * pParentScene)
{
	TimerManager::startAllTimersBindedWith(pParentScene->getRoot());
}

void e2d::TimerManager::stopAllTimersBindedWith(Scene * pParentScene)
{
	TimerManager::stopAllTimersBindedWith(pParentScene->getRoot());
}

void e2d::TimerManager::startAllTimersBindedWith(Node * pParentNode)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getParentNode() == pParentNode)
		{
			timer->start();
		}
	}
	for (auto child = pParentNode->getChildren().begin(); child != pParentNode->getChildren().end(); child++)
	{
		TimerManager::startAllTimersBindedWith((*child));
	}
}

void e2d::TimerManager::stopAllTimersBindedWith(Node * pParentNode)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getParentNode() == pParentNode)
		{
			timer->stop();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		TimerManager::stopAllTimersBindedWith(child);
	}
}

void e2d::TimerManager::__clearAllTimersBindedWith(Node * pParentNode)
{
	for (size_t i = 0; i < s_vTimers.size();)
	{
		auto t = s_vTimers[i];
		if (t->getParentNode() == pParentNode)
		{
			SafeRelease(&t);
			s_vTimers.erase(s_vTimers.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void e2d::TimerManager::__resetAllTimers()
{
	for (auto timer : s_vTimers)
	{
		timer->m_fLast = Time::getTotalTime();
	}
}

void e2d::TimerManager::startAllTimers()
{
	TimerManager::startAllTimersBindedWith(SceneManager::getCurrentScene());
}

void e2d::TimerManager::stopAllTimers()
{
	TimerManager::stopAllTimersBindedWith(SceneManager::getCurrentScene());
}