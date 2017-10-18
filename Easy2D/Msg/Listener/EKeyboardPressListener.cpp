#include "..\..\emsg.h"

e2d::EKeyboardPressListener::EKeyboardPressListener()
	: EKeyboardListener()
{
}

e2d::EKeyboardPressListener::EKeyboardPressListener(const EString & name)
	: EKeyboardListener(name)
{
}

e2d::EKeyboardPressListener::EKeyboardPressListener(const KEY_LISTENER_CALLBACK & callback)
	: EKeyboardListener(callback)
{
}

e2d::EKeyboardPressListener::EKeyboardPressListener(const EString & name, const KEY_LISTENER_CALLBACK & callback)
	: EKeyboardListener(name, callback)
{
}

void e2d::EKeyboardPressListener::_callOn()
{
	if (EKeyboardMsg::getMsg() == EKeyboardMsg::KEYBOARD_MSG::KEY_DOWN)
	{
		m_Callback();
	}
}
