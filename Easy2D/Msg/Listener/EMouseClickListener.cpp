#include "..\..\emsg.h"

e2d::EMouseClickListener::EMouseClickListener()
	: EMouseListener()
{
}

e2d::EMouseClickListener::EMouseClickListener(EString name)
	: EMouseListener(name)
{
}

e2d::EMouseClickListener::EMouseClickListener(const MOUSE_CLICK_LISTENER_CALLBACK & callback)
	: EMouseListener()
	, m_callback(callback)
{
}

e2d::EMouseClickListener::EMouseClickListener(EString name, const MOUSE_CLICK_LISTENER_CALLBACK & callback)
	: EMouseListener(name)
	, m_callback(callback)
{
}

void e2d::EMouseClickListener::runCallback()
{
	if (EMouseMsg::getMsg() == EMouseMsg::MOUSE_MSG::LBUTTON_UP)
	{
		m_callback(EMouseMsg::getPos());
	}
}

void e2d::EMouseClickListener::setCallback(const MOUSE_CLICK_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
