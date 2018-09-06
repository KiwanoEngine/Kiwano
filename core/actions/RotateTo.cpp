#include "..\e2daction.h"
#include "..\e2dobject.h"


e2d::RotateTo::RotateTo(float duration, float rotation)
	: RotateBy(duration, 0)
{
	end_val_ = rotation;
}

e2d::RotateTo * e2d::RotateTo::Clone() const
{
	return new (e2d::autorelease) RotateTo(duration_, end_val_);
}

void e2d::RotateTo::Init()
{
	RotateBy::Init();
	delta_val_ = end_val_ - start_val_;
}
