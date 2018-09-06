#include "..\e2daction.h"
#include "..\e2dobject.h"

e2d::JumpBy::JumpBy(float duration, const Point & vec, float height, int jumps)
	: FiniteTimeAction(duration)
	, delta_pos_(vec)
	, height_(height)
	, jumps_(jumps)
{
}

e2d::JumpBy * e2d::JumpBy::Clone() const
{
	return new JumpBy(duration_, delta_pos_, height_, jumps_);
}

e2d::JumpBy * e2d::JumpBy::Reverse() const
{
	return new JumpBy(duration_, -delta_pos_, height_, jumps_);
}

void e2d::JumpBy::Init()
{
	FiniteTimeAction::Init();

	if (target_)
	{
		prev_pos_ = start_pos_ = target_->GetPos();
	}
}

void e2d::JumpBy::Update()
{
	FiniteTimeAction::Update();

	if (target_)
	{
		float frac = fmod(delta_ * jumps_, 1.f);
		float x = delta_pos_.x * delta_;
		float y = height_ * 4 * frac * (1 - frac);
		y += delta_pos_.y * delta_;

		Point currentPos = target_->GetPos();

		Point diff = currentPos - prev_pos_;
		start_pos_ = diff + start_pos_;

		Point newPos = start_pos_ + Point(x, y);
		target_->SetPos(newPos);

		prev_pos_ = newPos;
	}
}
