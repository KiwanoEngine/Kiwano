#include "..\elisteners.h"

e2d::EListenerKeyboardPress::EListenerKeyboardPress()
	: EListenerKeyboard()
{
}

e2d::EListenerKeyboardPress::EListenerKeyboardPress(const EString & name)
	: EListenerKeyboard(name)
{
}

e2d::EListenerKeyboardPress::EListenerKeyboardPress(const KEY_LISTENER_CALLBACK & callback)
	: EListenerKeyboard(callback)
{
}

e2d::EListenerKeyboardPress::EListenerKeyboardPress(const EString & name, const KEY_LISTENER_CALLBACK & callback)
	: EListenerKeyboard(name, callback)
{
}

void e2d::EListenerKeyboardPress::_callOn()
{
	if (EKeyboardMsg::getMsg() == EKeyboardMsg::KEYBOARD_MSG::KEY_DOWN)
	{
		m_Callback();
	}
}
