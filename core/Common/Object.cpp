#include "..\e2dbase.h"
#include "..\e2dmanager.h"


e2d::Object::Object()
	: _refCount(0)
{
}

e2d::Object::~Object()
{
}

void e2d::Object::autorelease()
{
	ObjectManager::__add(this);
}

void e2d::Object::retain()
{
	++_refCount;
}

void e2d::Object::release()
{
	_refCount--;
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return _refCount;
}