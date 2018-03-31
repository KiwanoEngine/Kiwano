#include "..\eactions.h"

e2d::ActionSequence::ActionSequence() :
	m_nActionIndex(0)
{
}

e2d::ActionSequence::ActionSequence(const std::initializer_list<Action*>& vActions) :
	m_nActionIndex(0)
{
	this->add(vActions);
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
	Action::_init();
	// 将所有动作与目标绑定
	if (m_pTarget)
	{
		for (auto action : m_vActions)
		{
			action->m_pTarget = m_pTarget;
		}
	}
	// 初始化第一个动作
	m_vActions[0]->_init();
}

void e2d::ActionSequence::_update()
{
	Action::_update();

	auto &action = m_vActions[m_nActionIndex];
	action->_update();

	if (action->_isEnding())
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

void e2d::ActionSequence::reset()
{
	Action::reset();
	for (auto action : m_vActions)
	{
		action->reset();
	}
	m_nActionIndex = 0;
}

void e2d::ActionSequence::_resetTime()
{
	for (auto action : m_vActions)
	{
		action->_resetTime();
	}
}

void e2d::ActionSequence::add(Action * action)
{
	if (action)
	{
		m_vActions.push_back(action);
		action->retain();
	}
}

void e2d::ActionSequence::add(const std::initializer_list<Action*>& vActions)
{
	for (const auto &action : vActions)
	{
		this->add(action);
	}
}

e2d::ActionSequence * e2d::ActionSequence::clone() const
{
	auto a = new ActionSequence();
	for (auto action : m_vActions)
	{
		a->add(action->clone());
	}
	return a;
}

e2d::ActionSequence * e2d::ActionSequence::reverse(bool actionReverse) const
{
	auto a = new ActionSequence();
	for (auto action : m_vActions)
	{
		if (actionReverse)
		{
			a->add(action->reverse());
		}
		else
		{
			a->add(action->clone());
		}
	}
	// 将动作顺序逆序排列
	a->m_vActions.reserve(m_vActions.size());
	return a;
}