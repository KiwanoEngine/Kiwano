#include "..\e2daction.h"

e2d::Spawn::Spawn()
{
}

e2d::Spawn::Spawn(int number, Action * action, ...)
{
	va_list args;
	va_start(args, action);

	this->add(action);
	for (int i = 1; i < number; i++)
	{
		this->add(va_arg(args, Action*));
	}

	va_end(args);
}

#ifdef HIGHER_THAN_VS2012
e2d::Spawn::Spawn(const std::initializer_list<Action*>& actions)
{
	this->add(actions);
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

void e2d::Spawn::add(int number, Action * action, ...)
{
	va_list args;
	va_start(args, action);

	this->add(action);
	for (int i = 1; i < number; i++)
	{
		this->add(va_arg(args, Action*));
	}

	va_end(args);
}

#ifdef HIGHER_THAN_VS2012
void e2d::Spawn::add(const std::initializer_list<Action*>& actions)
{
	for (const auto &action : actions)
	{
		this->add(action);
	}
}
#endif

e2d::Spawn * e2d::Spawn::clone() const
{
	auto spawn = new (std::nothrow) Spawn();
	for (const auto& action : _actions)
	{
		if (action)
		{
			spawn->add(action->clone());
		}
	}
	return spawn;
}

e2d::Spawn * e2d::Spawn::reverse() const
{
	auto spawn = new (std::nothrow) Spawn();
	for (const auto& action : _actions)
	{
		if (action)
		{
			spawn->add(action->reverse());
		}
	}
	spawn->_actions.reserve(_actions.size());
	return spawn;
}