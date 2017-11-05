#include "..\elisteners.h"
#include "..\egeometry.h"

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision()
	: EListenerPhysics()
{
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const EString & name)
	: EListenerPhysics(name)
{
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const COLLISION_LISTENER_CALLBACK & callback)
	: EListenerPhysics()
{
	this->m_Callback = callback;
}

e2d::EListenerPhysicsCollision::EListenerPhysicsCollision(const EString & name, const COLLISION_LISTENER_CALLBACK & callback)
	: EListenerPhysics(name)
{
	this->m_Callback = callback;
}

void e2d::EListenerPhysicsCollision::_callOn()
{
	if (EPhysicsMsg::getMsg() == EPhysicsMsg::OVERLAP ||
		EPhysicsMsg::getMsg() == EPhysicsMsg::CONTAINS || 
		EPhysicsMsg::getMsg() == EPhysicsMsg::IS_CONTAINED)
	{
		m_Callback(
			EPhysicsMsg::getActiveGeometry()->getParentNode(),
			EPhysicsMsg::getPassiveGeometry()->getParentNode()
		);
	}
}
