#include "..\emanagers.h"
#include "..\ebase.h"

// EObjectManager 释放池的实现机制：
// EObject 类中的引用计数（m_nRefCount）保证了指针的使用安全
// 它记录了对象被使用的次数，当计数为 0 时，EObjectManager 会自动释放这个对象
// 所有的 EObject 对象都应在被使用时（例如 Text 添加到了场景中）
// 调用 retain 函数保证该对象不被删除，并在不再使用时调用 release 函数
// 让其自动释放

// 释放池容器
static std::vector<e2d::EObject*> s_vPool;
// 标志释放池执行状态
static bool s_bNotifyed = false;

void e2d::EObjectManager::__flush()
{
	if (!s_bNotifyed) return;

	s_bNotifyed = false;
	// 创建迭代器
	static std::vector<e2d::EObject*>::iterator iter;
	// 循环遍历容器中的所有对象
	for (iter = s_vPool.begin(); iter != s_vPool.end();)
	{
		if ((*iter)->m_nRefCount <= 0)
		{
			// 若对象的引用的计数小于等于 0, 释放该对象
			delete (*iter);
			// 从释放池中删除该对象
			iter = s_vPool.erase(iter);
		}
		else
		{
			iter++;		// 移动迭代器
		}
	}
}

void e2d::EObjectManager::add(e2d::EObject * nptr)
{
	if (!nptr->m_bManaged)
	{
		nptr->m_bManaged = true;
		s_vPool.push_back(nptr);	// 将一个对象放入释放池中
	}
}

void e2d::EObjectManager::notifyFlush()
{
	s_bNotifyed = true;
}
