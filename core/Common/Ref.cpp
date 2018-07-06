#include "..\e2dcommon.h"

e2d::Ref::Ref()
	: _refCount(0)
{
}

e2d::Ref::~Ref()
{
}

void e2d::Ref::retain()
{
	_refCount++;
}

void e2d::Ref::release()
{
	_refCount--;
}

int e2d::Ref::getRefCount() const
{
	return _refCount;
}
