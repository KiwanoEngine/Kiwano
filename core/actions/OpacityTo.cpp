#include "..\e2daction.h"
#include "..\e2dobject.h"


e2d::OpacityTo::OpacityTo(float duration, float opacity)
	: OpacityBy(duration, 0)
{
	end_val_ = opacity;
}

e2d::OpacityTo * e2d::OpacityTo::Clone() const
{
	return new OpacityTo(duration_, end_val_);
}

void e2d::OpacityTo::Init()
{
	OpacityBy::Init();
	delta_val_ = end_val_ - start_val_;
}
