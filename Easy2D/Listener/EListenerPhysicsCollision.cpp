#include "..\elisteners.h"
#include "..\egeometry.h"

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision()
	: EListenerPhysics()
	, m_Callback(nullptr)
{
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const EString & name)
	: EListenerPhysics(name)
	, m_Callback(nullptr)
{
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const COLLISION_LISTENER_CALLBACK & callback)
	: EListenerPhysics()
	, m_Callback(callback)
{
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const EString & name, const COLLISION_LISTENER_CALLBACK & callback)
	: EListenerPhysics(name)
	, m_Callback(callback)
{
}

void e2d::EListenerPhysicsCollision::_callOn()
{
	if (EPhysicsMsg::getMsg() == EPhysicsMsg::OVERLAP ||
		EPhysicsMsg::getMsg() == EPhysicsMsg::CONTAINS || 
		EPhysicsMsg::getMsg() == EPhysicsMsg::IS_CONTAINED)
	{
		if (m_Callback)
		{
			m_Callback();
		}
	}
}
