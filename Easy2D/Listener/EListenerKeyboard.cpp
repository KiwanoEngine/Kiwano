#include "..\elisteners.h"
#include "..\emanagers.h"

e2d::EListenerKeyboard::EListenerKeyboard()
	: EListener()
	, m_Callback(nullptr)
{
}

e2d::EListenerKeyboard::EListenerKeyboard(const EString & name)
	: EListener(name)
	, m_Callback(nullptr)
{
}

e2d::EListenerKeyboard::EListenerKeyboard(const KEY_LISTENER_CALLBACK & callback)
	: EListener()
	, m_Callback(callback)
{
}

e2d::EListenerKeyboard::EListenerKeyboard(const EString & name, const KEY_LISTENER_CALLBACK & callback)
	: EListener(name)
	, m_Callback(callback)
{
}

void e2d::EListenerKeyboard::_callOn()
{
	if (m_Callback)
	{
		m_Callback();
	}
}

void e2d::EListenerKeyboard::setCallback(const KEY_LISTENER_CALLBACK & callback)
{
	m_Callback = callback;
}

void e2d::EListenerKeyboard::bindWith(EScene * pParentScene)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentScene)
	{
		EMsgManager::bindListener(this, pParentScene);
	}
}

void e2d::EListenerKeyboard::bindWith(ENode * pParentNode)
{
	WARN_IF(m_pParentNode != nullptr, "A listener cannot bind with two object.");

	if (pParentNode)
	{
		EMsgManager::bindListener(this, pParentNode);
	}
}