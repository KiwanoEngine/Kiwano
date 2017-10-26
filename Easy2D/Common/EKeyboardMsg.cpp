#include "..\ecommon.h"

static e2d::EKeyboardMsg s_KeyboardMsg;

e2d::EKeyboardMsg::KEYBOARD_MSG e2d::EKeyboardMsg::getMsg()
{
	return KEYBOARD_MSG(s_KeyboardMsg.m_nMsg);
}

e2d::EKeyboardMsg::KEY e2d::EKeyboardMsg::getVal()
{
	return KEY(s_KeyboardMsg.m_wParam);
}

DWORD e2d::EKeyboardMsg::getCount()
{
	return (((DWORD)s_KeyboardMsg.m_lParam) & 0x0000FFFF);
}

bool e2d::EKeyboardMsg::isKeyDown(KEY key)
{
	if (::GetAsyncKeyState((int)key) & 0x8000)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyboardMsg::isCapitalLockOn()
{
	if (::GetKeyState(VK_CAPITAL) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyboardMsg::isNumpadLockOn()
{
	if (::GetKeyState(VK_NUMLOCK) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyboardMsg::isScrollLockOn()
{
	if (::GetKeyState(VK_SCROLL) & 0x0001)
	{
		return true;
	}
	return false;
}

e2d::EKeyboardMsg & e2d::EKeyboardMsg::getKeyboardMsg()
{
	return s_KeyboardMsg;
}
