#include "..\e2daction.h"


e2d::ActionMoveBy::ActionMoveBy(double duration, Vector vector) :
	ActionGradual(duration)
{
	_MoveVec = vector;
}

void e2d::ActionMoveBy::_init()
{
	ActionGradual::_init();
	if (_pTarget)
	{
		_BeginPos = _pTarget->getPos();
	}
}

void e2d::ActionMoveBy::_update()
{
	ActionGradual::_update();

	if (_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// ÒÆ¶¯½Úµã
	_pTarget->setPos(_BeginPos + _MoveVec * _fRateOfProgress);
}

e2d::ActionMoveBy * e2d::ActionMoveBy::clone() const
{
	return new ActionMoveBy(_fDuration, _MoveVec);
}

e2d::ActionMoveBy * e2d::ActionMoveBy::reverse() const
{
	return new ActionMoveBy(_fDuration, -_MoveVec);
}