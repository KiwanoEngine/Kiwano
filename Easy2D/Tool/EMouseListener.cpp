#include "..\etools.h"

e2d::EMouseListener::EMouseListener()
	: m_bRunning(false)
	, m_bWaiting(false)
	, m_sName(L"")
	, m_callback([] {})
	, m_pParentScene(nullptr)
{
}

e2d::EMouseListener::EMouseListener(EString name)
	: EMouseListener()
{
	m_sName = name;
}

e2d::EMouseListener::EMouseListener(const MOUSE_CALLBACK & callback)
	: EMouseListener()
{
	m_callback = callback;
}

e2d::EMouseListener::EMouseListener(EString name, const MOUSE_CALLBACK & callback)
	: EMouseListener()
{
	m_sName = name;
	m_callback = callback;
}

bool e2d::EMouseListener::isRunnint() const
{
	return m_bRunning && !m_bWaiting;
}

void e2d::EMouseListener::start()
{
	m_bRunning = true;
}

void e2d::EMouseListener::stop()
{
	m_bRunning = false;
}

void e2d::EMouseListener::wait()
{
	m_bWaiting = true;
}

void e2d::EMouseListener::notify()
{
	m_bWaiting = false;
}

void e2d::EMouseListener::runCallback()
{
	m_callback();
}

e2d::EString e2d::EMouseListener::getName() const
{
	return m_sName;
}

e2d::EScene * e2d::EMouseListener::getParentScene()
{
	return m_pParentScene;
}