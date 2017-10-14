#pragma once
#include "ebase.h"

namespace e2d
{

// 对象管理器
class EObjectManager
{
	friend EApp;

public:
	// 将一个节点放入内存池
	static void add(
		e2d::EObject * nptr
	);

	// 删除所有节点
	static void clearAllObjects();

	// 通知内存池刷新
	static void notifyFlush();

private:
	// 刷新内存池
	static void __flush();
};


// 定时器
class ETimer :
	public EObject
{

};


// 定时器管理器
class ETimerManager
{

};

}