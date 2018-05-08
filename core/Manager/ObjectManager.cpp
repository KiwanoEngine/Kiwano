#include "..\e2dmanager.h"
#include "..\e2dbase.h"

// ObjectManager 释放池的实现机制：
// Object 类中的引用计数（_nRefCount）在一定程度上保证了指针的使用安全
// 它记录了对象被使用的次数，当计数为 0 时，ObjectManager 会自动释放这个对象
// 所有的 Object 对象都应在被使用时（例如 Text 添加到了场景中）
// 调用 retain 函数保证该对象不被删除，并在不再使用时调用 release 函数

// 对象管理池
static std::set<e2d::Object*> s_vObjectPool;
// 标志释放池执行状态
static bool s_bNotifyed = false;

void e2d::ObjectManager::__update()
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
			iter++;		// 移动迭代器
		}
	}
}

void e2d::ObjectManager::__clear()
{
	for (auto pObj : s_vObjectPool)
	{
		delete pObj;
	}
	s_vObjectPool.clear();
}

void e2d::ObjectManager::__add(e2d::Object * pObject)
{
	if (pObject)
	{
		s_vObjectPool.insert(pObject);
	}
}

void e2d::ObjectManager::flush()
{
	s_bNotifyed = true;
}
