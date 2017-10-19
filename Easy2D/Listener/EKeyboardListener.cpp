#include "..\emsg.h"

e2d::EKeyboardListener::EKeyboardListener()
	: EListener()
{
}

e2d::EKeyboardListener::EKeyboardListener(const EString & name)
	: EListener(name)
{
}

e2d::EKeyboardListener::EKeyboardListener(const KEY_LISTENER_CALLBACK & callback)
	: EListener()
{
	m_Callback = callback;
}

e2d::EKeyboardListener::EKeyboardListener(const EString & name, const KEY_LISTENER_CALLBACK & callback)
	: EListener(name)
{
	m_Callback = callback;
}

void e2d::EKeyboardListener::_callOn()
{
	m_Callback();
}

void e2d::EKeyboardListener::setCallback(const KEY_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}

void e2d::EKeyboardListener::bindWith(EScene * pParentScene)
{
	WARN_IF(m_pParentScene != nullptr || m_pParentNode != nullptr, "EListener cannot bind with two object.");

	if (pParentScene)
	{
		EMsgManager::bindListener(this, pParentScene);
	}
}

void e2d::EKeyboardListener::bindWith(ENode * pParentNode)
{
	WARN_IF(m_pParentScene != nullptr || m_pParentNode != nullptr, "EListener cannot bind with two object.");

	if (pParentNode != nullptr && m_pParentScene == nullptr)
	{
		EMsgManager::bindListener(this, pParentNode);
	}
}