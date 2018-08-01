#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::MoveTransition::MoveTransition(float duration, Direction direction)
	: Transition(duration)
	, _direction(direction)
{
}

bool e2d::MoveTransition::init(Scene * prev, Scene * next)
{
	if (Transition::init(prev, next))
	{
		float width = _windowSize.width;
		float height = _windowSize.height;
		if (_direction == Direction::Up)
		{
			_posDelta = Vector2(0, -height);
			_startPos = Point(0, height);
		}
		else if (_direction == Direction::Down)
		{
			_posDelta = Vector2(0, height);
			_startPos = Point(0, -height);
		}
		else if (_direction == Direction::Left)
		{
			_posDelta = Vector2(-width, 0);
			_startPos = Point(width, 0);
		}
		else if (_direction == Direction::Right)
		{
			_posDelta = Vector2(width, 0);
			_startPos = Point(-width, 0);
		}

		if (_outScene) _outScene->getRoot()->setPos(0, 0);
		_inScene->getRoot()->setPos(_startPos);
		return true;
	}
	return false;
}

void e2d::MoveTransition::update()
{
	Transition::update();

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
		this->stop();
	}
}

void e2d::MoveTransition::reset()
{
	if (_outScene) _outScene->getRoot()->setPos(0, 0);
	_inScene->getRoot()->setPos(0, 0);
}

