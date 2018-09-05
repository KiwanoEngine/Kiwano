#include "..\e2daction.h"
#include "..\e2dcomponent.h"


e2d::ScaleBy::ScaleBy(float duration, float scale)
	: FiniteTimeAction(duration)
{
	delta_x_ = scale;
	delta_y_ = scale;
}

e2d::ScaleBy::ScaleBy(float duration, float scale_x, float scale_y)
	: FiniteTimeAction(duration)
{
	delta_x_ = scale_x;
	delta_y_ = scale_y;
}

void e2d::ScaleBy::Init()
{
	FiniteTimeAction::Init();

	if (target_)
	{
		start_scale_x_ = target_->GetScaleX();
		start_scale_y_ = target_->GetScaleY();
	}
}

void e2d::ScaleBy::Update()
{
	FiniteTimeAction::Update();

	if (target_)
	{
		target_->SetScale(start_scale_x_ + delta_x_ * delta_, start_scale_y_ + delta_y_ * delta_);
	}
}

e2d::ScaleBy * e2d::ScaleBy::Clone() const
{
	return new (e2d::autorelease) ScaleBy(duration_, delta_x_, delta_y_);
}

e2d::ScaleBy * e2d::ScaleBy::Reverse() const
{
	return new (e2d::autorelease) ScaleBy(duration_, -delta_x_, -delta_y_);
}