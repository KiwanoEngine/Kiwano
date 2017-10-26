#include "..\ecommon.h"

static e2d::EMouseMsg s_MouseMsg;

DWORD e2d::EMouseMsg::getPosX()
{
	return LOWORD(s_MouseMsg.m_lParam);
}

DWORD e2d::EMouseMsg::getPosY()
{
	return HIWORD(s_MouseMsg.m_lParam);
}

e2d::EPoint e2d::EMouseMsg::getPos()
{
	return EPoint(LOWORD(s_MouseMsg.m_lParam), HIWORD(s_MouseMsg.m_lParam));
}

bool e2d::EMouseMsg::isLButtonDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_LBUTTON;
}

bool e2d::EMouseMsg::isMButtonDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_MBUTTON;
}

bool e2d::EMouseMsg::isRButtonDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_RBUTTON;
}

bool e2d::EMouseMsg::isShiftDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_SHIFT;
}

bool e2d::EMouseMsg::isCtrlDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_CONTROL;
}

DWORD e2d::EMouseMsg::getWheelDelta()
{
	return GET_WHEEL_DELTA_WPARAM(s_MouseMsg.m_wParam);
}

e2d::EMouseMsg::MOUSE_MSG e2d::EMouseMsg::getMsg()
{
	return MOUSE_MSG(s_MouseMsg.m_nMsg);
}

e2d::EMouseMsg & e2d::EMouseMsg::getMouseMsg()
{
	return s_MouseMsg;
}
