#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::ActionLoop::ActionLoop(ActionBase * action, int times /* = -1 */)
	: _pAction(action)
	, _nTimes(0)
	, _nTotalTimes(times)
{
	ASSERT(_pAction, "ActionLoop NULL pointer exception!");
	_pAction->retain();
}

e2d::ActionLoop::~ActionLoop()
{
}

e2d::ActionLoop * e2d::ActionLoop::clone() const
{
	return new ActionLoop(_pAction->clone());
}

void e2d::ActionLoop::_init()
{
	ActionBase::_init();
	_pAction->_pTarget = _pTarget;
	_pAction->_init();
}

void e2d::ActionLoop::_update()
{
	ActionBase::_update();

	if (_nTimes == _nTotalTimes)
	{
		this->stop();
		return;
	}

	_pAction->_update();

	if (_pAction->_isDone())
	{
		_nTimes++;
		
		ActionBase::reset();
		_pAction->reset();
	}
}

void e2d::ActionLoop::reset()
{
	ActionBase::reset();

	_pAction->reset();
	_nTimes = 0;
}

void e2d::ActionLoop::destroy()
{
	ActionBase::destroy();
	SafeRelease(&_pAction);
}

void e2d::ActionLoop::_resetTime()
{
	_pAction->_resetTime();
}
