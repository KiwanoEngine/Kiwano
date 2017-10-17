#include "etools.h"

e2d::ETimer::ETimer()
	: m_bRunning(false)
	, m_bWaiting(false)
	, m_nRunTimes(0)
	, m_pParentScene(nullptr)
	, m_pParentNode(nullptr)
	, m_Callback([](int) {})
	, m_nInterval(20LL)
{
}

e2d::ETimer::ETimer(const EString & name)
	: ETimer()
{
	m_sName = name;
}

e2d::ETimer::ETimer(const TIMER_CALLBACK & callback, LONGLONG delay /* = 20LL */)
	: ETimer()
{
	m_Callback = callback;
}

e2d::ETimer::ETimer(const EString & name, const TIMER_CALLBACK & callback, LONGLONG delay /* = 20LL */)
	: ETimer()
{
	m_sName = name;
	m_Callback = callback;
}

bool e2d::ETimer::isRunning() const
{
	return m_bRunning && !m_bWaiting;
}

bool e2d::ETimer::isWaiting() const
{
	return m_bWaiting;
}

void e2d::ETimer::start()
{
	m_bRunning = true;
	m_tLast = std::chrono::steady_clock::now();
}

void e2d::ETimer::stop()
{
	m_bRunning = false;
}

void e2d::ETimer::_wait()
{
	m_bWaiting = true;
}

void e2d::ETimer::_notify()
{
	m_bWaiting = false;
	m_tLast = std::chrono::steady_clock::now();
}

e2d::EString e2d::ETimer::getName() const
{
	return m_sName;
}

e2d::EScene * e2d::ETimer::getParentScene() const
{
	return m_pParentScene;
}

e2d::ENode * e2d::ETimer::getParentNode() const
{
	return m_pParentNode;
}

void e2d::ETimer::setName(const EString & name)
{
	m_sName = name;
}

void e2d::ETimer::setInterval(LONGLONG interval)
{
	m_nInterval = max(interval, 0);
}

void e2d::ETimer::bindWith(EScene * pParentScene)
{
	ETimerManager::bindTimer(this, pParentScene);
}

void e2d::ETimer::bindWith(ENode * pParentNode)
{
	ETimerManager::bindTimer(this, pParentNode);
}

void e2d::ETimer::_runCallback()
{
	m_Callback(m_nRunTimes);
	m_nRunTimes++;
}
