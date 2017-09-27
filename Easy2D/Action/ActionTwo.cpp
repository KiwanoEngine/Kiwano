#include "..\easy2d.h"

ActionTwo::ActionTwo(Action * actionFirst, Action * actionSecond) :
	m_FirstAction(actionFirst),
	m_SecondAction(actionSecond)
{
	if (m_FirstAction) m_FirstAction->retain();
	if (m_SecondAction) m_SecondAction->retain();
}

ActionTwo::~ActionTwo()
{
	SAFE_RELEASE(m_FirstAction);
	SAFE_RELEASE(m_SecondAction);
}

ActionTwo * ActionTwo::copy()
{
	auto a = new ActionTwo(*this);
	a->_reset();
	return a;
}

ActionTwo * ActionTwo::reverse() const
{
	return new ActionTwo(m_SecondAction->copy(), m_FirstAction->copy());
}

void ActionTwo::_init()
{
	m_FirstAction->m_pParent = m_pParent;
	m_FirstAction->_init();
	
	m_SecondAction->m_pParent = m_pParent;
}

bool ActionTwo::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	if (m_FirstAction)
	{
		if (m_FirstAction->_exec(nNow))
		{
			// 返回 true 表示第一个动作已经结束，删除这个
			// 动作，并初始化第二个动作
			SAFE_RELEASE(m_FirstAction);
			m_FirstAction = nullptr;
			m_SecondAction->_init();
		}
	}
	else if (m_SecondAction)
	{
		if (m_SecondAction->_exec(nNow))
		{
			SAFE_RELEASE(m_SecondAction);
			m_SecondAction = nullptr;
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;
}

void ActionTwo::_reset()
{
	if (m_FirstAction) m_FirstAction->_reset();
	if (m_SecondAction) m_SecondAction->_reset();
}
