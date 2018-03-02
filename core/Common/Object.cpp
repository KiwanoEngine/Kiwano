#include "..\ebase.h"
#include "..\emanagers.h"

e2d::Obj::Obj()
	: m_nRefCount(0)
	, m_bManaged(false)
{
	ObjectManager::preload(this);	// 将该对象放入释放池中
}

e2d::Obj::~Obj()
{
}

// 引用计数加一
void e2d::Obj::retain()
{
	m_nRefCount++;
}

// 引用计数减一
void e2d::Obj::release()
{
	m_nRefCount--;
	// 通知对象管理池刷新
	ObjectManager::notifyFlush();
}
