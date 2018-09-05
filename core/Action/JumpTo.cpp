#include "..\e2daction.h"
#include "..\e2dcomponent.h"

e2d::JumpTo::JumpTo(float duration, const Point & pos, float height, int jumps)
	: JumpBy(duration, Point(), height, jumps)
	, end_pos_(pos)
{
}

e2d::JumpTo * e2d::JumpTo::Clone() const
{
	return new (e2d::autorelease) JumpTo(duration_, end_pos_, height_, jumps_);
}

void e2d::JumpTo::Init()
{
	JumpBy::Init();
	delta_pos_ = end_pos_ - start_pos_;
}
