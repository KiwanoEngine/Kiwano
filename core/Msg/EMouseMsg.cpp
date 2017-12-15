#include "..\ecommon.h"

UINT e2d::EMouseMsg::s_nMsg = 0;
WPARAM e2d::EMouseMsg::s_wParam = 0;
LPARAM e2d::EMouseMsg::s_lParam = 0;

DWORD e2d::EMouseMsg::getPosX()
{
	return LOWORD(EMouseMsg::s_lParam);
}

DWORD e2d::EMouseMsg::getPosY()
{
	return HIWORD(EMouseMsg::s_lParam);
}

e2d::EPoint e2d::EMouseMsg::getPos()
{
	return EPoint(LOWORD(EMouseMsg::s_lParam), HIWORD(EMouseMsg::s_lParam));
}

bool e2d::EMouseMsg::isLButtonDown()
{
	return GET_KEYSTATE_WPARAM(EMouseMsg::s_wParam) == MK_LBUTTON;
}

bool e2d::EMouseMsg::isMButtonDown()
{
	return GET_KEYSTATE_WPARAM(EMouseMsg::s_wParam) == MK_MBUTTON;
}

bool e2d::EMouseMsg::isRButtonDown()
{
	return GET_KEYSTATE_WPARAM(EMouseMsg::s_wParam) == MK_RBUTTON;
}

bool e2d::EMouseMsg::isShiftDown()
{
	return GET_KEYSTATE_WPARAM(EMouseMsg::s_wParam) == MK_SHIFT;
}

bool e2d::EMouseMsg::isCtrlDown()
{
	return GET_KEYSTATE_WPARAM(EMouseMsg::s_wParam) == MK_CONTROL;
}

DWORD e2d::EMouseMsg::getWheelDelta()
{
	return GET_WHEEL_DELTA_WPARAM(EMouseMsg::s_wParam);
}

e2d::EMouseMsg::MOUSE_MSG e2d::EMouseMsg::getMsg()
{
	return MOUSE_MSG(EMouseMsg::s_nMsg);
}
