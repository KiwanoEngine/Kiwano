#include "..\elisteners.h"
#include "..\emanagers.h"
#include "..\enodes.h"

e2d::EListener::EListener()
	: m_bRunning(false)
	, m_bAlways(false)
	, m_pParentNode(nullptr)
	, m_bSwallow(false)
{
}

e2d::EListener::EListener(const EString & name)
	: m_bRunning(false)
	, m_bAlways(false)
	, m_pParentNode(nullptr)
	, m_bSwallow(false)
{
	m_sName = name;
}

bool e2d::EListener::isRunning() const
{
	return m_bRunning;
}

void e2d::EListener::start()
{
	m_bRunning = true;
}

void e2d::EListener::stop()
{
	m_bRunning = false;
}

e2d::EString e2d::EListener::getName() const
{
	return m_sName;
}

e2d::ENode * e2d::EListener::getParentNode() const
{
	return m_pParentNode;
}

void e2d::EListener::setName(const EString & name)
{
	m_sName = name;
}

void e2d::EListener::setSwallow(bool bSwallow)
{
	m_bSwallow = bSwallow;
}

void e2d::EListener::setAlwaysWorking(bool bAlways)
{
	m_bAlways = bAlways;
}

bool e2d::EListener::_isReady() const
{
	if (m_bRunning && m_pParentNode)
	{
		if (m_pParentNode->getParentScene() == ESceneManager::getCurrentScene())
		{
			if (!EGame::isPaused() || m_bAlways)
				return true;
		}
	}
	return false;
}
