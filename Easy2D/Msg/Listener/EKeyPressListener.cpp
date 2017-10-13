#include "..\..\emsg.h"

e2d::EKeyPressListener::EKeyPressListener()
	: EKeyboardListener()
{
}

e2d::EKeyPressListener::EKeyPressListener(EString name)
	: EKeyboardListener(name)
{
}

e2d::EKeyPressListener::EKeyPressListener(const KEY_LISTENER_CALLBACK & callback)
	: EKeyboardListener(callback)
{
}

e2d::EKeyPressListener::EKeyPressListener(EString name, const KEY_LISTENER_CALLBACK & callback)
	: EKeyboardListener(name, callback)
{
}

void e2d::EKeyPressListener::runCallback()
{
	if (EKeyMsg::getMsg() == EKeyMsg::KEYBOARD_MSG::KEY_DOWN)
	{
		m_callback();
	}
}
