#include "..\e2dtool.h"
#include "..\e2dnode.h"

class TimerInfo
{
public:
	TimerInfo(
		e2d::Function func,
		e2d::String name,
		double delay,
		int updateTimes,
		bool paused
	)
		: running(!paused)
		, stopped(false)
		, runTimes(0)
		, totalTimes(updateTimes)
		, delay(max(delay, 0))
		, lastTime(e2d::Time::getTotalTime())
		, callback(func)
		, name(name)
	{
	}

	void update()
	{
		if (this->callback)
		{
			this->callback();
		}

		this->runTimes++;
		this->lastTime += this->delay;

		if (this->runTimes == this->totalTimes)
		{
			this->stopped = true;
		}
	}

	bool ready()
	{
		if (this->running)
		{
			if (this->delay == 0)
				return true;

			if ((e2d::Time::getTotalTime() - this->lastTime) >= this->delay)
				return true;
		}
		return false;
	}

public:
	bool	running;
	bool	stopped;
	int		runTimes;
	int		totalTimes;
	double	delay;
	double	lastTime;
	e2d::String		name;
	e2d::Function	callback;
};

static std::vector<TimerInfo*> s_vTimers;


void e2d::Timer::start(Function func, double delay, int updateTimes, bool paused, String name)
{
	auto timer = new TimerInfo(func, name, delay, updateTimes, paused);
	s_vTimers.push_back(timer);
}

void e2d::Timer::start(Function func, String name)
{
	Timer::start(func, 0, -1, false, name);
}

void e2d::Timer::startOnce(Function func, double timeOut)
{
	auto timer = new TimerInfo(func, L"", timeOut, 1, false);
	s_vTimers.push_back(timer);
}

void e2d::Timer::pause(String name)
{
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->name == name)
		{
			timer->running = false;
		}
	}
}

void e2d::Timer::resume(String name)
{
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->name == name)
		{
			timer->running = true;
		}
	}
}

void e2d::Timer::stop(String name)
{
	FOR_LOOP(timer, s_vTimers)
	{
		if (timer->name == name)
		{
			timer->stopped = true;
		}
	}
}

void e2d::Timer::pauseAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->running = false;
	}
}

void e2d::Timer::resumeAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->running = true;
	}
}

void e2d::Timer::stopAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->stopped = true;
	}
}

void e2d::Timer::__update()
{
	if (s_vTimers.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vTimers.size();)
	{
		auto timer = s_vTimers[i];
		// 清除已停止的定时器
		if (timer->stopped)
		{
			delete timer;
			s_vTimers.erase(s_vTimers.begin() + i);
		}
		else
		{
			// 更新定时器
			if (timer->ready())
			{
				timer->update();
			}

			++i;
		}
	}
}

void e2d::Timer::__resetAll()
{
	FOR_LOOP(timer, s_vTimers)
	{
		timer->lastTime = Time::getTotalTime();
	}
}

void e2d::Timer::__uninit()
{
	FOR_LOOP(timer, s_vTimers)
	{
		delete timer;
	}
	s_vTimers.clear();
}