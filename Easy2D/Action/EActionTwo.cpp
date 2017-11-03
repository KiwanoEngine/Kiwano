#include "..\eactions.h"

e2d::EActionTwo::EActionTwo(EAction * actionFirst, EAction * actionSecond) :
	m_pFirstAction(actionFirst),
	m_pSecondAction(actionSecond)
{
	m_pFirstAction->retain();
	m_pSecondAction->retain();
}

e2d::EActionTwo::~EActionTwo()
{
	SafeRelease(&m_pFirstAction);
	SafeRelease(&m_pSecondAction);
}

e2d::EActionTwo * e2d::EActionTwo::clone() const
{
	return new EActionTwo(m_pFirstAction->clone(), m_pSecondAction->clone());
}

e2d::EActionTwo * e2d::EActionTwo::reverse(bool actionReverse) const
{
	if (actionReverse)
	{
		return new EActionTwo(m_pSecondAction->reverse(), m_pFirstAction->reverse());
	}
	else
	{
		return new EActionTwo(m_pSecondAction->clone(), m_pFirstAction->clone());
	}
}

void e2d::EActionTwo::_init()
{
	EAction::_init();
	m_pFirstAction->setTarget(m_pTarget);
	m_pSecondAction->setTarget(m_pTarget);

	m_pFirstAction->_init();
}

void e2d::EActionTwo::_callOn()
{
	EAction::_callOn();

	if (!m_pFirstAction->_isEnding())
	{
		m_pFirstAction->_callOn();
		if (m_pFirstAction->_isEnding())
		{
			// 返回 true 表示第一个动作已经结束
			m_pSecondAction->_init();
		}
	}
	else if (!m_pSecondAction->_isEnding())
	{
		m_pSecondAction->_callOn();
	}
	else
	{
		this->stop();
	}
}

void e2d::EActionTwo::_reset()
{
	EAction::_reset();

	m_pFirstAction->_reset();
	m_pSecondAction->_reset();
}
