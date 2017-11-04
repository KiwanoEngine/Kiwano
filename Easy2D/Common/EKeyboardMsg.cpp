#include "..\ecommon.h"

UINT e2d::EKeyboardMsg::s_nMsg = 0;
WPARAM e2d::EKeyboardMsg::s_wParam = 0;
LPARAM e2d::EKeyboardMsg::s_lParam = 0;

e2d::EKeyboardMsg::KEYBOARD_MSG e2d::EKeyboardMsg::getMsg()
{
	return KEYBOARD_MSG(EKeyboardMsg::s_nMsg);
}

e2d::EKeyboardMsg::KEY e2d::EKeyboardMsg::getKeyValue()
{
	return KEY(EKeyboardMsg::s_wParam);
}

DWORD e2d::EKeyboardMsg::getCount()
{
	return (((DWORD)EKeyboardMsg::s_lParam) & 0x0000FFFF);
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
