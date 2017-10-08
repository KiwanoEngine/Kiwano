#include "..\easy2d.h"

ActionTwo::ActionTwo(Action * actionFirst, Action * actionSecond) :
	m_FirstAction(actionFirst),
	m_SecondAction(actionSecond)
{
	m_FirstAction->retain();
	m_SecondAction->retain();
}

ActionTwo::~ActionTwo()
{
	SafeRelease(m_FirstAction);
	SafeRelease(m_SecondAction);
}

ActionTwo * ActionTwo::copy() const
{
	return new ActionTwo(m_FirstAction->copy(), m_SecondAction->copy());
}

ActionTwo * ActionTwo::reverse(bool actionReverse) const
{
	if (actionReverse)
	{
		return new ActionTwo(m_SecondAction->reverse(), m_FirstAction->reverse());
	}
	else
	{
		return new ActionTwo(m_SecondAction->copy(), m_FirstAction->copy());
	}
}

void ActionTwo::_init()
{
	Action::_init();
	m_FirstAction->m_pTargetSprite = m_pTargetSprite;
	m_SecondAction->m_pTargetSprite = m_pTargetSprite;

	m_FirstAction->_init();
}

void ActionTwo::_exec(LARGE_INTEGER nNow)
{
	if (!m_FirstAction->isEnding())
	{
		m_FirstAction->_exec(nNow);
		if (m_FirstAction->isEnding())
		{
			// 返回 true 表示第一个动作已经结束
			m_SecondAction->_init();
		}
	}
	else if (!m_SecondAction->isEnding())
	{
		m_SecondAction->_exec(nNow);
	}
	else
	{
		this->stop();
	}
}

void ActionTwo::_reset()
{
	Action::_reset();

	m_FirstAction->_reset();
	m_SecondAction->_reset();
}
