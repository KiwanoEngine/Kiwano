#include "..\easy2d.h"
#include <assert.h>

// FreePool 释放池的实现机制：
/// Object 类中的引用计数（m_nRef）保证了指针的使用安全
/// 它记录了对象被使用的次数，当计数为 0 时，FreePool 会自动释放这个对象
/// 所有的 Object 对象都应在被使用时（例如 Text 添加到了场景中）
/// 调用 retain 函数保证该对象不被删除，并在不再使用时调用 release 函数
/// 让其自动释放

// 释放池容器
static std::vector<Object*> pool;

void FreePool::__flush()
{
	// 创建迭代器
	std::vector<Object*>::iterator iter;
	// 循环遍历容器中的所有对象
	for (iter = pool.begin(); iter != pool.end();)
	{
		// 若对象的引用的计数为 0
		if ((*iter)->m_nRef == 0)
		{
			// 释放该对象
			delete (*iter);
			// 从释放池中删除该对象
			iter = pool.erase(iter);
		}
		else
		{
			iter++;		// 移动迭代器
		}
	}
}

void FreePool::__add(Object * nptr)
{
#ifdef _DEBUG
	for (auto o : pool)
	{
		assert(o != nptr);	// 不得有重复的指针存在
	}
#endif

	pool.push_back(nptr);		// 将一个对象放入释放池中
}
