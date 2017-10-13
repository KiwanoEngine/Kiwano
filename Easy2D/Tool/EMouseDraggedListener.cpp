#include "..\etools.h"

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
{
}

e2d::EMouseDraggedListener::EMouseDraggedListener(EString name, const MOUSE_DRAG_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
{
}

void e2d::EMouseDraggedListener::runCallback()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		EMouseListener::runCallback();
	}
}

void e2d::EMouseDraggedListener::setCallback(const MOUSE_DRAG_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
