#pragma once
#include "e2dcommon.h"

namespace e2d
{


// 按键消息
class KeyEvent
{
public:
	// 按键消息类型
	enum class Type : int
	{
		Down = 0x0100,	// 按下
		Up				// 抬起
	};

public:
	explicit KeyEvent(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	// 获取按键键值
	KeyCode code() const;

	// 获取按键次数
	int count() const;

	// 获取事件类型
	KeyEvent::Type type() const;

	// VK 键值转换
	static KeyCode convertKeyCode(
		WPARAM wParam
	);

protected:
	int _count;
	KeyCode _code;
	KeyEvent::Type _type;
};


// 鼠标消息
class MouseEvent
{
public:
	// 鼠标消息类型
	enum class Type : int
	{
		Move = 0x0200,		// 鼠标移动
		LeftDown,			// 鼠标左键按下
		LeftUp,				// 鼠标左键抬起
		LeftDoubleClick,	// 鼠标左键双击
		RightDown,			// 鼠标右键按下
		RightUp,			// 鼠标右键抬起
		RightDoubleClick,	// 鼠标右键双击
		MiddleDown,			// 鼠标中键按下
		MiddleUp,			// 鼠标中键抬起
		MiddleDoubleClick,	// 鼠标中键双击
		Wheel				// 滑动滚轮
	};

public:
	explicit MouseEvent(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		float dpi
	);

	// 获取鼠标横坐标
	float positionX() const;

	// 获取鼠标纵坐标
	float positionY() const;

	// 获取鼠标坐标
	Point position() const;

	// 获取事件类型
	MouseEvent::Type type() const;

	float wheelDelta() const;

	// 鼠标左键是否按下
	bool isLButtonDown() const;

	// 鼠标右键是否按下
	bool isRButtonDown() const;

	// 鼠标中键是否按下
	bool isMButtonDown() const;

	// Shift 键是否按下
	bool isShiftDown() const;

	// Ctrl 键是否按下
	bool isCtrlDown() const;

protected:
	UINT _message;
	WPARAM _wParam;
	LPARAM _lParam;
	Point _pos;
	MouseEvent::Type _type;
};


// 碰撞事件
class Collision
{
public:
	Collision();

	explicit Collision(
		Node* node,
		Collider::Relation relation
	);

	~Collision();

	// 获取发生碰撞节点
	Node* node() const;

	// 获取交集关系
	Collider::Relation relation() const;

protected:
	Node * _node;
	Collider::Relation _relation;
};

}