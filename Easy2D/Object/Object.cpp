#include "..\easy2d.h"

Object::Object() :
	m_nRefCount(0)
{
}

Object::~Object()
{
}

void Object::retain()
{
	m_nRefCount++;	// 引用计数加一
}

void Object::release()
{
	m_nRefCount--;	// 引用计数减一
}

void Object::autoRelease()
{
	FreePool::__add(this);	// 将该对象放入释放池中
}
