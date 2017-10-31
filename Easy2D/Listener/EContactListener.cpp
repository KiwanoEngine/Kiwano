#include "..\elisteners.h"
#include "..\egeometry.h"

e2d::EContactListener::EContactListener()
	: EPhysicsListener()
{
}

e2d::EContactListener::EContactListener(const EString & name)
	: EPhysicsListener(name)
{
}

e2d::EContactListener::EContactListener(const COLLISION_LISTENER_CALLBACK & callback)
	: EPhysicsListener()
{
	this->m_Callback = callback;
}

e2d::EContactListener::EContactListener(const EString & name, const COLLISION_LISTENER_CALLBACK & callback)
	: EPhysicsListener(name)
{
	this->m_Callback = callback;
}

void e2d::EContactListener::_callOn()
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
