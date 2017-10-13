#include "..\..\emsg.h"

e2d::EKeyPressListener::EKeyPressListener()
	: EKeyListener()
{
}

e2d::EKeyPressListener::EKeyPressListener(EString name)
	: EKeyListener(name)
{
}

e2d::EKeyPressListener::EKeyPressListener(const KEY_LISTENER_CALLBACK & callback)
	: EKeyListener(callback)
{
}

e2d::EKeyPressListener::EKeyPressListener(EString name, const KEY_LISTENER_CALLBACK & callback)
	: EKeyListener(name, callback)
{
}

void e2d::EKeyPressListener::runCallback()
{
	if (EKeyMsg::getMsg() == EKeyMsg::KEYBOARD_MSG::KEY_DOWN)
	{
		m_callback();
	}
}
