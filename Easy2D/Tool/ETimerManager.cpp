#include "..\etools.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"

static e2d::EVector<e2d::ETimer*> s_vTimers;

void e2d::ETimerManager::bindTimer(ETimer * timer, EScene * pParentScene)
{
	ASSERT(
		(!timer->m_pParentScene) && (!timer->m_pParentNode),
		"The timer is already binded, it cannot bind again!"
	);
	WARN_IF(timer == nullptr, "ETimer NULL pointer exception!");
	WARN_IF(pParentScene == nullptr, "Bind ETimer with a NULL EScene pointer!");

	if (timer && pParentScene)
	{
		timer->start();
		timer->retain();
		timer->m_pParentScene = pParentScene;
		s_vTimers.push_back(timer);
	}
}

void e2d::ETimerManager::bindTimer(ETimer * timer, ENode * pParentNode)
{
	ASSERT(
		(!timer->m_pParentScene) && (!timer->m_pParentNode),
		"The timer is already binded, it cannot bind again!"
	);
	WARN_IF(timer == nullptr, "ETimer NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind ETimer with a NULL ENode pointer!");

	if (timer && pParentNode)
	{
		timer->start();
		timer->retain();
		timer->m_pParentNode = pParentNode;
		s_vTimers.push_back(timer);
	}
}

void e2d::ETimerManager::startTimers(const EString & name)
{
	for (auto t : s_vTimers)
	{
		if (t->getName() == name)
		{
			t->start();
		}
	}
}

void e2d::ETimerManager::stopTimers(const EString & name)
{
	for (auto t : s_vTimers)
	{
		if (t->getName() == name)
		{
			t->stop();
		}
	}
}

void e2d::ETimerManager::delTimers(const EString & name)
{
	EVector<ETimer*>::iterator mIter;
	for (mIter = s_vTimers.begin(); mIter != s_vTimers.end();)
	{
		if ((*mIter)->getName() == name)
		{
			(*mIter)->autoRelease();
			(*mIter)->release();
			mIter = s_vTimers.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
}

void e2d::ETimerManager::startAllTimersBindedWith(EScene * pParentScene)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentScene() == pParentScene)
		{
			t->start();
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		ETimerManager::startAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::stopAllTimersBindedWith(EScene * pParentScene)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentScene() == pParentScene)
		{
			t->stop();
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		ETimerManager::stopAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::clearAllTimersBindedWith(EScene * pParentScene)
{
	for (size_t i = 0; i < s_vTimers.size();)
	{
		auto t = s_vTimers[i];
		if (t->getParentScene() == pParentScene)
		{
			t->autoRelease();
			t->release();
			s_vTimers.erase(s_vTimers.begin() + i);
		}
		else
		{
			i++;
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		ETimerManager::clearAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::startAllTimersBindedWith(ENode * pParentNode)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentNode() == pParentNode)
		{
			t->start();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		ETimerManager::startAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::stopAllTimersBindedWith(ENode * pParentNode)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentNode() == pParentNode)
		{
			t->stop();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		ETimerManager::startAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::clearAllTimersBindedWith(ENode * pParentNode)
{
	for (size_t i = 0; i < s_vTimers.size();)
	{
		auto t = s_vTimers[i];
		if (t->getParentNode() == pParentNode)
		{
			t->autoRelease();
			t->release();
			s_vTimers.erase(s_vTimers.begin() + i);
		}
		else
		{
			i++;
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		ETimerManager::clearAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::_notifyAllTimersBindedWith(EScene * pParentScene)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentScene() == pParentScene)
			t->_notify();
	}
	for (auto child : pParentScene->getChildren())
	{
		_notifyAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::_waitAllTimersBindedWith(EScene * pParentScene)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentScene() == pParentScene)
			t->_wait();
	}
	for (auto child : pParentScene->getChildren())
	{
		_waitAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::_notifyAllTimersBindedWith(ENode * pParentNode)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentNode() == pParentNode)
			t->_notify();
	}
	for (auto child : pParentNode->getChildren())
	{
		_notifyAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::_waitAllTimersBindedWith(ENode * pParentNode)
{
	for (auto t : s_vTimers)
	{
		if (t->getParentNode() == pParentNode)
			t->_notify();
	}
	for (auto child : pParentNode->getChildren())
	{
		_waitAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::startAllTimers()
{
	ETimerManager::startAllTimersBindedWith(EApp::getCurrentScene());
}

void e2d::ETimerManager::stopAllTimers()
{
	ETimerManager::stopAllTimersBindedWith(EApp::getCurrentScene());
}

void e2d::ETimerManager::clearAllTimers()
{
	ETimerManager::clearAllTimersBindedWith(EApp::getCurrentScene());
}

void e2d::ETimerManager::TimerProc()
{
	for (auto t : s_vTimers)
	{
		if (GetInterval(t->m_tLast) >= t->m_nInterval)
		{
			t->_runCallback();
			t->m_tLast = GetNow();
		}
	}
}