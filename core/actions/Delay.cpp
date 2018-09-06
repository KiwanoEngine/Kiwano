#include "..\e2daction.h"

e2d::Delay::Delay(float duration)
	: delta_(0)
	, delay_(std::max(duration, 0.f))
{
}

e2d::Delay * e2d::Delay::Clone() const
{
	return new Delay(delay_);
}

e2d::Delay * e2d::Delay::Reverse() const
{
	return new Delay(delay_);
}

void e2d::Delay::Reset()
{
	Action::Reset();
	delta_ = 0;
}

void e2d::Delay::Init()
{
	Action::Init();
}

void e2d::Delay::Update()
{
	Action::Update();

	delta_ = (Time::Now() - started_).Seconds();

	if (delta_ >= delay_)
	{
		this->Stop();
	}
}

void e2d::Delay::ResetTime()
{
	Action::ResetTime();
	started_ = Time::Now() - Duration(delta_);
}
