#include "..\eactions.h"
#include "..\emanagers.h"

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
	EActionManager::__destroyAction(this);
}

bool e2d::EAction::isRunning()
{
	return m_bRunning;
}

bool e2d::EAction::_isEnding()
{
	return m_bEnding;
}

void e2d::EAction::startWith(ENode* pTarget)
{
	m_bRunning = true;
	m_pTarget = pTarget;
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

void e2d::EAction::reset()
{
	m_bInit = false;
	m_bEnding = false;
	m_fLast = ETime::getTotalTime();
}

void e2d::EAction::_resetTime()
{
	m_fLast = ETime::getTotalTime();
}
