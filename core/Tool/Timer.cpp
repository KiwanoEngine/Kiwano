#include "..\e2dtool.h"
#include "..\e2dnode.h"

namespace e2d
{
	class TimerEntity
	{
	public:
		explicit TimerEntity(
			const e2d::Function& func,
			const e2d::String& name,
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
			if (callback)
			{
				callback();
			}

			++runTimes;
			lastTime += delay;

			if (runTimes == totalTimes)
			{
				stopped = true;
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
}

static std::vector<e2d::TimerEntity*> s_vTimers;


void e2d::Timer::add(const Function& func, double delay, int updateTimes, bool paused, const String& name)
{
	auto timer = new (std::nothrow) TimerEntity(func, name, delay, updateTimes, paused);
	s_vTimers.push_back(timer);
}

void e2d::Timer::add(const Function& func, const String& name)
{
	Timer::add(func, 0, -1, false, name);
}

void e2d::Timer::start(double timeout, const Function& func)
{
	auto timer = new (std::nothrow) TimerEntity(func, L"", timeout, 1, false);
	s_vTimers.push_back(timer);
}

void e2d::Timer::stop(const String& name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->name == name)
		{
			timer->running = false;
		}
	}
}

void e2d::Timer::start(const String& name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->name == name)
		{
			timer->running = true;
		}
	}
}

void e2d::Timer::remove(const String& name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->name == name)
		{
			timer->stopped = true;
		}
	}
}

void e2d::Timer::stopAll()
{
	for (auto timer : s_vTimers)
	{
		timer->running = false;
	}
}

void e2d::Timer::startAll()
{
	for (auto timer : s_vTimers)
	{
		timer->running = true;
	}
}

void e2d::Timer::removeAll()
{
	for (auto timer : s_vTimers)
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
	for (auto timer : s_vTimers)
	{
		timer->lastTime = Time::getTotalTime();
	}
}

void e2d::Timer::__uninit()
{
	for (auto timer : s_vTimers)
	{
		delete timer;
	}
	s_vTimers.clear();
}