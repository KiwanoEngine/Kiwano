#include "..\..\emsg.h"

e2d::EMouseDraggedListener::EMouseDraggedListener()
	: EMouseListener()
{
}

e2d::EMouseDraggedListener::EMouseDraggedListener(EString name)
	: EMouseListener(name)
{
}

e2d::EMouseDraggedListener::EMouseDraggedListener(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_callback(callback)
{
}

e2d::EMouseDraggedListener::EMouseDraggedListener(EString name, const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_callback(callback)
{
}

void e2d::EMouseDraggedListener::runCallback()
{
	if (EMouseMsg::getMsg() == EMouseMsg::MOUSE_MSG::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::MOUSE_MSG::LBUTTON_DBLCLK)
	{
		m_Begin = EMouseMsg::getPos();
	}
	else if (EMouseMsg::getMsg() == EMouseMsg::MOUSE_MSG::LBUTTON_UP)
	{
		m_End = EMouseMsg::getPos();
		m_callback(m_Begin, m_End);
	}
}

void e2d::EMouseDraggedListener::setCallback(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
