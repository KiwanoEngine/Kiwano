#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::MoveTransition::MoveTransition(Scene* scene, float duration, Direction direction)
	: Transition(scene, duration)
	, _direction(direction)
{
}

bool e2d::MoveTransition::_init(Scene * prev)
{
	if (Transition::_init(prev))
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

		if (_outScene) _outScene->setPos(0, 0);
		_inScene->setPos(_startPos);
		return true;
	}
	return false;
}

void e2d::MoveTransition::_update()
{
	Transition::_update();

	if (_outScene)
	{
		_outScene->setPos(_posDelta * _delta);
	}
	if (_inScene)
	{
		_inScene->setPos(_startPos + _posDelta * _delta);
	}

	if (_delta >= 1)
	{
		this->_stop();
	}
}

void e2d::MoveTransition::_reset()
{
	if (_outScene) _outScene->setPos(0, 0);
	_inScene->setPos(0, 0);
}
