#include "..\elisteners.h"

e2d::EListenerMouseDoubleClick::EListenerMouseDoubleClick()
	: EListenerMouse()
	, m_bPressed(false)
	, m_Callback(nullptr)
{
}

e2d::EListenerMouseDoubleClick::EListenerMouseDoubleClick(const EString & name)
	: EListenerMouse(name)
	, m_bPressed(false)
	, m_Callback(nullptr)
{
}

e2d::EListenerMouseDoubleClick::EListenerMouseDoubleClick(const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
	: EListenerMouse()
	, m_Callback(callback)
	, m_bPressed(false)
{
}

e2d::EListenerMouseDoubleClick::EListenerMouseDoubleClick(const EString & name, const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
	: EListenerMouse(name)
	, m_Callback(callback)
	, m_bPressed(false)
{
}

void e2d::EListenerMouseDoubleClick::_callOn()
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
		if (m_Callback)
		{
			m_Callback(EMouseMsg::getPos());
		}
		m_bPressed = false;
	}
}

void e2d::EListenerMouseDoubleClick::setCallback(const MOUSE_DBLCLK_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}
