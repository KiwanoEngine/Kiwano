#include "..\e2daction.h"

e2d::JumpBy::JumpBy(double duration, const Vector & vec, double height, int jumps)
	: ActionGradual(duration)
	, _deltaPos(vec)
	, _height(height)
	, _jumps(jumps)
{
}

e2d::JumpBy * e2d::JumpBy::clone() const
{
	return new (std::nothrow) JumpBy(_duration, _deltaPos, _height, _jumps);
}

e2d::JumpBy * e2d::JumpBy::reverse() const
{
	return new (std::nothrow) JumpBy(_duration, -_deltaPos, _height, _jumps);
}

void e2d::JumpBy::_init()
{
	ActionGradual::_init();

	if (_target)
	{
		_prevPos = _startPos = _target->getPos();
	}
}

void e2d::JumpBy::_update()
{
	ActionGradual::_update();

	if (_target)
	{
		double frac = fmod(_delta * _jumps, 1.0);
		double x = _deltaPos.x * _delta;
		double y = _height * 4 * frac * (1 - frac);
		y += _deltaPos.y * _delta;

		Point currentPos = _target->getPos();

		Vector diff = currentPos - _prevPos;
		_startPos = diff + _startPos;

		Point newPos = _startPos + Vector(x, y);
		_target->setPos(newPos);

		_prevPos = newPos;
	}
}
