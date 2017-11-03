#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class EApp;
class EObject;
class EScene;
class ENode;
class ETimer;
class EAction;
class EListenerMouse;
class EListenerKeyboard;
class EGeometry;
class EListenerPhysics;

// 对象管理器
class EObjectManager
{
	friend EApp;

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


// 消息管理器
class EMsgManager
{
	friend EApp;
	friend EScene;
	friend ENode;

public:
	// 绑定鼠标消息监听器到场景
	static void bindListener(
		EListenerMouse * listener,
		EScene * pParentScene,
		bool always = false /* 是否在游戏暂停时仍然监听 */
	);

	// 绑定鼠标消息监听器到节点
	static void bindListener(
		EListenerMouse * listener,
		ENode * pParentNode,
		bool always = false /* 是否在游戏暂停时仍然监听 */
	);

	// 启动具有相同名称的鼠标消息监听器
	static void startMouseListeners(
		const EString &name
	);

	// 停止具有相同名称的鼠标消息监听器
	static void stopMouseListeners(
		const EString &name
	);

	// 删除具有相同名称的鼠标消息监听器
	static void delMouseListeners(
		const EString &name
	);

	// 启动绑定在场景及其子节点上的所有鼠标消息监听器
	static void startAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// 停止绑定在场景及其子节点上的所有鼠标消息监听器
	static void stopAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// 启动绑定在节点上的所有鼠标消息监听器
	static void startAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// 停止绑定在节点上的所有鼠标消息监听器
	static void stopAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// 启动所有鼠标消息监听器
	static void startAllMouseListeners();

	// 停止所有鼠标消息监听器
	static void stopAllMouseListeners();

	// 绑定按键消息监听器到场景
	static void bindListener(
		EListenerKeyboard * listener,
		EScene * pParentScene,
		bool always = false /* 是否在游戏暂停时仍然监听 */
	);

	// 绑定按键消息监听器到节点
	static void bindListener(
		EListenerKeyboard * listener,
		ENode * pParentNode,
		bool always = false /* 是否在游戏暂停时仍然监听 */
	);

	// 启动名称相同的按键消息监听器
	static void startKeyboardListeners(
		const EString &name
	);

	// 停止名称相同的按键消息监听器
	static void stopKeyboardListeners(
		const EString &name
	);

	// 删除名称相同的按键消息监听器
	static void delKeyboardListeners(
		const EString &name
	);

	// 启动绑定在场景及其子节点上的所有按键消息监听器
	static void startAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// 停止绑定在场景及其子节点上的所有按键消息监听器
	static void stopAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// 启动绑定在节点上的所有按键消息监听器
	static void startAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// 停止绑定在节点上的所有按键消息监听器
	static void stopAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// 启动所有按键消息监听器
	static void startAllKeyboardListeners();

	// 停止所有按键消息监听器
	static void stopAllKeyboardListeners();

private:
	// 清除所有监听器
	static void _clearManager();

	// 清除绑定在节点上的所有鼠标消息监听器
	static void _clearAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// 清除绑定在节点上的所有按键消息监听器
	static void _clearAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// 鼠标消息程序
	static void MouseProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	// 按键消息程序
	static void KeyboardProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
};


// 定时器管理器
class ETimerManager
{
	friend EApp;
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
	// 清空定时器管理器
	static void _clearManager();

	// 清空绑定在节点上的所有定时器
	static void _clearAllTimersBindedWith(
		ENode * pParentNode
	);

	// 重置定时器状态
	static void _resetAllTimers();

	// 定时器执行程序
	static void TimerProc();
};


// 动作管理器
class EActionManager
{
	friend EApp;
	friend EScene;
	friend ENode;

public:
	// 添加动作
	static void addAction(
		EAction * action
	);

	// 继续绑定在节点上的所有动作
	static void startAllActionsBindedWith(
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
	static void startAllActions();

	// 暂停所有动作
	static void pauseAllActions();

	// 停止所有动作
	static void stopAllActions();

private:
	// 清空动画管理器
	static void _clearManager();

	// 清空绑定在节点上的所有动作
	static void _clearAllActionsBindedWith(
		ENode * pTargetNode
	);

	// 重置所有动作状态
	static void _resetAllActions();

	// 动作执行程序
	static void ActionProc();
};


class EPhysicsManager
{
	friend EApp;
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