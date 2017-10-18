#include "..\eactions.h"

e2d::EAction::EAction() :
	m_bRunning(true),
	m_bWaiting(false),
	m_bEnding(false),
	m_bInit(false),
	m_pTarget(nullptr),
	m_pParentScene(nullptr)
{
	// 默认动作 15ms 运行一次
	setInterval(15);
}

e2d::EAction::~EAction()
{
}

bool e2d::EAction::isRunning()
{
	return m_bRunning && !m_bWaiting;
}

bool e2d::EAction::isEnding()
{
	return m_bEnding;
}

void e2d::EAction::start()
{
	m_bRunning = true;
}

void e2d::EAction::resume()
{
	m_bRunning = true;
}

void e2d::EAction::pause()
{
	m_bRunning = false;
}

void e2d::EAction::stop()
{
	m_bEnding = true;
}
void e2d::EAction::wait()
{
	m_bWaiting = true;
}

void e2d::EAction::notify()
{
	m_bWaiting = false;
}

void e2d::EAction::setInterval(LONGLONG milliSeconds)
{
	// 设置动作的时间间隔
	m_nAnimationInterval = milliSeconds;
}

e2d::EAction * e2d::EAction::reverse() const
{
	assert(0);
	return nullptr;
}

e2d::ENode * e2d::EAction::getTarget()
{
	return m_pTarget;
}

void e2d::EAction::_init()
{
	m_bInit = true;
}

void e2d::EAction::_reset()
{
	m_bInit = false;
	m_bEnding = false;
}
