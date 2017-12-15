#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EAction::EAction() :
	m_bRunning(false),
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
	m_tLast = GetNow();
}

void e2d::EAction::pause()
{
	m_bRunning = false;
}

void e2d::EAction::stop()
{
	m_bEnding = true;
}

void e2d::EAction::setInterval(LONGLONG milliSeconds)
{
	// 设置动作的时间间隔
	SetInterval(m_nAnimationInterval, milliSeconds);
}

void e2d::EAction::setTarget(ENode * node)
{
	if (m_pTarget)
	{
		// 动作只能有一个目标
		return;
	}

	if (node)
	{
		m_pTarget = node;
	}
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
	// 记录当前时间
	m_tLast = GetNow();
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
	m_tLast = GetNow();
}

void e2d::EAction::_resetTime()
{
	m_tLast = GetNow();
}
