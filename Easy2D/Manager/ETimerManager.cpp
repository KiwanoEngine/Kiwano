#include "..\emanagers.h"
#include "..\etools.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"

static e2d::EVector<e2d::ETimer*> s_vTimers;


void e2d::ETimerManager::TimerProc()
{
	if (EApp::isPaused())
		return;

	for (size_t i = 0; i < s_vTimers.size(); i++)
	{
		auto &t = s_vTimers[i];
		if (t->isRunning())
		{
			if (t->getParentNode() && 
				t->getParentNode()->getParentScene() == EApp::getCurrentScene())
			{
				if (t->_isReady())
				{
					t->_callOn();
				}
			}
		}
	}
}

void e2d::ETimerManager::bindTimer(ETimer * timer, EScene * pParentScene)
{
	ETimerManager::bindTimer(timer, pParentScene->getRoot());
}

void e2d::ETimerManager::bindTimer(ETimer * timer, ENode * pParentNode)
{
	WARN_IF(timer == nullptr, "ETimer NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind ETimer with a NULL ENode pointer!");

	if (timer && pParentNode)
	{
		ASSERT(
			!timer->m_pParentNode,
			"The timer is already binded, it cannot bind again!"
		);

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
			SafeRelease(&(*mIter));
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
	ETimerManager::startAllTimersBindedWith(pParentScene->getRoot());
}

void e2d::ETimerManager::stopAllTimersBindedWith(EScene * pParentScene)
{
	ETimerManager::stopAllTimersBindedWith(pParentScene->getRoot());
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
		ETimerManager::stopAllTimersBindedWith(child);
	}
}

void e2d::ETimerManager::_clearAllTimersBindedWith(ENode * pParentNode)
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

void e2d::ETimerManager::_clearManager()
{
	s_vTimers.clear();
}

void e2d::ETimerManager::_resetAllTimers()
{
	for (const auto & t : s_vTimers)
	{
		t->m_tLast = GetNow();
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