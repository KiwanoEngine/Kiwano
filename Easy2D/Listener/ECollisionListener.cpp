#include "..\elisteners.h"
#include "..\egeometry.h"

e2d::ECollisionListener::ECollisionListener()
	: EPhysicsListener()
{
}

e2d::ECollisionListener::ECollisionListener(const EString & name)
	: EPhysicsListener(name)
{
}

e2d::ECollisionListener::ECollisionListener(const COLLISION_LISTENER_CALLBACK & callback)
	: EPhysicsListener()
{
	this->m_Callback = callback;
}

e2d::ECollisionListener::ECollisionListener(const EString & name, const COLLISION_LISTENER_CALLBACK & callback)
	: EPhysicsListener(name)
{
	this->m_Callback = callback;
}

void e2d::ECollisionListener::_callOn()
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
