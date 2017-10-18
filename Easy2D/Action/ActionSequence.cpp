#include "..\eactions.h"
#include <stdarg.h>

e2d::ActionSequence::ActionSequence() :
	m_nActionIndex(0)
{
}

e2d::ActionSequence::ActionSequence(int number, EAction * action1, ...) :
	m_nActionIndex(0)
{
	va_list params;
	va_start(params, number);

	while (number > 0)
	{
		this->addAction(va_arg(params, EAction*));
		number--;
	}

	va_end(params);
}

e2d::ActionSequence::~ActionSequence()
{
	for (auto action : m_vActions)
	{
		SafeRelease(&action);
	}
}

void e2d::ActionSequence::_init()
{
	EAction::_init();
	// 将所有动作与目标绑定
	for (auto action : m_vActions)
	{
		action->m_pTarget = m_pTarget;
	}
	// 初始化第一个动作
	m_vActions[0]->_init();
}

void e2d::ActionSequence::_exec()
{
	m_vActions[m_nActionIndex]->_exec();

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

void e2d::ActionSequence::_reset()
{
	EAction::_reset();
	for (auto action : m_vActions)
	{
		action->_reset();
	}
	m_nActionIndex = 0;
}

void e2d::ActionSequence::addAction(EAction * action)
{
	m_vActions.push_back(action);
	action->retain();
}

e2d::ActionSequence * e2d::ActionSequence::copy() const
{
	auto a = new ActionSequence();
	for (auto action : m_vActions)
	{
		a->addAction(action->copy());
	}
	return a;
}

e2d::ActionSequence * e2d::ActionSequence::reverse(bool actionReverse) const
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