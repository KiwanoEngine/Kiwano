#pragma once
#include "e2dbase.h"


namespace e2d
{

class Node;
class Listener;
class ColliderManager;

// 碰撞事件
class Collision
{
	friend class ColliderManager;

public:
	// 添加可互相碰撞物体的名称
	static void addName(
		const String& name1,
		const String& name2
	);

	// 添加可互相碰撞物体的名称
	static void addName(
		const std::vector<std::pair<String, String> >& names
	);

	// 判断两个物体是否是可碰撞的
	static bool isCollidable(
		Node * node1,
		Node * node2
	);

	// 判断两个物体是否是可碰撞的
	static bool isCollidable(
		const String& name1,
		const String& name2
	);

	// 获取碰撞发生时的主动体
	static Node * getActiveNode();

	// 获取碰撞发生时的被动体
	static Node * getPassiveNode();

	// 判断发生碰撞的节点名称是否相同
	static bool isCausedBy(
		const String& name1,
		const String& name2
	);

	// 判断两物体是否发生碰撞
	static bool isCausedBy(
		Node * node1,
		Node * node2
	);

	// 判断发生碰撞的任意一方名称是否相同
	// 若相同，返回其指针，否则返回空
	static Node * isCausedBy(
		const String& name
	);

	// 判断物体是否发生碰撞
	// 如果是，返回与其相撞的节点指针，否则返回空
	static Node * isCausedBy(
		Node * node
	);

	// 添加碰撞监听
	static Listener * addListener(
		const Function& func,		/* 监听到碰撞时的执行函数 */
		const String& name = L"",	/* 监听器名称 */
		bool paused = false			/* 是否暂停 */
	);

	// 添加碰撞监听
	static void addListener(
		Listener * listener			/* 监听器 */
	);

	// 移除监听器
	static void removeListener(
		Listener * listener			/* 监听器 */
	);

	// 启动碰撞监听
	static void startListener(
		const String& name
	);

	// 停止碰撞监听
	static void stopListener(
		const String& name
	);

	// 移除碰撞监听
	static void removeListener(
		const String& name
	);

	// 启动所有监听器
	static void startAllListeners();

	// 停止所有监听器
	static void stopAllListeners();

	// 移除所有监听器
	static void removeAllListeners();

private:
	// 更新监听器
	static void __update(
		Node * active,
		Node * passive
	);
};

}