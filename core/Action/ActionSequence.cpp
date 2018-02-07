#include "..\eactions.h"

e2d::ActionSequence::ActionSequence() :
	m_nActionIndex(0)
{
}

e2d::ActionSequence::ActionSequence(int number, Action * action1, ...) :
	m_nActionIndex(0)
{
	Action ** ppAction = &action1;

	while (number > 0)
	{
		ASSERT((*ppAction) != nullptr, "ActionSequence NULL pointer exception!");
		this->_add(*ppAction);
		ppAction++;
		number--;
	}
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
	// �����ж�����Ŀ���
	if (m_pTarget)
	{
		for (auto action : m_vActions)
		{
			action->m_pTarget = m_pTarget;
		}
	}
	// ��ʼ����һ������
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

void e2d::ActionSequence::_add(Action * action)
{
	if (action)
	{
		m_vActions.push_back(action);
		action->retain();
	}
}

e2d::ActionSequence * e2d::ActionSequence::clone() const
{
	auto a = new ActionSequence();
	for (auto action : m_vActions)
	{
		a->_add(action->clone());
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
			a->_add(action->reverse());
		}
		else
		{
			a->_add(action->clone());
		}
	}
	// ������˳����������
	a->m_vActions.reserve(m_vActions.size());
	return a;
}