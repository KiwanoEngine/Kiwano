#include "..\eactions.h"
#include "..\emanagers.h"

e2d::Action::Action() 
	: m_bRunning(false)
	, m_bEnding(false)
	, m_bInit(false)
	, m_pTarget(nullptr)
	, m_pParentScene(nullptr)
	, m_fLast(0)
{
	ActionManager::__add(this);
}

e2d::Action::~Action()
{
	ActionManager::__remove(this);
}

bool e2d::Action::isRunning()
{
	return m_bRunning;
}

bool e2d::Action::_isEnding()
{
	return m_bEnding;
}

void e2d::Action::setTarget(Node* pTarget)
{
	if (pTarget)
	{
		m_bRunning = true;
		m_pTarget = pTarget;
		this->reset();
	}
}

void e2d::Action::resume()
{
	m_bRunning = true;
	m_fLast = Time::getTotalTime();
}

void e2d::Action::pause()
{
	m_bRunning = false;
}

void e2d::Action::stop()
{
	m_bEnding = true;
}

e2d::String e2d::Action::getName() const
{
	return m_sName;
}

void e2d::Action::setName(const String & name)
{
	m_sName = name;
}

e2d::Action * e2d::Action::reverse() const
{
	ASSERT(false, "Action cannot be reversed!");
	return nullptr;
}

e2d::Node * e2d::Action::getTarget()
{
	return m_pTarget;
}

void e2d::Action::_init()
{
	m_bInit = true;
	// 记录当前时间
	m_fLast = Time::getTotalTime();
}

void e2d::Action::_update()
{
	if (!m_bInit)
	{
		_init();
	}
}

void e2d::Action::reset()
{
	m_bInit = false;
	m_bEnding = false;
	m_fLast = Time::getTotalTime();
}

void e2d::Action::_resetTime()
{
	m_fLast = Time::getTotalTime();
}
