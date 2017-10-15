#pragma once
#include "ebase.h"

namespace e2d
{

class ENode;
class EMsgManager;

// 鼠标消息
class EMouseMsg
{
	friend EMsgManager;

public:
	// 鼠标消息集合
	enum MOUSE_MSG
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
	static MOUSE_MSG getMsg();

protected:
	UINT m_nMsg = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};


// 按键消息
class EKeyMsg
{
	friend EMsgManager;

public:
	// 按键消息类型集合
	enum KEYBOARD_MSG
	{
		KEY_DOWN = 0x0100,	// 按下
		KEY_UP				// 抬起
	};

	// 按键键值集合
	enum class KEY
	{
		A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,					// 字母键值
		NUM0 = '0', NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,									// 数字键值
		NUMPAD0 = 0x60, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,	// 数字小键盘键值
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,											// F键键值
		MULTIPLY,		// 乘号键键值
		ADD,			// 加号键键值
		SEPARATOR,		// 分割键键值
		SUBTRACT,		// 减号键键值
		DECIMAL,		// 小数点键键值
		DIVIDE,			// 除号键键值
		TAB = 0x09,		// TAB 键键值
		ENTER = 0x0D,	// 回车键键值
		SHIFT, CTRL, 	// SHIFT 键键值
		ESC = 0x1B, 	// ESCAPE 键键值
		SPACE = 0x20, 	// 空格键键值
		PAGE_UP, 		// PageUp 键键值
		PAGE_DOWN, 		// PageDown 键键值
		END, 			// End 键键值
		HOME, 			// Home 键键值
		LEFT, 			// 左键键值
		UP, 			// 上键键值
		RIGHT, 			// 右键键值
		DOWN			// 下键键值
	};

	// 获取按键消息类型
	static KEYBOARD_MSG getMsg();

	// 获取键值
	static KEY getVal();

	// 获取按键消息的计数
	static DWORD getCount();

	// 获取特定按键的状态
	static bool isKeyDown(
		KEY key
	);

	// 获取大小写锁定状态
	static bool isCapitalLockOn();

	// 获取数字小键盘锁定状态
	static bool isNumpadLockOn();

