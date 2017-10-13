#include "..\..\emsg.h"

e2d::EListener::EListener()
	: m_bRunning(false)
	, m_bWaiting(false)
	, m_sName(L"")
	, m_pParentScene(nullptr)
	, m_pParentNode(nullptr)
{
}

e2d::EListener::EListener(EString name)
	: EListener()
{
	m_sName = name;
}

bool e2d::EListener::isRunning() const
{
	return m_bRunning && !m_bWaiting;
}

bool e2d::EListener::isWaiting() const
{
	return m_bWaiting;
}

void e2d::EListener::start()
{
	m_bRunning = true;
}

void e2d::EListener::stop()
{
	m_bRunning = false;
}

void e2d::EListener::wait()
{
	m_bWaiting = true;
}

void e2d::EListener::notify()
{
	m_bWaiting = false;
}

e2d::EListener * e2d::EListener::clone()
{
	return nullptr;
}

e2d::EString e2d::EListener::getName() const
{
	return m_sName;
}

e2d::EScene * e2d::EListener::getParentScene() const
{
	return m_pParentScene;
}

e2d::ENode * e2d::EListener::getParentNode() const
{
	return m_pParentNode;
}

void e2d::EListener::setName(EString name)
{
	m_sName = name;
}

void e2d::EListener::bindWithScene(EScene * scene)
{
	WARN_IF(m_pParentNode != nullptr, "EListener bind with Scene Failed! Please use its clone.");

	if (scene != nullptr && m_pParentNode == nullptr)
	{
		m_pParentScene = scene;
	}
}

void e2d::EListener::bindWithNode(ENode * pParentNode)
{
	WARN_IF(m_pParentScene != nullptr, "EListener bind with Node Failed! Please use its clone.");

	if (pParentNode != nullptr && m_pParentScene == nullptr)
	{
		m_pParentNode = pParentNode;
	}
}
