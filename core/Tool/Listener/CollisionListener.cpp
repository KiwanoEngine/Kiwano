#include "..\..\etool.h"
#include "..\..\emanager.h"

e2d::CollisionListener::CollisionListener()
	: Listener()
{
	ColliderManager::__add(this);
}

e2d::CollisionListener::CollisionListener(Function func)
	: Listener()
	, m_callback(func)
{
	ColliderManager::__add(this);
}

e2d::CollisionListener::CollisionListener(Function func, String name)
	: Listener(name)
	, m_callback(func)
{
	ColliderManager::__add(this);
}

void e2d::CollisionListener::_update()
{
	if (m_callback)
	{
		m_callback();
	}
}