	// 获取滑动锁定状态
	static bool isScrollLockOn();

protected:
	UINT m_nMsg = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};


// 监听器
class EListener :
	public EObject
{
	friend EMsgManager;

public:
	EListener();

	EListener(
		EString name
	);

	// 获取监听器状态
	bool isRunning() const;

	// 获取监听器挂起状态
	bool isWaiting() const;

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

	// 获取监听器所在节点
	ENode * getParentNode() const;

	// 设置监听器名称
	void setName(
		EString name
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
	EString		m_sName;
	bool		m_bRunning;
	bool		m_bWaiting;
	EScene *	m_pParentScene;
	ENode *		m_pParentNode;
};


// 鼠标消息监听器
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
	MOUSE_LISTENER_CALLBACK m_callback;
};


// 鼠标按下消息监听器
class EMousePressListener :
	public EMouseListener
{
public:
	EMousePressListener();

	EMousePressListener(
		EString name
	);

	EMousePressListener(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	EMousePressListener(
		EString name,
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback() override;

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

protected:
	MOUSE_PRESS_LISTENER_CALLBACK m_callback;
};


// 鼠标点击消息监听器
class EMouseClickListener :
	public EMouseListener
{
public:
	EMouseClickListener();

	EMouseClickListener(
		EString name
	);

	EMouseClickListener(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	EMouseClickListener(
		EString name,
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback() override;

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

protected:
	bool m_bPressed;
	MOUSE_CLICK_LISTENER_CALLBACK m_callback;
};


// 鼠标点击消息监听器
class EMouseDoubleClickListener :
	public EMouseListener
{
public:
	EMouseDoubleClickListener();

	EMouseDoubleClickListener(
		EString name
	);

	EMouseDoubleClickListener(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	EMouseDoubleClickListener(
		EString name,
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback() override;

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

protected:
	bool m_bPressed;
	MOUSE_DBLCLK_LISTENER_CALLBACK m_callback;
};


// 鼠标拖动消息监听器
class EMouseDragListener :
	public EMouseListener
{
public:
	EMouseDragListener();

	EMouseDragListener(
		EString name
	);

	EMouseDragListener(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	EMouseDragListener(
		EString name,
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback() override;

	// 设置监听器回调函数
	void setCallback(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

protected:
	EPoint	m_Begin;
	MOUSE_DRAG_LISTENER_CALLBACK m_callback;
};


// 按键消息监听器
class EKeyboardListener :
	public EListener
{
public:
	EKeyboardListener();

	EKeyboardListener(
		EString name
	);

	EKeyboardListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyboardListener(
		EString name,
		const KEY_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback();

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
	KEY_LISTENER_CALLBACK m_callback;
};


// 按键按下消息监听
class EKeyPressListener :
	public EKeyboardListener
{
public:
	EKeyPressListener();

	EKeyPressListener(
		EString name
	);

	EKeyPressListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyPressListener(
		EString name,
		const KEY_LISTENER_CALLBACK &callback
	);

	// 执行监听器回调函数
	virtual void runCallback() override;
};


// 消息管理器
class EMsgManager
{
	friend EApp;

public:
	// 绑定鼠标消息监听器到场景
	static void bindListenerWith(
		EMouseListener * listener,
		EScene * pParentScene
	);

	// 绑定鼠标消息监听器到场景
	static void bindListenerWith(
		EKeyboardListener * listener,
		EScene * pParentScene
	);

	// 绑定按键消息监听器到节点
	static void bindListenerWith(
		EMouseListener * listener,
		ENode * pParentNode
	);

	// 绑定按键消息监听器到节点
	static void bindListenerWith(
		EKeyboardListener * listener,
		ENode * pParentNode
	);

	// 启动具有相同名称的监听器
	static void startListener(
		EString name
	);

	// 停止具有相同名称的监听器
	static void stopListener(
		EString name
	);

	// 删除具有相同名称的监听器
	static void delListener(
		EString name
	);

	// 启动所有鼠标消息监听器
	static void startAllMouseListener();

	// 停止所有鼠标消息监听器
	static void stopAllMouseListener();

	// 清除所有鼠标消息监听器
	static void clearAllMouseListeners();

	// 启动所有按键消息监听器
	static void startAllKeyboardListener();

	// 停止所有按键消息监听器
	static void stopAllKeyboardListener();

	// 清除所有按键消息监听器
	static void clearAllKeyboardListeners();

	// 启动绑定在场景上的所有鼠标消息监听器
	static void startAllMouseListenersBindWithScene(
		EScene * pParentScene
	);

	// 重启绑定在场景上的所有鼠标消息监听器
	static void stopAllMouseListenersBindWithScene(
		EScene * pParentScene
	);

	// 启动绑定在场景上的所有按键消息监听器
	static void startAllKeyboardListenersBindWithScene(
		EScene * pParentScene
	);

	// 重启绑定在场景上的所有按键消息监听器
	static void stopAllKeyboardListenersBindWithScene(
		EScene * pParentScene
	);

	// 挂起绑定在场景上的所有监听器
	static void waitAllListenersBindWithScene(
		EScene * pParentScene
	);

	// 重启绑定在场景上的所有监听器
	static void notifyAllListenersBindWithScene(
		EScene * pParentScene
	);

	// 清空绑定在场景上的所有监听器
	static void clearAllListenersBindWithScene(
		EScene * pParentScene
	);

	// 挂起绑定在节点上的所有监听器
	static void waitAllListenersBindWithNode(
		ENode * pParentNode
	);

	// 重启绑定在节点上的所有监听器
	static void notifyAllListenersBindWithNode(
		ENode * pParentNode
	);

	// 清空绑定在节点上的所有监听器
	static void clearAllListenersBindWithNode(
		ENode * pParentNode
	);

private:
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
};

}