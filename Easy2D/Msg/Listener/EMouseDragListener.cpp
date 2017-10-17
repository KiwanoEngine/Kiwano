#include "..\..\emsg.h"

e2d::EMouseDragListener::EMouseDragListener()
	: EMouseListener()
{
}

e2d::EMouseDragListener::EMouseDragListener(const EString & name)
	: EMouseListener(name)
{
}

e2d::EMouseDragListener::EMouseDragListener(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_Callback(callback)
{
}

e2d::EMouseDragListener::EMouseDragListener(const EString & name, const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_Callback(callback)
{
}

void e2d::EMouseDragListener::_runCallback()
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

void e2d::EMouseDragListener::setCallback(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
