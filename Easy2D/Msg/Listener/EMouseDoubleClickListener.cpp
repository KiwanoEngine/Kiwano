#include "..\..\emsg.h"

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener()
	: EMouseListener()
	, m_bPressed(false)
{
}

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener(EString name)
	: EMouseListener(name)
	, m_bPressed(false)
{
}

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener(const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_callback(callback)
	, m_bPressed(false)
{
}

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener(EString name, const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_callback(callback)
	, m_bPressed(false)
{
}

void e2d::EMouseDoubleClickListener::runCallback()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_bPressed = true;
	}
	else if (m_bPressed && EMouseMsg::getMsg() == EMouseMsg::LBUTTON_UP)
	{
		m_callback(EMouseMsg::getPos());
		m_bPressed = false;
	}
}

void e2d::EMouseDoubleClickListener::setCallback(const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
