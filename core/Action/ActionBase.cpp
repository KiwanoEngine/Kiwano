#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::ActionBase::ActionBase() 
	: m_bRunning(false)
	, m_bEnding(false)
	, m_bInit(false)
	, m_pTarget(nullptr)
	, m_pParentScene(nullptr)
	, m_fLast(0)
{
	ActionManager::__add(this);
}

e2d::ActionBase::~ActionBase()
{
}

bool e2d::ActionBase::isRunning()
{
	return m_bRunning;
}

bool e2d::ActionBase::_isEnding()
{
	return m_bEnding;
}

void e2d::ActionBase::setTarget(Node* pTarget)
{
	if (pTarget)
	{
		m_bRunning = true;
		m_pTarget = pTarget;
		this->reset();
	}
}

void e2d::ActionBase::resume()
{
	m_bRunning = true;
	m_fLast = Time::getTotalTime();
}

void e2d::ActionBase::pause()
{
	m_bRunning = false;
}

void e2d::ActionBase::stop()
{
	m_bEnding = true;
}

e2d::String e2d::ActionBase::getName() const
{
	return m_sName;
}

void e2d::ActionBase::setName(const String& name)
{
	m_sName = name;
}

e2d::ActionBase * e2d::ActionBase::reverse() const
{
	WARN_IF(true, "ActionBase cannot be reversed!");
	return nullptr;
}

e2d::Node * e2d::ActionBase::getTarget()
{
	return m_pTarget;
}

void e2d::ActionBase::destroy()
{
	ActionManager::__remove(this);
}

void e2d::ActionBase::_init()
{
	m_bInit = true;
	// 记录当前时间
	m_fLast = Time::getTotalTime();
}

void e2d::ActionBase::_update()
{
	if (!m_bInit)
	{
		_init();
	}
}

void e2d::ActionBase::reset()
{
	m_bInit = false;
	m_bEnding = false;
	m_fLast = Time::getTotalTime();
}

void e2d::ActionBase::_resetTime()
{
	m_fLast = Time::getTotalTime();
}
