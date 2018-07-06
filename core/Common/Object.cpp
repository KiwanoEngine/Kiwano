#include "..\e2dbase.h"
#include "..\e2dmanager.h"

e2d::autorelease_t const e2d::autorelease = e2d::autorelease_t();

void * operator new(size_t _Size, e2d::autorelease_t const &) E2D_NOEXCEPT
{
	void* p = ::operator new (_Size, std::nothrow);
	e2d::Object * newObject = static_cast<e2d::Object*>(p);
	if (newObject)
	{
		newObject->autorelease();
	}
	return p;
}

void operator delete(void * _Block, e2d::autorelease_t const &) E2D_NOEXCEPT
{
	::operator delete (_Block, std::nothrow);
}


e2d::Object::Object()
	: _refCount(0)
{
}

e2d::Object::~Object()
{
}

void e2d::Object::autorelease()
{
	GC::getInstance()->addObject(this);
}

void e2d::Object::retain()
{
	++_refCount;
}

void e2d::Object::release()
{
	_refCount--;
	GC::getInstance()->notify();
}

int e2d::Object::getRefCount() const
{
	return _refCount;
}