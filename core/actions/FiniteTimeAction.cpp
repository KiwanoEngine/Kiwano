#include "..\e2daction.h"

e2d::FiniteTimeAction::FiniteTimeAction(float duration)
	: delta_(0)
	, duration_(std::max(duration, 0.f))
{
}

void e2d::FiniteTimeAction::Reset()
{
	Action::Reset();
	delta_ = 0;
}

void e2d::FiniteTimeAction::Init()
{
	Action::Init();
}

void e2d::FiniteTimeAction::Update()
{
	Action::Update();

	if (duration_ == 0)
	{
		delta_ = 1;
		this->Stop();
	}
	else
	{
		delta_ = std::min((Time::Now() - started_).Seconds() / duration_, 1.f);

		if (delta_ >= 1)
		{
			this->Stop();
		}
	}
}

void e2d::FiniteTimeAction::ResetTime()
{
	Action::ResetTime();
	started_ = Time::Now() - Duration(delta_ * duration_);
}
