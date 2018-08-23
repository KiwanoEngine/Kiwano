#include "..\e2daction.h"
#include "..\e2dnode.h"

e2d::JumpBy::JumpBy(float duration, const Vector2 & vec, float height, int jumps)
	: FiniteTimeAction(duration)
	, _deltaPos(vec)
	, _height(height)
	, _jumps(jumps)
{
}

e2d::JumpBy * e2d::JumpBy::clone() const
{
	return new (e2d::autorelease) JumpBy(_duration, _deltaPos, _height, _jumps);
}

e2d::JumpBy * e2d::JumpBy::reverse() const
{
	return new (e2d::autorelease) JumpBy(_duration, -_deltaPos, _height, _jumps);
}

void e2d::JumpBy::_init()
{
	FiniteTimeAction::_init();

	if (_target)
	{
		_prevPos = _startPos = _target->getPos();
	}
}

void e2d::JumpBy::_update()
{
	FiniteTimeAction::_update();

	if (_target)
	{
		float frac = fmod(_delta * _jumps, 1.f);
		float x = _deltaPos.x * _delta;
		float y = _height * 4 * frac * (1 - frac);
		y += _deltaPos.y * _delta;

		Point currentPos = _target->getPos();

		Vector2 diff = currentPos - _prevPos;
		_startPos = diff + _startPos;

		Point newPos = _startPos + Vector2(x, y);
		_target->setPos(newPos);

		_prevPos = newPos;
	}
}
