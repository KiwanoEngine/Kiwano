#include "..\..\emsg.h"

e2d::EKeyListener::EKeyListener()
	: EListener()
{
}

e2d::EKeyListener::EKeyListener(EString name)
	: EListener(name)
{
}

e2d::EKeyListener::EKeyListener(const KEY_LISTENER_CALLBACK & callback)
	: EListener()
{
	m_callback = callback;
}

e2d::EKeyListener::EKeyListener(EString name, const KEY_LISTENER_CALLBACK & callback)
	: EListener(name)
{
	m_callback = callback;
}

void e2d::EKeyListener::runCallback()
{
	m_callback();
}

void e2d::EKeyListener::setCallback(const KEY_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
