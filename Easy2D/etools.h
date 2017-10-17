#pragma once
#include "ebase.h"
#include <chrono>

namespace e2d
{

class ETimerManager;

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
	friend ETimerManager;

public:
	ETimer();

	ETimer(
		const EString &name				/* 定时器名称 */
	);

	ETimer(
		const TIMER_CALLBACK &callback,	/* 定时器回调函数 */
		LONGLONG delay = 20LL			/* 时间间隔 */
	);

	ETimer(
		const EString &name,			/* 定时器名称 */
		const TIMER_CALLBACK &callback,	/* 定时器回调函数 */
		LONGLONG delay = 20LL			/* 时间间隔 */
	);

	// 获取定时器状态
	bool isRunning() const;

	// 获取定时器挂起状态
	bool isWaiting() const;

	// 启动监听
	void start();

	// 停止监听
	void stop();

	// 获取定时器名称
	EString getName() const;

	// 获取定时器所在场景
	EScene * getParentScene() const;

	// 获取定时器所在节点
	ENode * getParentNode() const;

	// 设置定时器名称
	void setName(
		const EString &name
	);

	// 设置定时器执行间隔
	void setInterval(
		LONGLONG interval
	);

	// 绑定定时器到场景
	virtual void bindWith(
		EScene * pParentScene
	);

	// 绑定定时器到节点
	virtual void bindWith(
		ENode * pParentNode
	);

protected:
	// 进入等待状态
	void _wait();

	// 唤醒
	void _notify();

	// 执行回调函数
	virtual void _runCallback();

protected:
	EString			m_sName;
	bool			m_bRunning;
	bool			m_bWaiting;
	int				m_nRunTimes;
	EScene *		m_pParentScene;
	ENode *			m_pParentNode;
	TIMER_CALLBACK	m_Callback;
	LONGLONG		m_nInterval;
	std::chrono::steady_clock::time_point m_tLast;
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

	// 清空绑定在场景及其子节点上的所有定时器
	static void clearAllTimersBindedWith(
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

	// 清空绑定在节点上的所有定时器
	static void clearAllTimersBindedWith(
		ENode * pParentNode
	);

	// 启动所有定时器
	static void startAllTimers();

	// 停止所有定时器
	static void stopAllTimers();

	// 清除所有定时器
	static void clearAllTimers();

private:
	// 挂起绑定在场景及其子节点上的所有定时器
	static void _waitAllTimersBindedWith(
		EScene * pParentScene
	);

	// 重启绑定在场景及其子节点上的所有定时器
	static void _notifyAllTimersBindedWith(
		EScene * pParentScene
	);

	// 挂起绑定在节点上的所有定时器
	static void _waitAllTimersBindedWith(
		ENode * pParentNode
	);

	// 重启绑定在节点上的所有定时器
	static void _notifyAllTimersBindedWith(
		ENode * pParentNode
	);

	// 定时器执行程序
	static void TimerProc();
};

}