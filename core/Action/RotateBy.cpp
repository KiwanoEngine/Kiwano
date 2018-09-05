#include "..\e2daction.h"
#include "..\e2dcomponent.h"


e2d::RotateBy::RotateBy(float duration, float rotation)
	: FiniteTimeAction(duration)
{
	delta_val_ = rotation;
}

void e2d::RotateBy::Init()
{
	FiniteTimeAction::Init();

	if (target_)
	{
		start_val_ = target_->GetRotation();
	}
}

void e2d::RotateBy::Update()
{
	FiniteTimeAction::Update();

	if (target_)
	{
		target_->SetRotation(start_val_ + delta_val_ * delta_);
	}
}

e2d::RotateBy * e2d::RotateBy::Clone() const
{
	return new (e2d::autorelease) RotateBy(duration_, delta_val_);
}

e2d::RotateBy * e2d::RotateBy::Reverse() const
{
	return new (e2d::autorelease) RotateBy(duration_, -delta_val_);
}