#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::ActionBase::ActionBase() 
	: _bRunning(false)
	, _bEnding(false)
	, _bInit(false)
	, _pTarget(nullptr)
	, _pParentScene(nullptr)
	, _fLast(0)
{
	ActionManager::__add(this);
}

e2d::ActionBase::~ActionBase()
{
}

bool e2d::ActionBase::_isDone()
{
	return _bEnding;
}

void e2d::ActionBase::_startWithTarget(Node* target)
{
	_pTarget = target;
	_bRunning = true;
	this->reset();
}

bool e2d::ActionBase::isRunning()
{
	return _bRunning;
}

void e2d::ActionBase::resume()
{
	_bRunning = true;
	_fLast = Time::getTotalTime();
}

void e2d::ActionBase::pause()
{
	_bRunning = false;
}

void e2d::ActionBase::stop()
{
	_bEnding = true;
}

e2d::String e2d::ActionBase::getName() const
{
	return _sName;
}

void e2d::ActionBase::setName(const String& name)
{
	_sName = name;
}

e2d::ActionBase * e2d::ActionBase::reverse() const
{
	WARN_IF(true, "ActionBase cannot be reversed!");
	return nullptr;
}

e2d::Node * e2d::ActionBase::getTarget()
{
	return _pTarget;
}

void e2d::ActionBase::destroy()
{
	ActionManager::__remove(this);
}

void e2d::ActionBase::_init()
{
	_bInit = true;
	// 记录当前时间
	_fLast = Time::getTotalTime();
}

void e2d::ActionBase::_update()
{
	if (!_bInit)
	{
		_init();
	}
}

void e2d::ActionBase::reset()
{
	_bInit = false;
	_bEnding = false;
	_fLast = Time::getTotalTime();
}

void e2d::ActionBase::_resetTime()
{
	_fLast = Time::getTotalTime();
}
