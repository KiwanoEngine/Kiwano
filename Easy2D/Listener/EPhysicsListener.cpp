#include "..\elisteners.h"
#include "..\egeometry.h"
#include "..\emanagers.h"

e2d::EPhysicsListener::EPhysicsListener()
	: EListener()
{
}

e2d::EPhysicsListener::EPhysicsListener(const EString & name)
	: EListener(name)
{
}

e2d::EPhysicsListener::EPhysicsListener(const PHYSICS_LISTENER_CALLBACK & callback)
	: EListener()
{
	m_Callback = callback;
}

e2d::EPhysicsListener::EPhysicsListener(const EString & name, const PHYSICS_LISTENER_CALLBACK & callback)
	: EListener(name)
{
	m_Callback = callback;
}

void e2d::EPhysicsListener::_callOn()
{
	m_Callback(
		EPhysicsMsg::getActiveGeometry()->getParentNode(),
		EPhysicsMsg::getPassiveGeometry()->getParentNode(),
		EPhysicsMsg::getMsg()
	);
}

void e2d::EPhysicsListener::setCallback(const PHYSICS_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}

void e2d::EPhysicsListener::bindWith(EScene * pParentScene)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentScene)
	{
		EPhysicsManager::bindListener(this, pParentScene);
	}
}

void e2d::EPhysicsListener::bindWith(ENode * pParentNode)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentNode != nullptr)
	{
		EPhysicsManager::bindListener(this, pParentNode);
	}
}
