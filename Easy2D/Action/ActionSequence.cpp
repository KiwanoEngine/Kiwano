#include "..\easy2d.h"
#include <stdarg.h>

ActionSequence::ActionSequence(int number, Action * action1, ...) :
	m_nActionIndex(0)
{
	va_list params;
	va_start(params, number);

	while (number > 0)
	{
		Action* arg = va_arg(params, Action*);
		arg->retain();
		m_vActions.push_back(arg);
		number--;
	}

	va_end(params);
}

ActionSequence::~ActionSequence()
{
	for (auto action : m_vActions)
	{
		SAFE_RELEASE(action);
	}
}

void ActionSequence::_init()
{
	for (auto action : m_vActions)
	{
		action->m_pParent = m_pParent;
	}
	m_vActions[0]->_init();
}

bool ActionSequence::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	if (m_vActions[m_nActionIndex]->_exec(nNow))
	{
		m_nActionIndex++;
		if (m_nActionIndex == m_vActions.size())
		{
			return true;
		}
		else
		{
			m_vActions[m_nActionIndex]->_init();
		}
	}
	return false;
}

void ActionSequence::_reset()
{
	for (auto action : m_vActions)
	{
		action->_reset();
	}
	m_nActionIndex = 0;
}

ActionSequence * ActionSequence::copy()
{
	auto a = new ActionSequence(*this);
	a->_reset();
	return a;
}

ActionSequence * ActionSequence::reverse() const
{
	// 复制一个相同的动作
	auto a = new ActionSequence(*this);
	a->_reset();
	a->m_bRunning = true;
	a->m_bStop = false;
	// 将动作顺序逆序排列
	a->m_vActions.reserve(m_vActions.size());
	// 将所有动作逆向运行
	for (auto action : a->m_vActions)
	{
		action->reverse();
	}
	return a;
}