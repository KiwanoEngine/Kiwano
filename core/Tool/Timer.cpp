#include "..\etools.h"
#include "..\enodes.h"
#include "..\emanagers.h"

e2d::Timer::Timer()
	: m_bRunning(false)
	, m_nRunTimes(0)
	, m_pParentNode(nullptr)
	, m_Callback(nullptr)
	, m_fInterval(0)
	, m_fLast(0)
	, m_nRepeatTimes(-1)
	, m_bAtOnce(false)
{
}

e2d::Timer::Timer(const TimerCallback & callback, double interval /* = 0 */, int repeatTimes /* = -1 */, bool atOnce /* = false */)
	: m_bRunning(false)
	, m_nRunTimes(0)
	, m_pParentNode(nullptr)
	, m_Callback(nullptr)
	, m_fInterval(0)
	, m_fLast(0)
	, m_nRepeatTimes(-1)
	, m_bAtOnce(false)
{
	this->setCallback(callback);
	this->setRepeatTimes(repeatTimes);
	this->setInterval(interval);
	m_bAtOnce = atOnce;
}

e2d::Timer::Timer(const String & name, const TimerCallback & callback, double interval /* = 0 */, int repeatTimes /* = -1 */, bool atOnce /* = false */)
	: m_bRunning(false)
	, m_nRunTimes(0)
	, m_pParentNode(nullptr)
	, m_Callback(nullptr)
	, m_fInterval(0)
	, m_fLast(0)
	, m_nRepeatTimes(-1)
	, m_bAtOnce(false)
{
	this->setName(name);
	this->setCallback(callback);
	this->setRepeatTimes(repeatTimes);
	this->setInterval(interval);
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

void e2d::Timer::start()
{
	m_bRunning = true;
	m_fLast = Time::getTotalTime();
}

e2d::String e2d::Timer::getName() const
{
	return m_sName;
}

e2d::Node * e2d::Timer::getParentNode() const
{
	return m_pParentNode;
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

void e2d::Timer::setRepeatTimes(int repeatTimes)
{
	m_nRepeatTimes = repeatTimes;
}

void e2d::Timer::setRunAtOnce(bool bAtOnce)
{
	m_bAtOnce = bAtOnce;
}

void e2d::Timer::_callOn()
{
	if (m_Callback)
	{
		m_Callback();
	}

	m_nRunTimes++;
	m_fLast += m_fInterval;

	if (m_nRunTimes == m_nRepeatTimes)
	{
		this->stop();
	}
}

bool e2d::Timer::_isReady() const
{
	if (m_bRunning && 
		m_pParentNode &&
		m_pParentNode->getParentScene() == SceneManager::getCurrentScene())
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
