#include "..\elisteners.h"
#include "..\egeometry.h"

e2d::EListenerPhysicsContact::EListenerPhysicsContact()
	: EListenerPhysics()
{
}

e2d::EListenerPhysicsContact::EListenerPhysicsContact(const EString & name)
	: EListenerPhysics(name)
{
}

e2d::EListenerPhysicsContact::EListenerPhysicsContact(const COLLISION_LISTENER_CALLBACK & callback)
	: EListenerPhysics()
{
	this->m_Callback = callback;
}

e2d::EListenerPhysicsContact::EListenerPhysicsContact(const EString & name, const COLLISION_LISTENER_CALLBACK & callback)
	: EListenerPhysics(name)
{
	this->m_Callback = callback;
}

void e2d::EListenerPhysicsContact::_callOn()
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
