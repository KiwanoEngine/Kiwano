#pragma once
#include "ebase.h"
#include "egeometry.h"

namespace e2d
{

class ENode;
class EPhysicsManager;

// 监听器
class EListener :
	public EObject
{
public:
	EListener();

	EListener(
		const EString &name
	);

	// 获取监听器状态
	bool isRunning() const;

	// 启动监听
	void start();

	// 停止监听
	void stop();

	// 获取监听器名称
	EString getName() const;

	// 获取监听器所在节点
	ENode * getParentNode() const;

	// 设置监听器名称
	void setName(
		const EString &name
	);

	// 设置监听器吞噬消息
	void setSwallow(
		bool bSwallow
	);

	// 设置监听器在游戏暂停时继续工作
	void setAlwaysWorking(
		bool bAlways
	);

	// 绑定监听器到场景
	virtual void bindWith(
		EScene * pParentScene
	) = 0;

	// 绑定监听器到节点
	virtual void bindWith(
		ENode * pParentNode
	) = 0;

protected:
	// 执行监听器回调函数
	virtual void _callOn() = 0;

	// 获取监听器状态是否就绪
	virtual bool _isReady() const;

protected:
	EString		m_sName;
	bool		m_bRunning;
	bool		m_bAlways;
	bool		m_bSwallow;
	ENode *		m_pParentNode;
};


// 物理世界消息监听器
class EListenerPhysics :
	public EListener
{
	friend EPhysicsManager;

public:
	EListenerPhysics();

	EListenerPhysics(
		const EString &name
	);

	EListenerPhysics(
		const PhysLsnrCallback &callback
	);

	EListenerPhysics(
		const EString &name,
		const PhysLsnrCallback &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const PhysLsnrCallback &callback
	);

	// 将监听器与场景绑定
	virtual void bindWith(
		EScene * pParentScene
	) override;

	// 将监听器与节点绑定
	virtual void bindWith(
		ENode * pParentNode
	) override;

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	PhysLsnrCallback m_Callback;
};


// 几何体冲突消息监听器
class EListenerPhysicsCollision :
	public EListenerPhysics
{
public:
	EListenerPhysicsCollision();

	EListenerPhysicsCollision(
		const EString &name
	);

	EListenerPhysicsCollision(
		const ClsLsnrCallback &callback
	);

	EListenerPhysicsCollision(
		const EString &name,
		const ClsLsnrCallback &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	ClsLsnrCallback m_Callback;
};

}