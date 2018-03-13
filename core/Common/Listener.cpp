#include "..\ecommon.h"
#include "..\ebase.h"

e2d::Listener::Listener()
	: m_bRunning(false)
	, m_callback(nullptr)
	, m_bClear(false)
{
	Input::__add(this);
}

e2d::Listener::Listener(ListenerCallback callback, const String & name)
	: m_bRunning(false)
	, m_sName(name)
	, m_callback(callback)
	, m_bClear(false)
{
	Input::__add(this);
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

void e2d::Listener::setName(const String & name)
{
	m_sName = name;
}

void e2d::Listener::setCallback(ListenerCallback callback)
{
	m_callback = callback;
}

void e2d::Listener::update()
{
	if (m_callback)
	{
		m_callback();
	}
}
