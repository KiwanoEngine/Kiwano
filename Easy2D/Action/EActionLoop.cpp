#include "..\eactions.h"

e2d::EActionLoop::EActionLoop(EAction * action, int times /* = -1 */)
	: m_pAction(action)
	, m_nTimes(0)
	, m_nTotalTimes(times)
{
	m_pAction->retain();
}

e2d::EActionLoop::~EActionLoop()
{
	SafeRelease(&m_pAction);
}

e2d::EActionLoop * e2d::EActionLoop::clone() const
{
	return new EActionLoop(m_pAction->clone());
}

void e2d::EActionLoop::_init()
{
	EAction::_init();
	m_pAction->setTarget(m_pTarget);
	m_pAction->_init();
}

void e2d::EActionLoop::_callOn()
{
	EAction::_callOn();

	if (m_nTimes == m_nTotalTimes)
	{
		this->stop();
		return;
	}

	m_pAction->_callOn();

	if (m_pAction->_isEnding())
	{
		m_nTimes++;
		
		EAction::_reset();
		m_pAction->_reset();
	}
}

void e2d::EActionLoop::_reset()
{
	EAction::_reset();

	m_pAction->_reset();
	m_nTimes = 0;
}
