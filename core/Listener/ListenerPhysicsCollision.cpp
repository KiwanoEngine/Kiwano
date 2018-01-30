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

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const ClsLsnrCallback & callback)
	: EListenerPhysics()
	, m_Callback(callback)
{
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const EString & name, const ClsLsnrCallback & callback)
	: EListenerPhysics(name)
	, m_Callback(callback)
{
}

void e2d::EListenerPhysicsCollision::_callOn()
{
	if (EPhysicsEvent::getMsg() == EPhysicsEvent::OVERLAP ||
		EPhysicsEvent::getMsg() == EPhysicsEvent::CONTAINS || 
		EPhysicsEvent::getMsg() == EPhysicsEvent::IS_CONTAINED)
	{
		if (m_Callback)
		{
			m_Callback();
		}
	}
}
