#pragma once
#include "ebase.h"

namespace e2d
{

class EObjectManager
{
public:
	// 将一个节点放入释放池
	static void add(
		e2d::EObject * nptr
	);

	// 删除所有节点
	static void clearAllObjects();

private:
	friend EApp;

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
protected:
	friend EMsgManager;
	e2d::EString name;

};


class EMsgManager
{
public:
	static void setMouseMsg(
		UINT message
	);

	static e2d::EMouseMsg::MESSAGE getMouseMsg();

	static void addListener(
		EMouseListener * listener
	);

private:
	friend EApp;

	static void __exec();
};

}