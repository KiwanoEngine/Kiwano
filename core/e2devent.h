#pragma once
#include "e2dutil.h"

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
			WPARAM w_param,
			LPARAM l_param
		);

		// 获取按键键值
		KeyCode GetCode() const;

		// 获取按键次数
		int GetCount() const;

		// 获取事件类型
		KeyEvent::Type GetType() const;

		// VK 键值转换
		static KeyCode ToKeyCode(
			WPARAM w_param
		);

	protected:
		int				count_;
		KeyCode			code_;
		KeyEvent::Type	type_;
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
			WPARAM w_param,
			LPARAM l_param,
			float dpi
		);

		// 获取鼠标横坐标
		float GetX() const;

		// 获取鼠标纵坐标
		float GetY() const;

		// 获取鼠标坐标
		Point GetPos() const;

		// 获取事件类型
		MouseEvent::Type GetType() const;

		float GetWheelDelta() const;

		// 鼠标左键是否按下
		bool IsLButtonDown() const;

		// 鼠标右键是否按下
		bool IsRButtonDown() const;

		// 鼠标中键是否按下
		bool IsMButtonDown() const;

		// Shift 键是否按下
		bool IsShiftDown() const;

		// Ctrl 键是否按下
		bool IsCtrlDown() const;

	protected:
		UINT	message_;
		WPARAM	w_param_;
		LPARAM	l_param_;
		Point	pos_;
		MouseEvent::Type type_;
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
		Node* GetNode() const;

		// 获取交集关系
		Collider::Relation GetRelation() const;

	protected:
		Node * node_;
		Collider::Relation relation_;
	};

}