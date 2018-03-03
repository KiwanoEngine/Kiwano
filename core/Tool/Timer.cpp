#include "..\etools.h"
#include "..\enodes.h"
#include "..\emanagers.h"

e2d::Timer::Timer()
	: m_bRunning(false)
	, m_nRunTimes(0)
	, m_Callback(nullptr)
	, m_fInterval(0)
	, m_fLast(0)
	, m_nUpdateTimes(-1)
	, m_bAtOnce(false)
	, m_bAutoRelease(false)
	, m_bClear(false)
{
	TimerManager::add(this);
}

e2d::Timer::Timer(const String & name, const TimerCallback & callback, double interval /* = 0 */, int repeatTimes /* = -1 */, bool atOnce /* = false */, bool autoRelease /* = false */)
	: Timer()
{
	this->setName(name);
	this->setCallback(callback);
	this->setUpdateTimes(repeatTimes);
	this->setInterval(interval);
	m_bAutoRelease = autoRelease;
	m_bAtOnce = atOnce;
}

bool e2d::Timer::isRunning() const
{
	return m_bRunning;
}

void e2d::Timer::stop()
{
	m_bRunning = false;
}

void e2d::Timer::stopAndClear()
{
	m_bRunning = false;
	m_bClear = true;
}

void e2d::Timer::start()
{
	m_bRunning = true;
	m_fLast = Time::getTotalTime();
}

e2d::String e2d::Timer::getName() const
{
	return m_sName;
}

void e2d::Timer::setName(const String & name)
{
	m_sName = name;
}

void e2d::Timer::setInterval(double interval)
{
	m_fInterval = max(interval, 0);
}

void e2d::Timer::setCallback(const TimerCallback & callback)
{
	m_Callback = callback;
}

void e2d::Timer::setUpdateTimes(int repeatTimes)
{
	m_nUpdateTimes = repeatTimes;
}

void e2d::Timer::setRunAtOnce(bool bAtOnce)
{
	m_bAtOnce = bAtOnce;
}

void e2d::Timer::update()
{
	if (m_Callback && m_nRunTimes < m_nUpdateTimes)
	{
		m_Callback();
	}

	m_nRunTimes++;
	m_fLast += m_fInterval;

	if (m_nRunTimes >= m_nUpdateTimes)
	{
		if (m_bAutoRelease)
		{
			this->stopAndClear();
		}
		else
		{
			this->stop();
		}
	}
}

bool e2d::Timer::isReady() const
{
	if (m_bRunning)
	{
		if (m_bAtOnce && m_nRunTimes == 0)
			return true;

		if (m_fInterval == 0)
			return true;

		if ((Time::getTotalTime() - m_fLast) >= m_fInterval)
		{
			return true;
		}
	}
	return false;
}
