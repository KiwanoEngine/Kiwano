#pragma once
#include "ebase.h"

namespace e2d
{

class EMsgManager;


class EObjectManager
{
	friend EApp;

public:
	// 将一个节点放入释放池
	static void add(
		e2d::EObject * nptr
	);

	// 删除所有节点
	static void clearAllObjects();

private:
	// 刷新内存池
	static void __flush();
};


class EMouseMsg
{
	friend EMsgManager;

public:
	// 鼠标消息集合
	enum MSG
	{
		MOVE = 0x0200,	// 鼠标移动
		LBUTTON_DOWN,	// 鼠标左键按下
		LBUTTON_UP,		// 鼠标左键抬起
		LBUTTON_DBLCLK,	// 鼠标左键双击
		RBUTTON_DOWN,	// 鼠标右键按下
		RBUTTON_UP,		// 鼠标右键抬起
		RBUTTON_DBLCLK,	// 鼠标右键双击
		MBUTTON_DOWN,	// 鼠标中键按下
		MBUTTON_UP,		// 鼠标中键抬起
		MBUTTON_DBLCLK,	// 鼠标中键双击
		WHEEL			// 滑动滚轮
	};

	// 获取鼠标横坐标
	static DWORD getX();

	// 获取鼠标纵坐标
	static DWORD getY();

	// 获取鼠标坐标
	static EPoint getPos();

	// 获取鼠标左键按下状态
	static bool isLButtonDown();

	// 获取鼠标中键按下状态
	static bool isMButtonDown();

	// 获取鼠标右键按下状态
	static bool isRButtonDown();

	// 获取 Shift 按键状态
	static bool isShiftDown();

	// 获取 Ctrl 按键状态
	static bool isCtrlDown();

	// 获取鼠标滚轮值
	static DWORD getWheelDelta();

	// 获取当前鼠标消息
	static MSG getMsg();

protected:
	UINT m_nMsg = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};


class EListener :
	public EObject
{
public:
	EListener();

	EListener(
		EString name
	);

	// 获取监听器状态
	bool isRunning() const;

	// 启动监听
	void start();

	// 停止监听
	void stop();

	// 进入等待状态
	void wait();

	// 唤醒
	void notify();

	// 获取监听器名称
	EString getName() const;

	// 获取监听器所在场景
	EScene * getParentScene() const;

	// 设置监听器名称
	void setName(
		EString name
	);

	// 设置监听器所在场景
	void setParentScene(
		EScene * scene
	);

protected:
	EString				m_sName;
	bool				m_bRunning;
	bool				m_bWaiting;
	EScene *			m_pParentScene;
};


class EMouseListener :
	public EListener
{
public:
	EMouseListener();

	EMouseListener(
		EString name
	);

	EMouseListener(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EMouseListener(
		EString name,
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback();

	// 设置监听器回调函数
	void setCallback(const MOUSE_LISTENER_CALLBACK &callback);

protected:
	MOUSE_LISTENER_CALLBACK m_callback;
};


class EMouseClickListener :
	public EMouseListener
{
public:
	EMouseClickListener();

	EMouseClickListener(
		EString name
	);

	EMouseClickListener(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EMouseClickListener(
		EString name,
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback() override;
};


class EMouseDraggedListener :
	public EMouseListener
{
public:
	EMouseDraggedListener();

	EMouseDraggedListener(
		EString name
		);

	EMouseDraggedListener(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
		);

	EMouseDraggedListener(
		EString name,
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
		);

	// 执行监听器回调函数
	virtual void runCallback() override;

	// 设置监听器回调函数
	void setCallback(const MOUSE_DRAG_LISTENER_CALLBACK &callback);

protected:
	EPoint m_Begin;
	EPoint m_End;
	MOUSE_DRAG_LISTENER_CALLBACK m_callback;
};


class EKeyMsg
{
public:
	// 鼠标消息集合
	enum MESSAGE
	{
		MOVE = 0x0200,	// 鼠标移动
		LBUTTON_DOWN,	// 鼠标左键按下
		LBUTTON_UP,		// 鼠标左键抬起
		LBUTTON_DBLCLK,	// 鼠标左键双击
		RBUTTON_DOWN,	// 鼠标右键按下
		RBUTTON_UP,		// 鼠标右键抬起
		RBUTTON_DBLCLK,	// 鼠标右键双击
		MBUTTON_DOWN,	// 鼠标中键按下
		MBUTTON_UP,		// 鼠标中键抬起
		MBUTTON_DBLCLK,	// 鼠标中键双击
		WHEEL			// 滑动滚轮
	};
};


class EKeyListener :
	public EListener
{
public:
	EKeyListener();

	EKeyListener(
		EString name
		);

	EKeyListener(
		const KEY_LISTENER_CALLBACK &callback
		);

	EKeyListener(
		EString name,
		const KEY_LISTENER_CALLBACK &callback
		);

	// 执行监听器回调函数
	void runCallback();

	// 设置监听器回调函数
	void setCallback(const KEY_LISTENER_CALLBACK &callback);

protected:
	KEY_LISTENER_CALLBACK	m_callback;
};


class EMsgManager
{
	friend EApp;

public:
	static void MouseProc(
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	);

	static void KeyboardProc(
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	);

	static void addListener(
		EMouseListener * listener
	);

private:
	
	static void __exec();
};

}