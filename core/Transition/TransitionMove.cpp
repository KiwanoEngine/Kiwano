#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionMove::TransitionMove(double duration, Direction direction)
	: Transition(duration)
	, _direction(direction)
{
}

void e2d::TransitionMove::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);

	double width = _windowSize.width;
	double height = _windowSize.height;
	if (_direction == Direction::UP)
	{
		_posDelta = Vector(0, -height);
		_startPos = Point(0, height);
	}
	else if (_direction == Direction::DOWN)
	{
		_posDelta = Vector(0, height);
		_startPos = Point(0, -height);
	}
	else if (_direction == Direction::LEFT)
	{
		_posDelta = Vector(-width, 0);
		_startPos = Point(width, 0);
	}
	else if (_direction == Direction::RIGHT)
	{
		_posDelta = Vector(width, 0);
		_startPos = Point(-width, 0);
	}

	if (_outScene) _outScene->getRoot()->setPos(0, 0);
	_inScene->getRoot()->setPos(_startPos);
}

void e2d::TransitionMove::_updateCustom()
{
	if (_outScene)
	{
		_outScene->getRoot()->setPos(_posDelta * _delta);
	}
	if (_inScene)
	{
		_inScene->getRoot()->setPos(_startPos + _posDelta * _delta);
	}

	if (_delta >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionMove::_reset()
{
	if (_outScene) _outScene->getRoot()->setPos(0, 0);
	_inScene->getRoot()->setPos(0, 0);
}

