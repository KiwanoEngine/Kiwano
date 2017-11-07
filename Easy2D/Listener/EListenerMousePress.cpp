#include "..\elisteners.h"

e2d::EListenerMousePress::EListenerMousePress()
	: EListenerMouse()
	, m_Callback(nullptr)
{
}

e2d::EListenerMousePress::EListenerMousePress(const EString & name)
	: EListenerMouse(name)
	, m_Callback(nullptr)
{
}

e2d::EListenerMousePress::EListenerMousePress(const MOUSE_PRESS_LISTENER_CALLBACK & callback)
	: EListenerMouse()
	, m_Callback(callback)
{
}

e2d::EListenerMousePress::EListenerMousePress(const EString & name, const MOUSE_PRESS_LISTENER_CALLBACK & callback)
	: EListenerMouse(name)
	, m_Callback(callback)
{
}

void e2d::EListenerMousePress::_callOn()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		if (m_Callback)
		{
			m_Callback(EMouseMsg::getPos());
		}
	}
}

void e2d::EListenerMousePress::setCallback(const MOUSE_PRESS_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
