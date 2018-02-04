#include "..\eactions.h"

e2d::EAction::EAction() 
	: m_bRunning(false)
	, m_bEnding(false)
	, m_bInit(false)
	, m_pTarget(nullptr)
	, m_pParentScene(nullptr)
	, m_fLast(0)
{
}

e2d::EAction::~EAction()
{
}

bool e2d::EAction::isRunning()
{
	return m_bRunning;
}

bool e2d::EAction::_isEnding()
{
	return m_bEnding;
}

void e2d::EAction::start()
{
	this->resume();
}

void e2d::EAction::resume()
{
	m_bRunning = true;
	m_fLast = ETime::getTotalTime();
}

void e2d::EAction::pause()
{
	m_bRunning = false;
}

void e2d::EAction::stop()
{
	m_bEnding = true;
}

void e2d::EAction::_setTarget(ENode * node)
{
	m_pTarget = node;
}

e2d::EAction * e2d::EAction::reverse() const
{
	ASSERT(false, "EAction cannot be reversed!");
	return nullptr;
}

e2d::ENode * e2d::EAction::getTarget()
{
	return m_pTarget;
}

void e2d::EAction::_init()
{
	m_bInit = true;
	// 记录当前时间
	m_fLast = ETime::getTotalTime();
}

void e2d::EAction::_update()
{
	if (!m_bInit)
	{
		_init();
	}
}

void e2d::EAction::_reset()
{
	m_bInit = false;
	m_bEnding = false;
	m_fLast = ETime::getTotalTime();
}

void e2d::EAction::_resetTime()
{
	m_fLast = ETime::getTotalTime();
}
