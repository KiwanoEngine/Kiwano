#include "..\elisteners.h"

e2d::EListenerMouseDrag::EListenerMouseDrag()
	: EListenerMouse()
{
}

e2d::EListenerMouseDrag::EListenerMouseDrag(const EString & name)
	: EListenerMouse(name)
{
}

e2d::EListenerMouseDrag::EListenerMouseDrag(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EListenerMouse()
	, m_Callback(callback)
{
}

e2d::EListenerMouseDrag::EListenerMouseDrag(const EString & name, const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EListenerMouse(name)
	, m_Callback(callback)
{
}

void e2d::EListenerMouseDrag::_callOn()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_Begin = EMouseMsg::getPos();
	}
	else if (EMouseMsg::isLButtonDown() && EMouseMsg::getMsg() == EMouseMsg::MOVE)
	{
		m_Callback(m_Begin, EMouseMsg::getPos());
	}
}

void e2d::EListenerMouseDrag::setCallback(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
