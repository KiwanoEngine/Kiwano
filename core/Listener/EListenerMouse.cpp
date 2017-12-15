#include "..\elisteners.h"
#include "..\emanagers.h"

e2d::EListenerMouse::EListenerMouse()
	: EListener()
	, m_Callback(nullptr)
{
}

e2d::EListenerMouse::EListenerMouse(const EString & name)
	: EListener(name)
	, m_Callback(nullptr)
{
}

e2d::EListenerMouse::EListenerMouse(const MOUSE_LISTENER_CALLBACK & callback)
	: EListener()
	, m_Callback(callback)
{
}

e2d::EListenerMouse::EListenerMouse(const EString & name, const MOUSE_LISTENER_CALLBACK & callback)
	: EListener(name)
	, m_Callback(callback)
{
}

void e2d::EListenerMouse::_callOn()
{
	if (m_Callback)
	{
		m_Callback();
	}
}

void e2d::EListenerMouse::setCallback(const MOUSE_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}

void e2d::EListenerMouse::bindWith(EScene * pParentScene)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentScene)
	{
		EMsgManager::bindListener(this, pParentScene);
	}
}

void e2d::EListenerMouse::bindWith(ENode * pParentNode)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentNode != nullptr)
	{
		EMsgManager::bindListener(this, pParentNode);
	}
}
