#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class EGame;
class ERenderer;
class EObject;
class EScene;
class ENode;
class ETimer;
class EAction;
class EGeometry;
class ETransition;
class EListenerPhysics;

// 对象管理器
class EObjectManager
{
	friend EGame;

public:
	// 将一个节点放入内存池
	static void add(
		e2d::EObject * nptr
	);

	// 通知内存池刷新
	static void notifyFlush();

private:
	// 刷新内存池
	static void __flush();
};


// 场景管理器
class ESceneManager
{
	friend EGame;
	friend ERenderer;

public:
	// 切换场景
	static void enterScene(
		EScene * scene,						/* 下一个场景的指针 */
		ETransition * transition = nullptr,	/* 场景切换动画 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void backScene(
		ETransition * transition = nullptr	/* 场景切换动画 */
	);

	// 清空保存的所有场景
	static void clearScene();

	// 获取当前场景
	static EScene * getCurrentScene();

private:
	// 更新场景内容
	static void __update();

	// 渲染场景画面
	static void __render();

	// 进入下一场景
	static void __enterNextScene();
};


// 定时器管理器
class ETimerManager
{
	friend EGame;
	friend EScene;
	friend ENode;

public:
	// 绑定定时器到场景
	static void bindTimer(
		ETimer * timer,
		EScene * pParentScene
	);

	// 绑定定时器到节点
	static void bindTimer(
		ETimer * timer,
		ENode * pParentNode
	);

	// 启动具有相同名称的定时器
	static void startTimers(
		const EString &name
	);

	// 停止具有相同名称的定时器
	static void stopTimers(
		const EString &name
	);

	// 删除具有相同名称的定时器
	static void delTimers(
		const EString &name
	);

	// 启动绑定在场景及其子节点上的所有定时器
	static void startAllTimersBindedWith(
		EScene * pParentScene
	);

	// 停止绑定在场景及其子节点上的所有定时器
	static void stopAllTimersBindedWith(
		EScene * pParentScene
	);

	// 启动绑定在节点上的所有定时器
	static void startAllTimersBindedWith(
		ENode * pParentNode
	);

	// 停止绑定在节点上的所有定时器
	static void stopAllTimersBindedWith(
		ENode * pParentNode
	);

	// 启动所有定时器
	static void startAllTimers();

	// 停止所有定时器
	static void stopAllTimers();

private:
	// 更新定时器
	static void __update();

	// 清空绑定在节点上的所有定时器
	static void __clearAllTimersBindedWith(
		ENode * pParentNode
	);

	// 重置定时器状态
	static void __resetAllTimers();
};


// 动作管理器
class EActionManager
{
	friend EGame;
	friend EScene;
	friend ENode;

public:
	// 添加动作
	static void addAction(
		EAction * action
	);

	// 继续绑定在节点上的所有动作
	static void resumeAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 暂停绑定在节点上的所有动作
	static void pauseAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 停止绑定在节点上的所有动作
	static void stopAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 继续所有动作
	static void resumeAllActions();

	// 暂停所有动作
	static void pauseAllActions();

	// 停止所有动作
	static void stopAllActions();

private:
	// 更新动画状态
	static void __update();

	// 清空绑定在节点上的所有动作
	static void __clearAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 重置所有动作状态
	static void __resetAllActions();
};


class EPhysicsManager
{
	friend EGame;
	friend EScene;
	friend ENode;
	friend EGeometry;

public:
	// 将监听器与场景绑定
	static void bindListener(
		EListenerPhysics * listener,
		EScene * pParentScene
	);

	// 将监听器与节点绑定
	static void bindListener(
		EListenerPhysics * listener,
		ENode * pParentNode
	);

	// 启动具有相同名称的监听器
	static void startListeners(
		const EString &name
	);

	// 停止具有相同名称的监听器
	static void stopListeners(
		const EString &name
	);

	// 删除具有相同名称的监听器
	static void delListeners(
		const EString &name
	);

	// 启动绑定在场景及其子节点上的所有监听器
	static void startAllListenersBindedWith(
		EScene * pParentScene
	);

	// 停止绑定在场景及其子节点上的所有监听器
	static void stopAllListenersBindedWith(
		EScene * pParentScene
	);

	// 启动绑定在节点上的所有监听器
	static void startAllListenersBindedWith(
		ENode * pParentNode
	);

	// 停止绑定在节点上的所有监听器
	static void stopAllListenersBindedWith(
		ENode * pParentNode
	);

	// 启动所有监听器
	static void startAllListeners();

	// 停止所有监听器
	static void stopAllListeners();

private:
	// 清空监听器管理器
	static void _clearManager();

	// 添加形状
	static void _addGeometry(
		EGeometry * geometry
	);

	// 删除已绑定的形状
	static void _delGeometry(
		EGeometry * geometry
	);

	// 清空绑定在节点上的所有监听器
	static void _clearAllListenersBindedWith(
		ENode * pParentNode
	);

	// 几何图形判断程序
	static void PhysicsGeometryProc(
		EGeometry * pActiveGeometry
	);

	// 物理碰撞监听器执行程序
	static void PhysicsListenerProc();
};

}