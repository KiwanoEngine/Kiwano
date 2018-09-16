#include "..\e2daction.h"
#include "..\e2dobject.h"


e2d::MoveBy::MoveBy(float duration, Point vector)
	: FiniteTimeAction(duration)
{
	delta_pos_ = vector;
}

void e2d::MoveBy::Init()
{
	FiniteTimeAction::Init();

	if (target_)
	{
		prev_pos_ = start_pos_ = target_->GetPos();
	}
}

void e2d::MoveBy::Update()
{
	FiniteTimeAction::Update();

	if (target_)
	{
		Point currentPos = target_->GetPos();
		Point diff = currentPos - prev_pos_;
		start_pos_ = start_pos_ + diff;

		Point newPos = start_pos_ + (delta_pos_ * delta_);
		target_->SetPosition(newPos);

		prev_pos_ = newPos;
	}
}

e2d::MoveBy * e2d::MoveBy::Clone() const
{
	return new MoveBy(duration_, delta_pos_);
}

e2d::MoveBy * e2d::MoveBy::Reverse() const
{
	return new MoveBy(duration_, -delta_pos_);
}