#include "..\e2daction.h"
#include "..\e2dnode.h"

e2d::ScaleTo::ScaleTo(float duration, float scale)
	: ScaleBy(duration, 0, 0)
{
	end_scale_x_ = scale;
	end_scale_y_ = scale;
}

e2d::ScaleTo::ScaleTo(float duration, float scale_x, float scale_y)
	: ScaleBy(duration, 0, 0)
{
	end_scale_x_ = scale_x;
	end_scale_y_ = scale_y;
}

e2d::ScaleTo * e2d::ScaleTo::Clone() const
{
	return new (e2d::autorelease) ScaleTo(duration_, end_scale_x_, end_scale_y_);
}

void e2d::ScaleTo::Init()
{
	ScaleBy::Init();
	delta_x_ = end_scale_x_ - start_scale_x_;
	delta_y_ = end_scale_y_ - start_scale_y_;
}
