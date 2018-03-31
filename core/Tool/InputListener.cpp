#include "..\etools.h"
#include "..\emanagers.h"

e2d::InputListener::InputListener()
	: Listener()
	, m_callback(nullptr)
{
	InputManager::__add(this);
}

e2d::InputListener::InputListener(Function func)
	: Listener()
	, m_callback(func)
{
	InputManager::__add(this);
}

e2d::InputListener::InputListener(Function func, String name)
	: Listener(name)
	, m_callback(func)
{
	InputManager::__add(this);
}

void e2d::InputListener::setFunc(Function func)
{
	m_callback = func;
}

void e2d::InputListener::_update()
{
	if (m_callback)
	{
		m_callback();
	}
}
