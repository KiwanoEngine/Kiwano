#include "..\ebase.h"
#include "..\etools.h"

e2d::EObject::EObject()
	: m_nRefCount(0)
	, m_bManaged(false)
	, m_bAutoRelease(false)
{
	EObjectManager::add(this);	// 将该对象放入释放池中
}

e2d::EObject::~EObject()
{
}

// 引用计数加一
void e2d::EObject::retain()
{
	m_nRefCount++;
}

// 引用计数减一
void e2d::EObject::release()
{
	m_nRefCount--;
}

void e2d::EObject::autoRelease()
{
	m_bAutoRelease = true;
}
