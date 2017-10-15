#include "..\..\emsg.h"

e2d::EMousePressListener::EMousePressListener()
	: EMouseListener()
{
}

e2d::EMousePressListener::EMousePressListener(EString name)
	: EMouseListener(name)
{
}

e2d::EMousePressListener::EMousePressListener(const MOUSE_PRESS_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_callback(callback)
{
}

e2d::EMousePressListener::EMousePressListener(EString name, const MOUSE_PRESS_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_callback(callback)
{
}

void e2d::EMousePressListener::runCallback()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_callback(EMouseMsg::getPos());
	}
}

void e2d::EMousePressListener::setCallback(const MOUSE_PRESS_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
