#include "..\e2dcommon.h"

e2d::Ref::Ref()
	: ref_count_(0)
{
}

e2d::Ref::~Ref()
{
}

void e2d::Ref::Retain()
{
	ref_count_++;
}

void e2d::Ref::Release()
{
	ref_count_--;
}

int e2d::Ref::GetRefCount() const
{
	return ref_count_;
}
