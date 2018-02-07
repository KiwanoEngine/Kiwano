#include "..\eactions.h"

e2d::ActionLoop::ActionLoop(Action * action, int times /* = -1 */)
	: m_pAction(action)
	, m_nTimes(0)
	, m_nTotalTimes(times)
{
	ASSERT(m_pAction != nullptr, "ActionLoop NULL pointer exception!");
	m_pAction->retain();
}

e2d::ActionLoop::~ActionLoop()
{
	SafeRelease(&m_pAction);
}

e2d::ActionLoop * e2d::ActionLoop::clone() const
{
	return new ActionLoop(m_pAction->clone());
}

void e2d::ActionLoop::_init()
{
	Action::_init();
	m_pAction->m_pTarget = m_pTarget;
	m_pAction->_init();
}

void e2d::ActionLoop::_update()
{
	Action::_update();

	if (m_nTimes == m_nTotalTimes)
	{
		this->stop();
		return;
	}

	m_pAction->_update();

	if (m_pAction->_isEnding())
	{
		m_nTimes++;
		
		Action::reset();
		m_pAction->reset();
	}
}

void e2d::ActionLoop::reset()
{
	Action::reset();

	m_pAction->reset();
	m_nTimes = 0;
}

void e2d::ActionLoop::_resetTime()
{
	m_pAction->_resetTime();
}
