#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::Action::Action() 
	: _running(false)
	, _done(false)
	, _initialized(false)
	, _target(nullptr)
	, _last(0)
{
	ActionManager::__add(this);
}

e2d::Action::~Action()
{
}

bool e2d::Action::_isDone()
{
	return _done;
}

void e2d::Action::_startWithTarget(Node* target)
{
	_target = target;
	_running = true;
	this->reset();
}

bool e2d::Action::isRunning()
{
	return _running;
}

void e2d::Action::resume()
{
	_running = true;
	_last = Time::getTotalTime();
}

void e2d::Action::pause()
{
	_running = false;
}

void e2d::Action::stop()
{
	_done = true;
}

e2d::String e2d::Action::getName() const
{
	return _name;
}

void e2d::Action::setName(const String& name)
{
	_name = name;
}

e2d::Action * e2d::Action::reverse() const
{
	WARN_IF(true, "Action cannot be reversed!");
	return nullptr;
}

e2d::Node * e2d::Action::getTarget()
{
	return _target;
}

void e2d::Action::onDestroy()
{
	ActionManager::__remove(this);
}

void e2d::Action::_init()
{
	_initialized = true;
	// 记录当前时间
	_last = Time::getTotalTime();
}

void e2d::Action::_update()
{
	if (!_initialized)
	{
		_init();
	}
}

void e2d::Action::reset()
{
	_initialized = false;
	_done = false;
	_last = Time::getTotalTime();
}

void e2d::Action::_resetTime()
{
	_last = Time::getTotalTime();
}
