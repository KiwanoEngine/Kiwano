#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{

class Game;
class Input;
class Renderer;
class Node;
class Task;
class Action;
class Player;
class Collider;
class Transition;


// 场景管理器
class SceneManager
{
public:
	// 获取场景管理器实例
	static SceneManager * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 切换场景
	void enter(
		Scene * scene,						/* 下一个场景的指针 */
		Transition * transition = nullptr,	/* 场景切换动作 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	void back(
		Transition * transition = nullptr	/* 场景切换动作 */
	);

	// 清空保存的所有场景
	void clear();

	// 获取当前场景
	Scene * getCurrentScene();

	// 获取场景栈
	std::stack<Scene*> getSceneStack();

	// 是否正在进行转场动作
	bool isTransitioning();

	// 更新场景内容
	void update();

	// 渲染场景画面
	void render();

private:
	SceneManager();

	~SceneManager();

	E2D_DISABLE_COPY(SceneManager);

private:
	bool				_saveCurrScene;
	Scene *				_currScene;
	Scene *				_nextScene;
	Transition *		_transition;
	std::stack<Scene*>	_scenes;

	static SceneManager * _instance;
};


// 动作管理器
class ActionManager
{
	friend class Action;

public:
	// 获取动作管理器实例
	static ActionManager * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 获取所有名称相同的动作
	std::vector<Action *> get(
		const String& name
	);

	// 获取所有动作
	const std::vector<Action*>& getAll();

	// 执行动作
	void start(
		Action * action,
		Node * target,
		bool paused
	);

	// 继续名称相同的所有动作
	void resume(
		const String& name
	);

	// 暂停名称相同的所有动作
	void pause(
		const String& name
	);

	// 停止名称相同的所有动作
	void stop(
		const String& name
	);

	// 继续绑定在节点上的所有动作
	void resumeAllBindedWith(
		Node * target
	);

	// 暂停绑定在节点上的所有动作
	void pauseAllBindedWith(
		Node * target
	);

	// 停止绑定在节点上的所有动作
	void stopAllBindedWith(
		Node * target
	);

	// 强制清除绑定在节点上的所有动作
	void clearAllBindedWith(
		Node * target
	);

	// 强制清除所有动作
	void clearAll();

	// 更新动作管理器状态
	void update();

	// 刷新所有动作计时
	void updateTime();

private:
	ActionManager();

	~ActionManager();

	E2D_DISABLE_COPY(ActionManager);

	// 添加动作
	void __add(
		Action * action
	);

	// 删除动作
	void __remove(
		Action * action
	);

private:
	std::vector<Action*> _actions;
	std::vector<Action*> _runningActions;

	static ActionManager * _instance;
};


class Listener;

// 碰撞体管理器
class CollisionManager
{
	friend class Node;

public:
	// 获取碰撞体管理器实例
	static CollisionManager * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 添加可互相碰撞物体的名称
	void addName(
		const String& name1,
		const String& name2
	);

	// 添加可互相碰撞物体的名称
	void addName(
		const std::vector<std::pair<String, String> >& names
	);

	// 判断两个物体是否是可碰撞的
	bool isCollidable(
		Node * node1,
		Node * node2
	);

	// 判断两个物体是否是可碰撞的
	bool isCollidable(
		const String& name1,
		const String& name2
	);

	// 获取碰撞发生时的主动体
	Node * getActiveNode();

	// 获取碰撞发生时的被动体
	Node * getPassiveNode();

	// 判断发生碰撞的节点名称是否相同
	bool isCausedBy(
		const String& name1,
		const String& name2
	);

	// 判断两物体是否发生碰撞
	bool isCausedBy(
		Node * node1,
		Node * node2
	);

	// 判断发生碰撞的任意一方名称是否相同
	// 若相同，返回其指针，否则返回空
	Node * isCausedBy(
		const String& name
	);

	// 判断物体是否发生碰撞
	// 如果是，返回与其相撞的节点指针，否则返回空
	Node * isCausedBy(
		Node * node
	);

	// 添加碰撞监听
	Listener * addListener(
		const Function& func,		/* 监听到碰撞时的执行函数 */
		const String& name = L"",	/* 监听器名称 */
		bool paused = false			/* 是否暂停 */
	);

	// 添加碰撞监听
	void addListener(
		Listener * listener			/* 监听器 */
	);

	// 移除监听器
	void removeListener(
		Listener * listener			/* 监听器 */
	);

	// 启动碰撞监听
	void startListener(
		const String& name
	);

	// 停止碰撞监听
	void stopListener(
		const String& name
	);

	// 移除碰撞监听
	void removeListener(
		const String& name
	);

	// 启动所有监听器
	void startAllListeners();

	// 停止所有监听器
	void stopAllListeners();

	// 移除所有监听器
	void removeAllListeners();

	// 强制清除所有监听器
	void clearAllListeners();

	// 更新碰撞体
	void updateCollider(
		Node * node
	);

	// 更新碰撞体管理器
	void update();

private:
	CollisionManager();

	~CollisionManager();

	E2D_DISABLE_COPY(CollisionManager);

	void __remove(
		Node* node
	);

	// 更新监听器
	void __update(
		Node * active,
		Node * passive
	);

private:
	typedef std::pair<UINT, UINT> HashPair;
	
	e2d::Node * _activeNode;
	e2d::Node * _passiveNode;
	std::set<Node*> _collisionNodes;
	std::set<HashPair> _collisionList;
	std::vector<Listener*> _listeners;

	static CollisionManager * _instance;
};

}