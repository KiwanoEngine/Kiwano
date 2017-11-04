#include "..\elisteners.h"

e2d::EListener::EListener()
	: m_bRunning(false)
	, m_bAlways(false)
	, m_pParentNode(nullptr)
{
}

e2d::EListener::EListener(const EString & name)
	: EListener()
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

void e2d::EListener::setAlwaysWorking(bool bAlways)
{
	m_bAlways = bAlways;
}
