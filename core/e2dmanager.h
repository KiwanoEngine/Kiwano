#pragma once
#include "e2dmacros.h"
#include "e2dutil.h"

namespace e2d
{


class Node;


// 碰撞体管理器
class CollisionManager
{
	friend class Node;
	friend class Collider;

public:
	// 获取碰撞体管理器实例
	static CollisionManager * GetInstance();

	// 打开或关闭碰撞监听
	// 默认：关闭
	void SetCollisionEnabled(
		bool enabled
	);

	// 添加可互相碰撞物体的名称
	void AddName(
		const String& name1,
		const String& name2
	);

	// 添加可互相碰撞物体的名称
	void AddName(
		const std::vector<std::pair<String, String>>& names
	);

	// 判断两个物体是否是可碰撞的
	bool IsCollidable(
		Node * node1,
		Node * node2
	);

	// 判断两个物体是否是可碰撞的
	bool IsCollidable(
		const String& name1,
		const String& name2
	);

private:
	CollisionManager();

	~CollisionManager();

	E2D_DISABLE_COPY(CollisionManager);

	// 添加碰撞体
	void AddCollider(
		Collider* collider
	);

	// 移除碰撞体
	void RemoveCollider(
		Collider* collider
	);

	// 更新碰撞体
	void UpdateCollider(
		Collider* collider
	);

private:
	bool collision_enabled_;
	std::vector<Collider*> colliders_;
	std::set<std::pair<size_t, size_t>> collision_list_;
};

}