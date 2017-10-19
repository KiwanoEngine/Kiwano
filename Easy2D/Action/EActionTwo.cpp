#include "..\eactions.h"

e2d::EActionTwo::EActionTwo(EAction * actionFirst, EAction * actionSecond) :
	m_FirstAction(actionFirst),
	m_SecondAction(actionSecond)
{
	m_FirstAction->retain();
	m_SecondAction->retain();
}

e2d::EActionTwo::~EActionTwo()
{
	SafeRelease(&m_FirstAction);
	SafeRelease(&m_SecondAction);
}

e2d::EActionTwo * e2d::EActionTwo::copy() const
{
	return new EActionTwo(m_FirstAction->copy(), m_SecondAction->copy());
}

e2d::EActionTwo * e2d::EActionTwo::reverse(bool actionReverse) const
{
	if (actionReverse)
	{
		return new EActionTwo(m_SecondAction->reverse(), m_FirstAction->reverse());
	}
	else
	{
		return new EActionTwo(m_SecondAction->copy(), m_FirstAction->copy());
	}
}

void e2d::EActionTwo::_init()
{
	EAction::_init();
	m_FirstAction->m_pTarget = m_pTarget;
	m_SecondAction->m_pTarget = m_pTarget;

	m_FirstAction->_init();
}

void e2d::EActionTwo::_exec()
{
	if (!m_FirstAction->isEnding())
	{
		m_FirstAction->_exec();
		if (m_FirstAction->isEnding())
		{
			// 返回 true 表示第一个动作已经结束
			m_SecondAction->_init();
		}
	}
	else if (!m_SecondAction->isEnding())
	{
		m_SecondAction->_exec();
	}
	else
	{
		this->stop();
	}
}

void e2d::EActionTwo::_reset()
{
	EAction::_reset();

	m_FirstAction->_reset();
	m_SecondAction->_reset();
}
