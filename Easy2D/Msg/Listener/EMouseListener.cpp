#include "..\..\emsg.h"

e2d::EMouseListener::EMouseListener()
	: EListener()
{
}

e2d::EMouseListener::EMouseListener(EString name)
	: EListener(name)
{
}

e2d::EMouseListener::EMouseListener(const MOUSE_LISTENER_CALLBACK & callback)
	: EListener()
{
	m_callback = callback;
}

e2d::EMouseListener::EMouseListener(EString name, const MOUSE_LISTENER_CALLBACK & callback)
	: EListener(name)
{
	m_callback = callback;
}

void e2d::EMouseListener::runCallback()
{
	m_callback();
}

void e2d::EMouseListener::setCallback(const MOUSE_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}
