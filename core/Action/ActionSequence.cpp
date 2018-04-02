#include "..\eaction.h"

e2d::ActionSequence::ActionSequence()
	: m_nActionIndex(0)
{
}

#if HIGHER_THAN_VS2012
e2d::ActionSequence::ActionSequence(const InitList<Action*>& vActions)
	: m_nActionIndex(0)
{
	this->add(vActions);
}
#else
e2d::ActionSequence::ActionSequence(int number, Action * action1, ...) :
	m_nActionIndex(0)
{
	Action ** ppAction = &action1;

	while (number > 0)
	{
		WARN_IF((*ppAction) == nullptr, "ActionSequence NULL pointer exception!");
		this->add(*ppAction);
		ppAction++;
		number--;
	}
}
#endif

e2d::ActionSequence::~ActionSequence()
{
	FOR_LOOP(action, m_vActions)
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
		FOR_LOOP(action, m_vActions)
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
	FOR_LOOP(action, m_vActions)
	{
		action->reset();
	}
	m_nActionIndex = 0;
}

void e2d::ActionSequence::_resetTime()
{
	FOR_LOOP(action, m_vActions)
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

#if HIGHER_THAN_VS2012
void e2d::ActionSequence::add(const InitList<Action*>& vActions)
{
	for (const auto &action : vActions)
	{
		this->add(action);
	}
}
#else
void e2d::ActionSequence::add(int number, Action * action, ...)
{
	Action ** ppAction = &action;

	while (number > 0)
	{
		WARN_IF((*ppAction) == nullptr, "ActionSequence NULL pointer exception!");
		this->add(*ppAction);
		ppAction++;
		number--;
	}
}
#endif

e2d::ActionSequence * e2d::ActionSequence::clone() const
{
	auto a = new ActionSequence();
	FOR_LOOP(action, m_vActions)
	{
		a->add(action->clone());
	}
	return a;
}

e2d::ActionSequence * e2d::ActionSequence::reverse(bool actionReverse) const
{
	auto a = new ActionSequence();
	FOR_LOOP(action, m_vActions)
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