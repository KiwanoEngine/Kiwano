#include "..\..\e2dtool.h"

e2d::Listener::Listener()
	: m_bRunning(false)
	, m_bClear(false)
{
}

e2d::Listener::Listener(String name)
	: m_bRunning(false)
	, m_sName(name)
	, m_bClear(false)
{
}

void e2d::Listener::start()
{
	m_bRunning = true;
}

void e2d::Listener::stop()
{
	m_bRunning = false;
}

void e2d::Listener::stopAndClear()
{
	m_bRunning = false;
	m_bClear = true;
}

bool e2d::Listener::isRunning()
{
	return m_bRunning;
}

e2d::String e2d::Listener::getName()
{
	return m_sName;
}

void e2d::Listener::setName(String name)
{
	m_sName = name;
}
