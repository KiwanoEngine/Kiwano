#include "..\emsg.h"

e2d::EMouseClickListener::EMouseClickListener()
	: EMouseListener()
	, m_bPressed(false)
{
}

e2d::EMouseClickListener::EMouseClickListener(const EString & name)
	: EMouseListener(name)
	, m_bPressed(false)
{
}

e2d::EMouseClickListener::EMouseClickListener(const MOUSE_CLICK_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_Callback(callback)
	, m_bPressed(false)
{
}

e2d::EMouseClickListener::EMouseClickListener(const EString & name, const MOUSE_CLICK_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_Callback(callback)
	, m_bPressed(false)
{
}

void e2d::EMouseClickListener::_callOn()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_bPressed = true;
	}
	else if (m_bPressed && EMouseMsg::getMsg() == EMouseMsg::LBUTTON_UP)
	{
		m_Callback(EMouseMsg::getPos());
		m_bPressed = false;
	}
}

void e2d::EMouseClickListener::setCallback(const MOUSE_CLICK_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
