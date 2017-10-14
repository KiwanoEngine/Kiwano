#include "..\..\emsg.h"

e2d::EKeyboardListener::EKeyboardListener()
	: EListener()
{
}

e2d::EKeyboardListener::EKeyboardListener(EString name)
	: EListener(name)
{
}

e2d::EKeyboardListener::EKeyboardListener(const KEY_LISTENER_CALLBACK & callback)
	: EListener()
{
	m_callback = callback;
}

e2d::EKeyboardListener::EKeyboardListener(EString name, const KEY_LISTENER_CALLBACK & callback)
	: EListener(name)
{
	m_callback = callback;
}

void e2d::EKeyboardListener::runCallback()
{
	m_callback();
}

void e2d::EKeyboardListener::setCallback(const KEY_LISTENER_CALLBACK & callback)
{
	m_callback = callback;
}

void e2d::EKeyboardListener::bindWithScene(EScene * pParentScene)
{
	WARN_IF(m_pParentScene != nullptr || m_pParentNode != nullptr, "EListener cannot bind with two object.");

	if (pParentScene)
	{
		EMsgManager::bindListenerWith(this, pParentScene);
	}
}

void e2d::EKeyboardListener::bindWithNode(ENode * pParentNode)
{
	WARN_IF(m_pParentScene != nullptr || m_pParentNode != nullptr, "EListener cannot bind with two object.");

	if (pParentNode != nullptr && m_pParentScene == nullptr)
	{
		EMsgManager::bindListenerWith(this, pParentNode);
	}
}