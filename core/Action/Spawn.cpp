#include "..\e2daction.h"

e2d::Spawn::Spawn()
{
}

#ifdef HIGHER_THAN_VS2012
e2d::Spawn::Spawn(const std::initializer_list<Action*>& vActions)
{
	this->add(vActions);
}
#else
e2d::Spawn::Spawn(int number, Action * action1, ...) :
	_currIndex(0)
{
	Action ** ppAction = &action1;

	while (number > 0)
	{
		WARN_IF((*ppAction) == nullptr, "Spawn NULL pointer exception!");
		this->add(*ppAction);
		ppAction++;
		number--;
	}
}
#endif

e2d::Spawn::~Spawn()
{
}

void e2d::Spawn::_init()
{
	Action::_init();

	if (_target)
	{
		for (auto action : _actions)
		{
			action->_target = _target;
			action->_init();
		}
	}
}

void e2d::Spawn::onDestroy()
{
	Action::onDestroy();
	for (auto action : _actions)
	{
		SafeRelease(&action);
	}
}

void e2d::Spawn::_update()
{
	Action::_update();

	size_t doneNum = 0;
	for (auto action : _actions)
	{
		if (action->_isDone())
		{
			doneNum++;
		}
		else
		{
			action->_update();
		}
	}

	if (doneNum == _actions.size())
	{
		this->stop();
	}
}

void e2d::Spawn::reset()
{
	Action::reset();
	for (auto action : _actions)
	{
		action->reset();
	}
}

void e2d::Spawn::_resetTime()
{
	for (auto action : _actions)
	{
		action->_resetTime();
	}
}

void e2d::Spawn::add(Action * action)
{
	if (action)
	{
		_actions.push_back(action);
		action->retain();
	}
}

#ifdef HIGHER_THAN_VS2012
void e2d::Spawn::add(const std::initializer_list<Action*>& vActions)
{
	for (const auto &action : vActions)
	{
		this->add(action);
	}
}
#else
void e2d::Spawn::add(int number, Action * action, ...)
{
	Action ** ppAction = &action;

	while (number > 0)
	{
		WARN_IF((*ppAction) == nullptr, "Spawn NULL pointer exception!");
		this->add(*ppAction);
		ppAction++;
		number--;
	}
}
#endif

e2d::Spawn * e2d::Spawn::clone() const
{
	auto a = new Spawn();
	for (auto action : _actions)
	{
		a->add(action->clone());
	}
	return a;
}

e2d::Spawn * e2d::Spawn::reverse() const
{
	auto a = new Spawn();
	for (auto action : _actions)
	{
		a->add(action->reverse());
	}
	// 将动作顺序逆序排列
	a->_actions.reserve(_actions.size());
	return a;
}