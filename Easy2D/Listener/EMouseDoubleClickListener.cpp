#include "..\elisteners.h"

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener()
	: EMouseListener()
	, m_bPressed(false)
{
}

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener(const EString & name)
	: EMouseListener(name)
	, m_bPressed(false)
{
}

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener(const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_Callback(callback)
	, m_bPressed(false)
{
}

e2d::EMouseDoubleClickListener::EMouseDoubleClickListener(const EString & name, const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_Callback(callback)
	, m_bPressed(false)
{
}

void e2d::EMouseDoubleClickListener::_callOn()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN)
	{
		m_bPressed = false;
	}
	else if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_bPressed = true;
	}
	else if (m_bPressed && EMouseMsg::getMsg() == EMouseMsg::LBUTTON_UP)
	{
		m_Callback(EMouseMsg::getPos());
		m_bPressed = false;
	}
}

void e2d::EMouseDoubleClickListener::setCallback(const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
