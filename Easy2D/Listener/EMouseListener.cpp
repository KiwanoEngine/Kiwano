#include "..\emsg.h"

e2d::EMouseListener::EMouseListener()
	: EListener()
{
}

e2d::EMouseListener::EMouseListener(const EString & name)
	: EListener(name)
{
}

e2d::EMouseListener::EMouseListener(const MOUSE_LISTENER_CALLBACK & callback)
	: EListener()
{
	m_Callback = callback;
}

e2d::EMouseListener::EMouseListener(const EString & name, const MOUSE_LISTENER_CALLBACK & callback)
	: EListener(name)
{
	m_Callback = callback;
}

void e2d::EMouseListener::_callOn()
{
	m_Callback();
}

void e2d::EMouseListener::setCallback(const MOUSE_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}

void e2d::EMouseListener::bindWith(EScene * pParentScene)
{
	WARN_IF(m_pParentScene != nullptr || m_pParentNode != nullptr, "EListener cannot bind with two object.");

	if (pParentScene)
	{
		EMsgManager::bindListener(this, pParentScene);
	}
}

void e2d::EMouseListener::bindWith(ENode * pParentNode)
{
	WARN_IF(m_pParentScene != nullptr || m_pParentNode != nullptr, "EListener cannot bind with two object.");

	if (pParentNode != nullptr && m_pParentScene == nullptr)
	{
		EMsgManager::bindListener(this, pParentNode);
	}
}
