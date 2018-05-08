#include "..\e2dbase.h"
#include "..\e2dmanager.h"

e2d::Object::Object()
	: _nRefCount(0)
{
	ObjectManager::__add(this);
}

e2d::Object::~Object()
{
}

// 引用计数加一
void e2d::Object::retain()
{
	_nRefCount++;
}

// 引用计数减一
void e2d::Object::release()
{
	_nRefCount--;
	// 通知对象管理池刷新
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return _nRefCount;
}
