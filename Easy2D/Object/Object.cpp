#include "easy2d.h"

Object::Object() :
	m_nRef(0)
{
	FreePool::__add(this);	// 将该对象放入释放池中
}

Object::~Object()
{
}

void Object::retain()
{
	m_nRef++;	// 引用计数加一
}

void Object::release()
{
	m_nRef--;	// 引用计数减一
}
