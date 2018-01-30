#include "..\elisteners.h"
#include "..\egeometry.h"
#include "..\emanagers.h"

e2d::EListenerPhysics::EListenerPhysics()
	: EListener()
	, m_Callback(nullptr)
{
}

e2d::EListenerPhysics::EListenerPhysics(const EString & name)
	: EListener(name)
	, m_Callback(nullptr)
{
}

e2d::EListenerPhysics::EListenerPhysics(const PhysLsnrCallback & callback)
	: EListener()
	, m_Callback(callback)
{
}

e2d::EListenerPhysics::EListenerPhysics(const EString & name, const PhysLsnrCallback & callback)
	: EListener(name)
	, m_Callback(callback)
{
}

void e2d::EListenerPhysics::_callOn()
{
	if (m_Callback)
	{
		m_Callback();
	}
}

void e2d::EListenerPhysics::setCallback(const PhysLsnrCallback & callback)
{
	m_Callback = callback;
}

void e2d::EListenerPhysics::bindWith(EScene * pParentScene)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentScene)
	{
		EPhysicsManager::bindListener(this, pParentScene);
	}
}

void e2d::EListenerPhysics::bindWith(ENode * pParentNode)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentNode != nullptr)
	{
		EPhysicsManager::bindListener(this, pParentNode);
	}
}
