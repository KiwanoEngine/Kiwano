#include "..\e2daction.h"

e2d::Spawn::Spawn()
{
}

e2d::Spawn::Spawn(const std::vector<Action*>& actions)
{
	this->add(actions);
}

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
		SafeRelease(action);
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

void e2d::Spawn::add(const std::vector<Action*>& actions)
{
	for (const auto &action : actions)
	{
		this->add(action);
	}
}

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
	if (!_actions.empty())
	{
		std::vector<Action*> newActions(_actions.size());
		for (auto iter = _actions.crbegin(), iterCrend = _actions.crend(); iter != iterCrend; ++iter)
		{
			if (*iter)
			{
				newActions.push_back((*iter)->reverse());
			}
		}
		spawn->add(newActions);
	}
	return spawn;
}