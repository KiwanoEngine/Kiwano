#include "..\e2dtool.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Timer::Timer(String name, Function func, double interval /* = 0 */, int updateTimes /* = -1 */, bool atOnce /* = false */, bool autoRelease /* = false */)
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
	this->setName(name);
	this->setFunc(func);
	this->setUpdateTimes(updateTimes);
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
	if (!m_bRunning)
	{
		m_bRunning = true;
		m_fLast = Time::getTotalTime();
		m_nRunTimes = 0;
	}
}

void e2d::Timer::start(int times)
{
	if (!m_bRunning)
	{
		m_bRunning = true;
		m_fLast = Time::getTotalTime();
		m_nUpdateTimes = times;
		m_nRunTimes = 0;
	}
}

e2d::String e2d::Timer::getName() const
{
	return m_sName;
}

void e2d::Timer::setName(String name)
{
	m_sName = name;
}

void e2d::Timer::setInterval(double interval)
{
	m_fInterval = max(interval, 0);
}

void e2d::Timer::setFunc(Function func)
{
	m_Callback = func;
}

void e2d::Timer::setUpdateTimes(int updateTimes)
{
	m_nUpdateTimes = updateTimes;
}

void e2d::Timer::setRunAtOnce(bool bAtOnce)
{
	m_bAtOnce = bAtOnce;
}

void e2d::Timer::update()
{
	if (m_Callback)
	{
		m_Callback();
	}

	m_nRunTimes++;
	m_fLast += m_fInterval;

	if (m_nRunTimes == m_nUpdateTimes)
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
	if (m_bRunning && !m_bClear)
	{
		if (m_bAtOnce && m_nRunTimes == 0)
			return true;

		if (m_fInterval == 0)
			return true;

		if ((Time::getTotalTime() - m_fLast) >= m_fInterval)
			return true;
	}
	return false;
}
