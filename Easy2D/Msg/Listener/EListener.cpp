#include "..\..\emsg.h"

e2d::EListener::EListener()
	: m_bRunning(false)
	, m_bWaiting(false)
	, m_pParentScene(nullptr)
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

void e2d::EListener::_wait()
{
	m_bWaiting = true;
}

void e2d::EListener::_notify()
{
	m_bWaiting = false;
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

void e2d::EListener::setName(const EString & name)
{
	m_sName = name;
}
