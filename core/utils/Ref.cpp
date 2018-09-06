#include "..\e2dobject.h"

e2d::Ref::Ref()
	: ref_count_(0)
{
}

e2d::Ref::~Ref()
{
}

LONG e2d::Ref::Retain()
{
	return ::InterlockedIncrement(&ref_count_);
}

LONG e2d::Ref::Release()
{
	LONG new_count = ::InterlockedDecrement(&ref_count_);

	if (new_count <= 0)
	{
		delete this;
		return 0;
	}

	return new_count;
}

LONG e2d::Ref::GetRefCount() const
{
	return ref_count_;
}
