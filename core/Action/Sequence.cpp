#include "..\e2daction.h"

e2d::Sequence::Sequence()
	: _currIndex(0)
{
}

e2d::Sequence::Sequence(const std::vector<Action*>& actions)
	: _currIndex(0)
{
	this->add(actions);
}

e2d::Sequence::~Sequence()
{
}

void e2d::Sequence::_init()
{
	Action::_init();
	// 将所有动作与目标绑定
	if (_target)
	{
		for (auto action : _actions)
		{
			action->_target = _target;
		}
	}
	// 初始化第一个动作
	_actions[0]->_init();
}

void e2d::Sequence::onDestroy()
{
	Action::onDestroy();
	for (auto action : _actions)
	{
		GC::release(action);
	}
}

void e2d::Sequence::_update()
{
	Action::_update();

	auto &action = _actions[_currIndex];
	action->_update();

	if (action->_isDone())
	{
		++_currIndex;
		if (_currIndex == _actions.size())
		{
			this->stop();
		}
		else
		{
			_actions[_currIndex]->_init();
		}
	}
}

void e2d::Sequence::reset()
{
	Action::reset();
	for (auto action : _actions)
	{
		action->reset();
	}
	_currIndex = 0;
}

void e2d::Sequence::_resetTime()
{
	for (auto action : _actions)
	{
		action->_resetTime();
	}
}

void e2d::Sequence::add(Action * action)
{
	if (action)
	{
		_actions.push_back(action);
		action->retain();
	}
}

void e2d::Sequence::add(const std::vector<Action*>& actions)
{
	for (const auto &action : actions)
	{
		this->add(action);
	}
}

e2d::Sequence * e2d::Sequence::clone() const
{
	auto sequence = GC::create<Sequence>();
	for (const auto& action : _actions)
	{
		if (action)
		{
			sequence->add(action->clone());
		}
	}
	return sequence;
}

e2d::Sequence * e2d::Sequence::reverse() const
{
	auto sequence = GC::create<Sequence>();
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
		sequence->add(newActions);
	}
	return sequence;
}