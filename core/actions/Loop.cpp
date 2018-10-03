#include "..\e2daction.h"

e2d::Loop::Loop(Action * action, int times /* = -1 */)
	: action_(action)
	, times_(0)
	, total_times_(times)
{
	WARN_IF(action == nullptr, "Loop NULL pointer exception!");

	if (action)
	{
		action_ = action;
		action_->Retain();
	}
}

e2d::Loop::~Loop()
{
	SafeRelease(action_);
}

e2d::Loop * e2d::Loop::Clone() const
{
	if (action_)
	{
		return new Loop(action_->Clone());
	}
	else
	{
		return nullptr;
	}
}

e2d::Loop * e2d::Loop::Reverse() const
{
	if (action_)
	{
		return new Loop(action_->Clone());
	}
	else
	{
		return nullptr;
	}
}

void e2d::Loop::Init()
{
	Action::Init();

	if (action_)
	{
		action_->target_ = target_;
		action_->Init();
	}
}

void e2d::Loop::Update()
{
	Action::Update();

	if (times_ == total_times_)
	{
		this->Stop();
		return;
	}

	if (action_)
	{
		action_->Update();

		if (action_->IsDone())
		{
			++times_;

			Action::Reset();
			action_->Reset();
		}
	}
	else
	{
		this->Stop();
	}
}

void e2d::Loop::Reset()
{
	Action::Reset();

	if (action_) action_->Reset();
	times_ = 0;
}

void e2d::Loop::ResetTime()
{
	if (action_) action_->ResetTime();
}
