#include "..\eactions.h"

e2d::EActionSequence::EActionSequence() :
	m_nActionIndex(0)
{
}

e2d::EActionSequence::EActionSequence(int number, EAction * action1, ...) :
	m_nActionIndex(0)
{
	EAction ** ppAction = &action1;

	while (number > 0)
	{
		ASSERT((*ppAction) != nullptr, "EActionSequence NULL pointer exception!");
		this->addAction(*ppAction);
		ppAction++;
		number--;
	}
}

e2d::EActionSequence::~EActionSequence()
{
	for (auto action : m_vActions)
	{
		SafeRelease(&action);
	}
}

void e2d::EActionSequence::_init()
{
	EAction::_init();
	// 将所有动作与目标绑定
	if (m_pTarget)
	{
		for (auto action : m_vActions)
		{
			action->setTarget(m_pTarget);
		}
	}
	// 初始化第一个动作
	m_vActions[0]->_init();
}

void e2d::EActionSequence::_update()
{
	EAction::_update();

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

void e2d::EActionSequence::_reset()
{
	EAction::_reset();
	for (auto action : m_vActions)
	{
		action->_reset();
	}
	m_nActionIndex = 0;
}

void e2d::EActionSequence::_resetTime()
{
	for (auto action : m_vActions)
	{
		action->_resetTime();
	}
}

void e2d::EActionSequence::addAction(EAction * action)
{
	if (action)
	{
		m_vActions.push_back(action);
		action->retain();
	}
}

e2d::EActionSequence * e2d::EActionSequence::clone() const
{
	auto a = new EActionSequence();
	for (auto action : m_vActions)
	{
		a->addAction(action->clone());
	}
	return a;
}

e2d::EActionSequence * e2d::EActionSequence::reverse(bool actionReverse) const
{
	auto a = new EActionSequence();
	for (auto action : m_vActions)
	{
		if (actionReverse)
		{
			a->addAction(action->reverse());
		}
		else
		{
			a->addAction(action->clone());
		}
	}
	// 将动作顺序逆序排列
	a->m_vActions.reserve(m_vActions.size());
	return a;
}