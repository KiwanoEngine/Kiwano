#include "..\e2dcommon.h"


e2d::MouseEvent::MouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
	: _pos(LOWORD(lParam), HIWORD(lParam))
	, _shiftDown(GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
	, _ctrlDown(GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL)
	, _wheelDelta(GET_WHEEL_DELTA_WPARAM(wParam))
	, _type(Type(message))
{
}

double e2d::MouseEvent::getX() const
{
	return _pos.x;
}

double e2d::MouseEvent::getY() const
{
	return _pos.y;
}

e2d::Point e2d::MouseEvent::getPos() const
{
	return _pos;
}

bool e2d::MouseEvent::isShiftDown() const
{
	return _shiftDown;
}

bool e2d::MouseEvent::isCtrlDown() const
{
	return _ctrlDown;
}

double e2d::MouseEvent::getWheelDelta() const
{
	return _wheelDelta;
}

e2d::MouseEvent::Type e2d::MouseEvent::getType() const
{
	return _type;
}
