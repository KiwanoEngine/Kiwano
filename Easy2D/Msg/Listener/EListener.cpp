#include "..\..\emsg.h"

e2d::EListener::EListener()
	: m_bRunning(false)
	, m_bWaiting(false)
	, m_sName(L"")
	, m_pParentScene(nullptr)
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

e2d::EString e2d::EListener::getName() const
{
	return m_sName;
}

e2d::EScene * e2d::EListener::getParentScene() const
{
	return m_pParentScene;
}

void e2d::EListener::setName(EString name)
{
	m_sName = name;
}

void e2d::EListener::setParentScene(EScene * scene)
{
	if (scene != nullptr) m_pParentScene = scene;
}
