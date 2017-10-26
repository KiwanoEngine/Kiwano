#pragma once
#include "ebase.h"

namespace e2d
{

class ENode;
class EMsgManager;


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
class EMouseListener :
	public EListener
{
	friend EMsgManager;

public:
	EMouseListener();

	EMouseListener(
		const EString &name
	);

	EMouseListener(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EMouseListener(
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
class EMousePressListener :
	public EMouseListener
{
public:
	EMousePressListener();

	EMousePressListener(
		const EString &name
	);

	EMousePressListener(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	EMousePressListener(
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
class EMouseClickListener :
	public EMouseListener
{
public:
	EMouseClickListener();

	EMouseClickListener(
		const EString &name
	);

	EMouseClickListener(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	EMouseClickListener(
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
class EMouseDoubleClickListener :
	public EMouseListener
{
public:
	EMouseDoubleClickListener();

	EMouseDoubleClickListener(
		const EString &name
	);

	EMouseDoubleClickListener(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	EMouseDoubleClickListener(
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
class EMouseDragListener :
	public EMouseListener
{
public:
	EMouseDragListener();

	EMouseDragListener(
		const EString &name
	);

	EMouseDragListener(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	EMouseDragListener(
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
class EKeyboardListener :
	public EListener
{
	friend EMsgManager;

public:
	EKeyboardListener();

	EKeyboardListener(
		const EString &name
	);

	EKeyboardListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyboardListener(
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
class EKeyboardPressListener :
	public EKeyboardListener
{
	friend EMsgManager;

public:
	EKeyboardPressListener();

	EKeyboardPressListener(
		const EString &name
	);

	EKeyboardPressListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyboardPressListener(
		const EString &name,
		const KEY_LISTENER_CALLBACK &callback
	);

protected:
	// 执行监听器回调函数
	virtual void _callOn() override;
};

}