#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{

class Game;
class Input;
class Renderer;
class Node;
class Timer;
class Action;
class Player;
class Collider;
class Transition;

// 对象管理器
class ObjectManager
{
	friend Game;
	friend Object;

public:
	// 释放垃圾对象的内存空间
	static void flush();

private:
	// 将对象放入内存池进行管理
	static void __add(
		Object * pObject
	);

	// 更新对象管理器
	static void __update();

	// 清空所有对象
	static void __clear();
};


// 场景管理器
class SceneManager
{
	friend Game;
	friend Renderer;

public:
	// 切换场景
	static void enter(
		Scene * scene,						/* 下一个场景的指针 */
		Transition * transition = nullptr,	/* 场景切换动作 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void back(
		Transition * transition = nullptr	/* 场景切换动作 */
	);

	// 清空保存的所有场景
	static void clear();

	// 获取当前场景
	static Scene * getCurrentScene();

	// 获取场景栈
	static std::stack<Scene*> getSceneStack();

	// 是否正在进行转场动作
	static bool isTransitioning();

private:
	// 更新场景内容
	static void __update();

	// 渲染场景画面
	static void __render();

	// 初始化场景
	static bool __init();

	// 回收场景资源
	static void __uninit();
};


// 动作管理器
class ActionManager
{
	friend Game;
	friend Node;
	friend Action;

public:
	// 执行动作
	static void start(
		Action * action,
		Node * target,
		bool paused
	);

	// 继续名称相同的所有动作
	static void resume(
		const String& strActionName
	);

	// 暂停名称相同的所有动作
	static void pause(
		const String& strActionName
	);

	// 停止名称相同的所有动作
	static void stop(
		const String& strActionName
	);

	// 继续所有动作
	static void resumeAll();

	// 暂停所有动作
	static void pauseAll();

	// 停止所有动作
	static void stopAll();

	// 获取所有名称相同的动作
	static std::vector<Action *> get(
		const String& strActionName
	);

	// 获取所有动作
	static const std::vector<Action*>& getAll();

private:
	// 更新动作状态
	static void __update();

	// 添加动作
	static void __add(
		Action * action
	);

	// 删除动作
	static void __remove(
		Action * action
	);

	// 继续绑定在节点上的所有动作
	static void __resumeAllBindedWith(
		Node * target
	);

	// 暂停绑定在节点上的所有动作
	static void __pauseAllBindedWith(
		Node * target
	);

	// 停止绑定在节点上的所有动作
	static void __stopAllBindedWith(
		Node * target
	);

	// 清空绑定在节点上的所有动作
	static void __clearAllBindedWith(
		Node * target
	);

	// 重置所有动作状态
	static void __resetAll();

	// 回收资源
	static void __uninit();
};


// 键盘和鼠标消息管理器
class InputManager
{
	friend Game;
	friend Input;

public:
	// 添加输入监听
	static void add(
		const Function& func,		/* 监听到用户输入时的执行函数 */
		const String& name = L"",	/* 监听器名称 */
		bool paused = false			/* 是否暂停 */
	);

	// 暂停输入监听
	static void pause(
		const String& name
	);

	// 暂停输入监听
	static void resume(
		const String& name
	);

	// 停止输入监听
	static void stop(
		const String& name
	);

	// 暂停所有监听器
	static void pauseAll();

	// 继续所有监听器
	static void resumeAll();

	// 停止所有监听器
	static void stopAll();

private:
	// 更新监听器
	static void __update();

	// 回收资源
	static void __uninit();
};


// 碰撞管理器
class ColliderManager
{
	friend Game;
	friend Node;
	friend Collider;

public:
	// 开启或关闭碰撞监听功能（默认关闭）
	static void setEnable(
		bool enable
	);

	// 添加碰撞监听
	static void add(
		const Function& func,		/* 监听到碰撞时的执行函数 */
		const String& name = L"",	/* 监听器名称 */
		bool paused = false			/* 是否暂停 */
	);

	// 暂停碰撞监听
	static void pause(
		const String& name
	);

	// 暂停碰撞监听
	static void resume(
		const String& name
	);

	// 停止碰撞监听
	static void stop(
		const String& name
	);

	// 暂停所有监听器
	static void pauseAll();

	// 继续所有监听器
	static void resumeAll();

	// 停止所有监听器
	static void stopAll();

private:
	// 更新监听器
	static void __update();

	// 更新碰撞体
	static void __updateCollider(
		Collider * pActiveCollider
	);

	// 添加碰撞体
	static void __addCollider(
		Collider * pCollider
	);

	// 删除已绑定的碰撞体
	static void __removeCollider(
		Collider * pCollider
	);

	// 回收资源
	static void __uninit();
};

}