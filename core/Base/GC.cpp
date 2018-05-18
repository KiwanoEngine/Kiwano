#include "..\e2dbase.h"

// GC 释放池的实现机制：
// Object 类中的引用计数（_refCount）在一定程度上防止了内存泄漏
// 它记录了对象被使用的次数，当计数为 0 时，GC 会自动释放这个对象
// 所有的 Object 对象都应在被使用时（例如 Text 添加到了场景中）
// 调用 retain 函数保证该对象不被删除，并在不再使用时调用 release 函数

// 对象管理池
static std::set<e2d::Object*> s_vObjectPool;
// 标志释放池执行状态
static bool s_bNotifyed = false;

void e2d::GC::__update()
{
	if (!s_bNotifyed) return;

	s_bNotifyed = false;
	for (auto iter = s_vObjectPool.begin(); iter != s_vObjectPool.end();)
	{
		if ((*iter)->getRefCount() <= 0)
		{
			(*iter)->onDestroy();
			delete (*iter);
			iter = s_vObjectPool.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::GC::__clear()
{
	for (auto pObj : s_vObjectPool)
	{
		delete pObj;
	}
	s_vObjectPool.clear();
}

void e2d::GC::__add(e2d::Object * pObject)
{
	if (pObject)
	{
		s_vObjectPool.insert(pObject);
	}
}

void e2d::GC::notify()
{
	s_bNotifyed = true;
}

void e2d::GC::flush()
{
	GC::notify();
	GC::__update();
}
