#include "..\e2daction.h"

e2d::ActionSequence::ActionSequence()
	: _nActionIndex(0)
{
}

#ifdef HIGHER_THAN_VS2012
e2d::ActionSequence::ActionSequence(const std::initializer_list<ActionBase*>& vActions)
	: _nActionIndex(0)
{
	this->add(vActions);
}
#else
e2d::ActionSequence::ActionSequence(int number, ActionBase * action1, ...) :
	_nActionIndex(0)
{
	ActionBase ** ppAction = &action1;

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
}

void e2d::ActionSequence::_init()
{
	ActionBase::_init();
	// 将所有动作与目标绑定
	if (_pTarget)
	{
		for (auto action : _vActions)
		{
			action->_pTarget = _pTarget;
		}
	}
	// 初始化第一个动作
	_vActions[0]->_init();
}

void e2d::ActionSequence::destroy()
{
	ActionBase::destroy();
	for (auto action : _vActions)
	{
		SafeRelease(&action);
	}
}

void e2d::ActionSequence::_update()
{
	ActionBase::_update();

	auto &action = _vActions[_nActionIndex];
	action->_update();

	if (action->_isDone())
	{
		_nActionIndex++;
		if (_nActionIndex == _vActions.size())
		{
			this->stop();
		}
		else
		{
			_vActions[_nActionIndex]->_init();
		}
	}
}

void e2d::ActionSequence::reset()
{
	ActionBase::reset();
	for (auto action : _vActions)
	{
		action->reset();
	}
	_nActionIndex = 0;
}

void e2d::ActionSequence::_resetTime()
{
	for (auto action : _vActions)
	{
		action->_resetTime();
	}
}

void e2d::ActionSequence::add(ActionBase * action)
{
	if (action)
	{
		_vActions.push_back(action);
		action->retain();
	}
}

#ifdef HIGHER_THAN_VS2012
void e2d::ActionSequence::add(const std::initializer_list<ActionBase*>& vActions)
{
	for (const auto &action : vActions)
	{
		this->add(action);
	}
}
#else
void e2d::ActionSequence::add(int number, ActionBase * action, ...)
{
	ActionBase ** ppAction = &action;

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
	for (auto action : _vActions)
	{
		a->add(action->clone());
	}
	return a;
}

e2d::ActionSequence * e2d::ActionSequence::reverse(bool actionReverse) const
{
	auto a = new ActionSequence();
	for (auto action : _vActions)
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
	a->_vActions.reserve(_vActions.size());
	return a;
}