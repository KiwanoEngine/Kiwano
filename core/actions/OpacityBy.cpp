#include "..\e2daction.h"
#include "..\e2dobject.h"


e2d::OpacityBy::OpacityBy(float duration, float opacity)
	: FiniteTimeAction(duration)
{
	delta_val_ = opacity;
}

void e2d::OpacityBy::Init()
{
	FiniteTimeAction::Init();

	if (target_)
	{
		start_val_ = target_->GetOpacity();
	}
}

void e2d::OpacityBy::Update()
{
	FiniteTimeAction::Update();

	if (target_)
	{
		target_->SetOpacity(start_val_ + delta_val_ * delta_);
	}
}

e2d::OpacityBy * e2d::OpacityBy::Clone() const
{
	return new OpacityBy(duration_, delta_val_);
}

e2d::OpacityBy * e2d::OpacityBy::Reverse() const
{
	return new OpacityBy(duration_, -delta_val_);
}