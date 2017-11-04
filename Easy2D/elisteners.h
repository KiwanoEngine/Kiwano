#pragma once
#include "ebase.h"
#include "egeometry.h"

namespace e2d
{

class ENode;
class EMsgManager;
class EPhysicsManager;

// 监听器
class EListener :
	public EObject
{
	friend EMsgManager;

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

protected:
	EString		m_sName;
	bool		m_bRunning;
	bool		m_bAlways;
	ENode *		m_pParentNode;
};


// 鼠标消息监听器
class EListenerMouse :
	public EListener
{
	friend EMsgManager;

public:
	EListenerMouse();

	EListenerMouse(
		const EString &name
	);

	EListenerMouse(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EListenerMouse(
		const EString &name,
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// 绑定监听器到场景
	virtual void bindWith(
		EScene * pParentScene
	) override;

	// 绑定监听器到节点
	virtual void bindWith(
		ENode * pParentNode
	) override;

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	MOUSE_LISTENER_CALLBACK m_Callback;
};


// 鼠标按下消息监听器
class EListenerMousePress :
	public EListenerMouse
{
public:
	EListenerMousePress();

	EListenerMousePress(
		const EString &name
	);

	EListenerMousePress(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	EListenerMousePress(
		const EString &name,
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	MOUSE_PRESS_LISTENER_CALLBACK m_Callback;
};


// 鼠标点击消息监听器
class EListenerMouseClick :
	public EListenerMouse
{
public:
	EListenerMouseClick();

	EListenerMouseClick(
		const EString &name
	);

	EListenerMouseClick(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	EListenerMouseClick(
		const EString &name,
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	bool m_bPressed;
	MOUSE_CLICK_LISTENER_CALLBACK m_Callback;
};


// 鼠标点击消息监听器
class EListenerMouseDoubleClick :
	public EListenerMouse
{
public:
	EListenerMouseDoubleClick();

	EListenerMouseDoubleClick(
		const EString &name
	);

	EListenerMouseDoubleClick(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	EListenerMouseDoubleClick(
		const EString &name,
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	bool m_bPressed;
	MOUSE_DBLCLK_LISTENER_CALLBACK m_Callback;
};


// 鼠标拖动消息监听器
class EListenerMouseDrag :
	public EListenerMouse
{
public:
	EListenerMouseDrag();

	EListenerMouseDrag(
		const EString &name
	);

	EListenerMouseDrag(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	EListenerMouseDrag(
		const EString &name,
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	EPoint	m_Begin;
	MOUSE_DRAG_LISTENER_CALLBACK m_Callback;
};


// 按键消息监听器
class EListenerKeyboard :
	public EListener
{
	friend EMsgManager;

public:
	EListenerKeyboard();

	EListenerKeyboard(
		const EString &name
	);

	EListenerKeyboard(
		const KEY_LISTENER_CALLBACK &callback
	);

	EListenerKeyboard(
		const EString &name,
		const KEY_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const KEY_LISTENER_CALLBACK &callback
	);

	// 绑定监听器到场景
	virtual void bindWith(
		EScene * pParentScene
	) override;

	// 绑定监听器到节点
	virtual void bindWith(
		ENode * pParentNode
	) override;

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	KEY_LISTENER_CALLBACK m_Callback;
};


// 按键按下消息监听
class EListenerKeyboardPress :
	public EListenerKeyboard
{
	friend EMsgManager;

public:
	EListenerKeyboardPress();

	EListenerKeyboardPress(
		const EString &name
	);

	EListenerKeyboardPress(
		const KEY_LISTENER_CALLBACK &callback
	);

	EListenerKeyboardPress(
		const EString &name,
		const KEY_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;
};


// 物理世界消息监听器回调函数（参数：主动方、被动方、两方关系）
typedef std::function<void(ENode *active, ENode *passive, int relation)> PHYSICS_LISTENER_CALLBACK;

// 碰撞消息监听器回调函数（参数：主动方、被动方）
typedef std::function<void(ENode *active, ENode *passive)> COLLISION_LISTENER_CALLBACK;

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
		const PHYSICS_LISTENER_CALLBACK &callback
	);

	EListenerPhysics(
		const EString &name,
		const PHYSICS_LISTENER_CALLBACK &callback
	);

	// 设置监听器回调函数
	void setCallback(
		const PHYSICS_LISTENER_CALLBACK &callback
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
	PHYSICS_LISTENER_CALLBACK m_Callback;
};


class EListenerPhysicsContact :
	public EListenerPhysics
{
	friend EMsgManager;

public:
	EListenerPhysicsContact();

	EListenerPhysicsContact(
		const EString &name
	);

	EListenerPhysicsContact(
		const COLLISION_LISTENER_CALLBACK &callback
	);

	EListenerPhysicsContact(
		const EString &name,
		const COLLISION_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;

protected:
	COLLISION_LISTENER_CALLBACK m_Callback;
};

}