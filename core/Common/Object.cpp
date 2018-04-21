#include "..\ebase.h"
#include "..\emanager.h"

e2d::Object::Object()
	: m_nRefCount(0)
{
	ObjectManager::__add(this);
}

e2d::Object::~Object()
{
}

// 引用计数加一
void e2d::Object::retain()
{
	m_nRefCount++;
}

// 引用计数减一
void e2d::Object::release()
{
	m_nRefCount--;
	// 通知对象管理池刷新
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return m_nRefCount;
}
