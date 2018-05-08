#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionMove::TransitionMove(double duration, Direct direct)
	: TransitionBase(duration)
	, _Direct(direct)
{
}

void e2d::TransitionMove::_init(Scene * prev, Scene * next)
{
	TransitionBase::_init(prev, next);

	double width = _WindowSize.width;
	double height = _WindowSize.height;
	if (_Direct == Direct::UP)
	{
		_Vector = Vector(0, -height);
		_NextPos = Point(0, height);
	}
	else if (_Direct == Direct::DOWN)
	{
		_Vector = Vector(0, height);
		_NextPos = Point(0, -height);
	}
	else if (_Direct == Direct::LEFT)
	{
		_Vector = Vector(-width, 0);
		_NextPos = Point(width, 0);
	}
	else if (_Direct == Direct::RIGHT)
	{
		_Vector = Vector(width, 0);
		_NextPos = Point(-width, 0);
	}

	if (_pPrevScene) _pPrevScene->getRoot()->setPos(0, 0);
	_pNextScene->getRoot()->setPos(_NextPos);
}

void e2d::TransitionMove::_updateCustom()
{
	if (_pPrevScene)
	{
		_pPrevScene->getRoot()->setPos(_Vector * _fRateOfProgress);
	}
	if (_pNextScene)
	{
		_pNextScene->getRoot()->setPos(_NextPos + _Vector * _fRateOfProgress);
	}

	if (_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionMove::_reset()
{
	if (_pPrevScene) _pPrevScene->getRoot()->setPos(0, 0);
	_pNextScene->getRoot()->setPos(0, 0);
}

