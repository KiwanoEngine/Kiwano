#include "..\eaction.h"

e2d::ActionTwo::ActionTwo(Action * pActionFirst, Action * pActionSecond, bool bAtSameTime/* = false*/)
	: m_pFirstAction(pActionFirst)
	, m_pSecondAction(pActionSecond)
	, m_bAtSameTime(bAtSameTime)
{
	ASSERT(m_pFirstAction && m_pSecondAction, "ActionTwo NULL pointer exception!");
	m_pFirstAction->retain();
	m_pSecondAction->retain();
}

e2d::ActionTwo::~ActionTwo()
{
}

e2d::ActionTwo * e2d::ActionTwo::clone() const
{
	return new ActionTwo(m_pFirstAction->clone(), m_pSecondAction->clone());
}

e2d::ActionTwo * e2d::ActionTwo::reverse(bool actionReverse) const
{
	if (actionReverse)
	{
		return new ActionTwo(m_pSecondAction->reverse(), m_pFirstAction->reverse());
	}
	else
	{
		return new ActionTwo(m_pSecondAction->clone(), m_pFirstAction->clone());
	}
}

void e2d::ActionTwo::_init()
{
	Action::_init();
	m_pFirstAction->m_pTarget = m_pTarget;
	m_pSecondAction->m_pTarget = m_pTarget;

	m_pFirstAction->_init();
	if (m_bAtSameTime) m_pSecondAction->_init();
}

void e2d::ActionTwo::_update()
{
	Action::_update();

	if (!m_pFirstAction->_isEnding())
	{
		m_pFirstAction->_update();

		if (!m_bAtSameTime && m_pFirstAction->_isEnding())
		{
			m_pSecondAction->_init();
		}
	}

	if (m_bAtSameTime)
	{
		if (!m_pSecondAction->_isEnding())
		{
			m_pSecondAction->_update();
		}
	}
	else if (m_pFirstAction->_isEnding())
	{
		m_pSecondAction->_update();
	}

	if (m_pFirstAction->_isEnding() && m_pSecondAction->_isEnding())
	{
		this->stop();
	}
}

void e2d::ActionTwo::reset()
{
	Action::reset();

	m_pFirstAction->reset();
	m_pSecondAction->reset();
}

void e2d::ActionTwo::destroy()
{
	Action::destroy();
	SafeRelease(&m_pFirstAction);
	SafeRelease(&m_pSecondAction);
}

void e2d::ActionTwo::_resetTime()
{
	m_pFirstAction->_resetTime();
	m_pSecondAction->_resetTime();
}
