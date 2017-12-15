#include "..\elisteners.h"

e2d::EListenerMouseClick::EListenerMouseClick()
	: EListenerMouse()
	, m_bPressed(false)
	, m_Callback(nullptr)
{
}

e2d::EListenerMouseClick::EListenerMouseClick(const EString & name)
	: EListenerMouse(name)
	, m_bPressed(false)
	, m_Callback(nullptr)
{
}

e2d::EListenerMouseClick::EListenerMouseClick(const MOUSE_CLICK_LISTENER_CALLBACK & callback)
	: EListenerMouse()
	, m_Callback(callback)
	, m_bPressed(false)
{
}

e2d::EListenerMouseClick::EListenerMouseClick(const EString & name, const MOUSE_CLICK_LISTENER_CALLBACK & callback)
	: EListenerMouse(name)
	, m_Callback(callback)
	, m_bPressed(false)
{
}

void e2d::EListenerMouseClick::_callOn()
{
	if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN ||
		EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DBLCLK)
	{
		m_bPressed = true;
	}
	else if (m_bPressed && EMouseMsg::getMsg() == EMouseMsg::LBUTTON_UP)
	{
		if (m_Callback)
		{
			m_Callback(EMouseMsg::getPos());
		}
		m_bPressed = false;
	}
}

void e2d::EListenerMouseClick::setCallback(const MOUSE_CLICK_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
