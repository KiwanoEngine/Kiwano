#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// 鼠标消息
static MouseMsg s_mouseMsg = MouseMsg();

// 将 EasyX 的 MOUSEMSG 转换为 MouseMsg
static void ConvertMsg(MOUSEMSG msg);

void easy2d::MouseMsg::__exec()
{
	// 获取鼠标消息
	while (MouseHit())
	{
		// 转换鼠标消息
		ConvertMsg(GetMouseMsg());
		// 执行场景程序
		App::get()->getCurrentScene()->_exec();
	}
}

MouseMsg MouseMsg::getMsg()
{
	return s_mouseMsg;	// 获取当前鼠标消息
}

bool MouseMsg::isLButtonDown()
{
	return s_mouseMsg.mkLButton;
}

bool MouseMsg::isRButtonDown()
{
	return s_mouseMsg.mkRButton;
}

bool MouseMsg::isMButtonDown()
{
	return s_mouseMsg.mkMButton;
}

int MouseMsg::getMouseX()
{
	return s_mouseMsg.x;
}

int MouseMsg::getMouseY()
{
	return s_mouseMsg.y;
}

int MouseMsg::getMouseWheel()
{
	return s_mouseMsg.wheel;
}

bool MouseMsg::isOnMouseMoved()
{
	return s_mouseMsg.uMsg == WM_MOUSEMOVE;
}

bool MouseMsg::isOnLButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_LBUTTONDBLCLK;
}

bool MouseMsg::isOnLButtonDown()
{
	return s_mouseMsg.uMsg == WM_LBUTTONDOWN;
}

bool MouseMsg::isOnLButtonUp()
{
	return s_mouseMsg.uMsg == WM_LBUTTONUP;
}

bool MouseMsg::isOnRButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_RBUTTONDBLCLK;
}

bool MouseMsg::isOnRButtonDown()
{
	return s_mouseMsg.uMsg == WM_RBUTTONDOWN;
}

bool MouseMsg::isOnRButtonUp()
{
	return s_mouseMsg.uMsg == WM_LBUTTONUP;
}

bool MouseMsg::isOnMButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_MBUTTONDBLCLK;
}

bool MouseMsg::isOnMButtonDown()
{
	return s_mouseMsg.uMsg == WM_MBUTTONDOWN;
}

bool MouseMsg::isOnMButtonUp()
{
	return s_mouseMsg.uMsg == WM_MBUTTONUP;
}

bool MouseMsg::isOnWheel()
{
	return s_mouseMsg.uMsg == WM_MOUSEWHEEL;
}

void MouseMsg::resetMouseMsg()
{
	s_mouseMsg.uMsg = 0;
}

void ConvertMsg(MOUSEMSG msg)
{
	// 将 MOUSEMSG 转换为 MouseMsg
	/// 虽然 MOUSEMSG 和 MouseMsg 本质上是一样的
	/// 但是为了实现 Easy2D 与 EasyX 的分离，所以定义了新的 MouseMsg
	s_mouseMsg.uMsg = msg.uMsg;
	s_mouseMsg.mkLButton = msg.mkLButton;
	s_mouseMsg.mkMButton = msg.mkMButton;
	s_mouseMsg.mkRButton = msg.mkRButton;
	s_mouseMsg.wheel = msg.wheel;
	s_mouseMsg.x = msg.x;
	s_mouseMsg.y = msg.y;
}
