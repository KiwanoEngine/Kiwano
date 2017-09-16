#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// 鼠标监听回调函数的容器
static std::vector<MouseMsg*> s_vMouseMsg;

// 鼠标消息
static MouseMsg s_mouseMsg = MouseMsg();

// 将 EasyX 的 MOUSEMSG 转换为 MouseMsg
static void ConvertMsg(MOUSEMSG msg);

void MouseMsg::__exec()
{
	// 获取鼠标消息
	while (MouseHit())
	{
		// 转换鼠标消息
		ConvertMsg(GetMouseMsg());
		// 执行场景程序
		App::get()->getCurrentScene()->_exec();
		// 执行鼠标监听回调函数
		for (auto m : s_vMouseMsg)		// 循环遍历所有的鼠标监听
		{
			m->onMouseMsg(s_mouseMsg);	// 执行回调函数
		}
	}
}

MouseMsg::MouseMsg()
{
}

MouseMsg::MouseMsg(tstring name, const MOUSE_CALLBACK & callback)
{
	m_sName = name;
	m_callback = callback;
}

MouseMsg::~MouseMsg()
{
}

void MouseMsg::onMouseMsg(MouseMsg mouse)
{
	m_callback(mouse);
}

void MouseMsg::addListener(tstring name, const MOUSE_CALLBACK & callback)
{
	// 创建新的监听对象
	auto mouse = new MouseMsg(name, callback);
	// 添加新的按键回调函数
	s_vMouseMsg.push_back(mouse);
}

bool MouseMsg::delListener(tstring name)
{
	// 创建迭代器
	std::vector<MouseMsg*>::iterator iter;
	// 循环遍历所有监听器
	for (iter = s_vMouseMsg.begin(); iter != s_vMouseMsg.end(); iter++)
	{
		// 查找相同名称的监听器
		if ((*iter)->m_sName == name)
		{
			// 删除该定时器
			delete (*iter);
			s_vMouseMsg.erase(iter);
			return true;
		}
	}
	// 若未找到同样名称的监听器，返回 false
	return false;
}

void MouseMsg::clearAllListener()
{
	// 删除所有监听器
	for (auto m : s_vMouseMsg)
	{
		delete m;
	}
	// 清空容器
	s_vMouseMsg.clear();
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
