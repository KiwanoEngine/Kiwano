#include "..\e2daction.h"

e2d::Action::Action() 
	: running_(false)
	, done_(false)
	, initialized_(false)
	, target_(nullptr)
{
}

e2d::Action::~Action()
{
}

bool e2d::Action::IsRunning()
{
	return running_;
}

void e2d::Action::Resume()
{
	running_ = true;
}

void e2d::Action::Pause()
{
	running_ = false;
}

void e2d::Action::Stop()
{
	done_ = true;
}

const e2d::String& e2d::Action::GetName() const
{
	return name_;
}

void e2d::Action::SetName(const String& name)
{
	name_ = name;
}

e2d::Node * e2d::Action::GetTarget()
{
	return target_;
}

void e2d::Action::Reset()
{
	initialized_ = false;
	done_ = false;
	started_ = Time::Now();
}

bool e2d::Action::IsDone() const
{
	return done_;
}

void e2d::Action::StartWithTarget(Node* target)
{
	target_ = target;
	running_ = true;
	this->Reset();
}

void e2d::Action::Init()
{
	initialized_ = true;
	started_ = Time::Now();
}

void e2d::Action::Update()
{
	if (!initialized_)
	{
		Init();
	}
}

void e2d::Action::ResetTime()
{
}
