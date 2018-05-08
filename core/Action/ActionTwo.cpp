#include "..\e2daction.h"

e2d::ActionTwo::ActionTwo(ActionBase * pActionFirst, ActionBase * pActionSecond, bool bAtSameTime/* = false*/)
	: _pFirstAction(pActionFirst)
	, _pSecondAction(pActionSecond)
	, _bAtSameTime(bAtSameTime)
{
	ASSERT(_pFirstAction && _pSecondAction, "ActionTwo NULL pointer exception!");
	_pFirstAction->retain();
	_pSecondAction->retain();
}

e2d::ActionTwo::~ActionTwo()
{
}

e2d::ActionTwo * e2d::ActionTwo::clone() const
{
	return new ActionTwo(_pFirstAction->clone(), _pSecondAction->clone());
}

e2d::ActionTwo * e2d::ActionTwo::reverse(bool actionReverse) const
{
	if (actionReverse)
	{
		return new ActionTwo(_pSecondAction->reverse(), _pFirstAction->reverse());
	}
	else
	{
		return new ActionTwo(_pSecondAction->clone(), _pFirstAction->clone());
	}
}

void e2d::ActionTwo::_init()
{
	ActionBase::_init();
	_pFirstAction->_pTarget = _pTarget;
	_pSecondAction->_pTarget = _pTarget;

	_pFirstAction->_init();
	if (_bAtSameTime) _pSecondAction->_init();
}

void e2d::ActionTwo::_update()
{
	ActionBase::_update();

	if (!_pFirstAction->_isDone())
	{
		_pFirstAction->_update();

		if (!_bAtSameTime && _pFirstAction->_isDone())
		{
			_pSecondAction->_init();
		}
	}

	if (_bAtSameTime)
	{
		if (!_pSecondAction->_isDone())
		{
			_pSecondAction->_update();
		}
	}
	else if (_pFirstAction->_isDone())
	{
		_pSecondAction->_update();
	}

	if (_pFirstAction->_isDone() && _pSecondAction->_isDone())
	{
		this->stop();
	}
}

void e2d::ActionTwo::reset()
{
	ActionBase::reset();

	_pFirstAction->reset();
	_pSecondAction->reset();
}

void e2d::ActionTwo::destroy()
{
	ActionBase::destroy();
	SafeRelease(&_pFirstAction);
	SafeRelease(&_pSecondAction);
}

void e2d::ActionTwo::_resetTime()
{
	_pFirstAction->_resetTime();
	_pSecondAction->_resetTime();
}
