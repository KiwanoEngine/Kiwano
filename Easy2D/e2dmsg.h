#pragma once
#include <e2dbase.h>

namespace easy2d 
{
	class MouseMsg
	{
		friend class App;

	public:
		MouseMsg();
		MouseMsg(tstring name, const MOUSE_CALLBACK& callback);
		~MouseMsg();

		// 添加键盘监听
		static void addListener(tstring name, const MOUSE_CALLBACK& callback);
		// 删除键盘监听
		static bool delListener(tstring name);
		// 删除所有键盘监听
		static void clearAllListeners();
		// 左键是否按下
		static bool isLButtonDown();
		// 右键是否按下
		static bool isRButtonDown();
		// 中键是否按下
		static bool isMButtonDown();
		// 获取鼠标X坐标
		static int getX();
		// 获取鼠标Y坐标
		static int getY();
		// 获取鼠标坐标
		static CPoint getPos();
		// 获取鼠标滚轮值
		static int getWheel();
		// 鼠标移动消息
		static bool isOnMouseMoved();
		// 左键双击消息
		static bool isOnLButtonDBClicked();
		// 右键按下消息
		static bool isOnLButtonDown();
		// 左键弹起消息
		static bool isOnLButtonUp();
		// 右键双击消息
		static bool isOnRButtonDBClicked();
		// 右键按下消息
		static bool isOnRButtonDown();
		// 右键弹起消息
		static bool isOnRButtonUp();
		// 中键双击消息
		static bool isOnMButtonDBClicked();
		// 中键按下消息
		static bool isOnMButtonDown();
		// 中键弹起消息
		static bool isOnMButtonUp();
		// 鼠标滚轮拨动消息
		static bool isOnWheel();
		// 清空鼠标消息
		static void resetMouseMsg();

	private:
		static void __exec();

	protected:
		tstring			m_sName;
		MOUSE_CALLBACK	m_callback;

	protected:
		// 执行回调函数
		void onMouseMsg();
	};


	class KeyMsg
	{
		friend class App;

	public:
		KeyMsg(tstring name, const KEY_CALLBACK& callback);
		~KeyMsg();

		// 执行回调函数
		void onKbHit(VK_KEY key);

		// 添加键盘监听
		static void addListener(tstring name, const KEY_CALLBACK& callback);
		// 删除键盘监听
		static bool delListener(tstring name);
		// 删除所有键盘监听
		static void clearAllListeners();
		// 判断键是否被按下，按下返回true
		static bool isKeyDown(VK_KEY key);

	public:
		// 字母键值
		static const VK_KEY A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
		// 数字键值
		static const VK_KEY NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0;
		// 小数字键盘值
		static const VK_KEY NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, NUMPAD_0;
		// 控制键值
		static const VK_KEY Enter, Space, Up, Down, Left, Right, Esc, Shift, LShift, RShift, Ctrl, LCtrl, RCtrl;
		// F 键值
		static const VK_KEY F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

	private:
		static void __exec();

	protected:
		tstring			m_sName;
		KEY_CALLBACK	m_callback;
	};
}	// End of easy2d namespace