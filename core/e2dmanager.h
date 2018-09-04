#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{


class Node;
class Action;

// 动作管理器
class ActionManager
{
	friend class Action;

public:
	// 获取动作管理器实例
	static ActionManager * GetInstance();

	// 获取所有名称相同的动作
	std::vector<Action *> Get(
		const String& name
	);

	// 获取所有动作
	const std::vector<Action*>& GetAll();

	// 执行动作
	void Start(
		Action * action,
		Node * target,
		bool paused
	);

	// 继续名称相同的所有动作
	void Resume(
		const String& name
	);

	// 暂停名称相同的所有动作
	void Pause(
		const String& name
	);

	// 停止名称相同的所有动作
	void Stop(
		const String& name
	);

	// 继续绑定在节点上的所有动作
	void ResumeAllBindedWith(
		Node * target
	);

	// 暂停绑定在节点上的所有动作
	void PauseAllBindedWith(
		Node * target
	);

	// 停止绑定在节点上的所有动作
	void StopAllBindedWith(
		Node * target
	);

	// 强制清除绑定在节点上的所有动作
	void ClearAllBindedWith(
		Node * target
	);

	// 强制清除所有动作
	void ClearAll();

	// 更新动作管理器状态
	void Update();

	// 刷新所有动作计时
	void UpdateTime();

private:
	ActionManager();

	~ActionManager();

	E2D_DISABLE_COPY(ActionManager);

	// 添加动作
	void Add(
		Action * action
	);

	// 删除动作
	void Remove(
		Action * action
	);

private:
	std::vector<Action*> actions_;
	std::vector<Action*> running_actions_;
};


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