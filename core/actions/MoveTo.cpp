#include "..\e2daction.h"
#include "..\e2dobject.h"

e2d::MoveTo::MoveTo(float duration, Point pos)
	: MoveBy(duration, Point())
{
	end_pos_ = pos;
}

e2d::MoveTo * e2d::MoveTo::Clone() const
{
	return new (e2d::autorelease) MoveTo(duration_, end_pos_);
}

void e2d::MoveTo::Init()
{
	MoveBy::Init();
	delta_pos_ = end_pos_ - start_pos_;
}
