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

public:
	EPoint m_Pos;
	UINT m_nMsg = 0;
	bool m_bLButtonDown = false;
	bool m_bRButtonDown = false;
	bool m_bMButtonDown = false;
};


class EMouseListener :
	public EObject
{
	friend EMsgManager;

public:
	EMouseListener();

	EMouseListener(
		EString name
	);

	EMouseListener(
		const MOUSE_CALLBACK &callback
	);

	EMouseListener(
		EString name,
		const MOUSE_CALLBACK &callback
	);

	// 获取监听器状态
	bool isRunnint() const;

	// 启动监听
	void start();

	// 停止监听
	void stop();

	// 进入等待状态
	void wait();

	// 唤醒
	void notify();

	// 执行监听器回调函数
	void runCallback();

	// 获取监听器名称
	EString getName() const;

	// 获取监听器所在场景
	EScene * getParentScene();
	
	// 设置监听器名称
	void setName(EString name);

	// 设置监听器回调函数
	void setCallback(const MOUSE_CALLBACK &callback);

protected:
	EString			m_sName;
	bool			m_bRunning;
	bool			m_bWaiting;
	MOUSE_CALLBACK	m_callback;
	EScene *		m_pParentScene;
};


class EMsgManager
{
	friend EApp;

public:
	static void setMouseMsg(
		UINT message
	);

	static e2d::EMouseMsg::MESSAGE getMouseMsg();

	static void addListener(
		EMouseListener * listener
	);

private:
	
	static void __exec();
};

}