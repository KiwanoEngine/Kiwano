#include "..\e2dbase.h"
#include "..\e2dmanager.h"

e2d::Object::Object()
	: _refCount(0)
{
	ObjectManager::__add(this);
}

e2d::Object::~Object()
{
}

// 引用计数加一
void e2d::Object::retain()
{
	++_refCount;
}

// 引用计数减一
void e2d::Object::release()
{
	_refCount--;
	// 通知对象管理池刷新
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return _refCount;
}
