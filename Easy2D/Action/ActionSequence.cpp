#include "..\easy2d.h"
#include <stdarg.h>

ActionSequence::ActionSequence() :
	m_nActionIndex(0)
{
}

ActionSequence::ActionSequence(int number, Action * action1, ...) :
	m_nActionIndex(0)
{
	va_list params;
	va_start(params, number);

	while (number > 0)
	{
		this->addAction(va_arg(params, Action*));
		number--;
	}

	va_end(params);
}

ActionSequence::~ActionSequence()
{
	for (auto action : m_vActions)
	{
		SafeRelease(action);
	}
}

void ActionSequence::_init()
{
	Action::_init();
	// 将所有动作与目标绑定
	for (auto action : m_vActions)
	{
		action->m_pTargetSprite = m_pTargetSprite;
	}
	// 初始化第一个动作
	m_vActions[0]->_init();
}

void ActionSequence::_exec(LARGE_INTEGER nNow)
{
	m_vActions[m_nActionIndex]->_exec(nNow);

	if (m_vActions[m_nActionIndex]->isEnding())
	{
		m_nActionIndex++;
		if (m_nActionIndex == m_vActions.size())
		{
			this->stop();
		}
		else
		{
			m_vActions[m_nActionIndex]->_init();
		}
	}
}

void ActionSequence::_reset()
{
	Action::_reset();
	for (auto action : m_vActions)
	{
		action->_reset();
	}
	m_nActionIndex = 0;
}

void ActionSequence::addAction(Action * action)
{
	m_vActions.push_back(action);
	action->retain();
}

ActionSequence * ActionSequence::copy() const
{
	auto a = new ActionSequence();
	for (auto action : m_vActions)
	{
		a->addAction(action->copy());
	}
	return a;
}

ActionSequence * ActionSequence::reverse(bool actionReverse) const
{
	auto a = new ActionSequence();
	for (auto action : a->m_vActions)
	{
		if (actionReverse)
		{
			a->addAction(action->reverse());
		}
		else
		{
			a->addAction(action->copy());
		}
	}
	// 将动作顺序逆序排列
	a->m_vActions.reserve(m_vActions.size());
	return a;
}