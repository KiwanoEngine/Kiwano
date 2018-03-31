#include "..\etools.h"
#include "..\emanagers.h"

e2d::CollisionListener::CollisionListener()
	: Listener()
{
	CollisionManager::__add(this);
}

e2d::CollisionListener::CollisionListener(Function func)
	: Listener()
	, m_callback(func)
{
	CollisionManager::__add(this);
}

e2d::CollisionListener::CollisionListener(Function func, String name)
	: Listener(name)
	, m_callback(func)
{
	CollisionManager::__add(this);
}

void e2d::CollisionListener::_update()
{
	if (m_callback)
	{
		m_callback();
	}
}
