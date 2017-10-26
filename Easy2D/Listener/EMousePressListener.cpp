#include "..\elisteners.h"

e2d::EMousePressListener::EMousePressListener()
	: EMouseListener()
{
}

e2d::EMousePressListener::EMousePressListener(const EString & name)
	: EMouseListener(name)
{
}

e2d::EMousePressListener::EMousePressListener(const MOUSE_PRESS_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_Callback(callback)
{
}

e2d::EMousePressListener::EMousePressListener(const EString & name, const MOUSE_PRESS_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_Callback(callback)
{
}

void e2d::EMousePressListener::_callOn()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_Callback(EMouseMsg::getPos());
	}
}

void e2d::EMousePressListener::setCallback(const MOUSE_PRESS_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
