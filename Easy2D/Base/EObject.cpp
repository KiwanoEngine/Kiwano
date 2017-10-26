#include "..\ebase.h"
#include "..\emanagers.h"

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
	// 通知对象管理池刷新
	EObjectManager::notifyFlush();
}

void e2d::EObject::autoRelease()
{
	m_bAutoRelease = true;
}
