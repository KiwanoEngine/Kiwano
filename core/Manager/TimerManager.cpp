#include "..\emanager.h"
#include "..\etool.h"
#include "..\enode.h"

static std::vector<e2d::Timer*> s_vTimers;


void e2d::TimerManager::__update()
{
	if (s_vTimers.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vTimers.size(); i++)
	{
		auto pTimer = s_vTimers[i];
		// 更新定时器
		if (pTimer->isReady())
		{
			pTimer->update();
		}
		// 清除不必要的定时器
		if (pTimer->m_bClear)
		{
			pTimer->release();
			s_vTimers.erase(s_vTimers.begin() + i);
			i--;
		}
	}
}

e2d::Timer * e2d::TimerManager::start(String name, Function func, double interval, int times, bool atOnce, bool autoRelease)
{
	auto pTimer = new (std::nothrow) Timer(name, func, interval, times, atOnce, autoRelease);
	if (pTimer)
	{
		pTimer->retain();
		pTimer->start();
		s_vTimers.push_back(pTimer);
	}
	return pTimer;
}

e2d::Timer* e2d::TimerManager::start(double timeOut, Function func)
{
	auto pTimer = new (std::nothrow) Timer(L"", func, timeOut, 1, false, true);
	if (pTimer)
	{
		pTimer->retain();
		pTimer->start();
		s_vTimers.push_back(pTimer);
	}
	return pTimer;
}

void e2d::TimerManager::add(Timer * pTimer)
{
	WARN_IF(pTimer == nullptr, "Timer NULL pointer exception!");

	if (pTimer)
	{
		auto findTimer = [](Timer * pTimer) -> bool
		{
			FOR_LOOP(t, s_vTimers)
			{
				if (pTimer == t)
				{
					return true;
				}
			}
			return false;
		};

		bool bHasTimer = findTimer(pTimer);
		WARN_IF(bHasTimer, "The timer is already added, cannot be added again!");

		if (!bHasTimer)
		{
			pTimer->retain();
			s_vTimers.push_back(pTimer);
		}
	}
}

void e2d::TimerManager::start(String name)
{
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->start();
		}
	}
}

void e2d::TimerManager::stop(String name)
{
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->stop();
		}
	}
}

void e2d::TimerManager::clear(String name)
{
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->stopAndClear();
		}
	}
}

std::vector<e2d::Timer*> e2d::TimerManager::get(String name)
{
	std::vector<Timer*> vTimers;
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->getName() == name)
		{
			vTimers.push_back(timer);
		}
	}
	return std::move(vTimers);
}

void e2d::TimerManager::startAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->start();
	}
}

void e2d::TimerManager::stopAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->stop();
	}
}

void e2d::TimerManager::stopAndClearAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->stop();
		timer->release();
	}
	s_vTimers.clear();
}

std::vector<e2d::Timer*> e2d::TimerManager::getAll()
{
	return s_vTimers;
}

void e2d::TimerManager::__resetAllTimers()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->m_fLast = Time::getTotalTime();
	}
}

void e2d::TimerManager::__uninit()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->release();
	}
	s_vTimers.clear();
}