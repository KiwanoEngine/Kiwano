#include "..\e2dbase.h"
#include "..\e2dmanager.h"


e2d::Object::Object()
	: _refCount(0)
{
	if (Game::getInstance()->getConfig().isObjectsAutoReleaseEnabled())
	{
		this->autorelease();
	}
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