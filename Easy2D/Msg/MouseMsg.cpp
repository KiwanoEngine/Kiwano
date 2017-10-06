#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// 鼠标监听回调函数的容器
static std::vector<MouseMsg*> s_vMouseMsg;

// 鼠标消息
static MOUSEMSG s_mouseMsg;

void MouseMsg::__exec()
{
	// 获取鼠标消息
	while (MouseHit())
	{
		// 获取鼠标消息
		s_mouseMsg = GetMouseMsg();
		// 执行场景程序
		App::get()->getCurrentScene()->_exec();
		// 执行鼠标监听回调函数
		for (auto m : s_vMouseMsg)	// 循环遍历所有的鼠标监听
		{
			m->onMouseMsg();		// 执行回调函数
		}
	}
}

MouseMsg::MouseMsg() :
	m_callback([]() {}),
	m_pParentScene(nullptr)
{
}

MouseMsg::MouseMsg(TString name, const MOUSE_CALLBACK & callback) :
	m_sName(name),
	m_callback(callback),
	m_pParentScene(nullptr)
{
}

MouseMsg::~MouseMsg()
{
}

void MouseMsg::onMouseMsg()
{
	m_callback();
}

void MouseMsg::addListener(TString name, const MOUSE_CALLBACK & callback)
{
	// 创建新的监听对象
	auto mouse = new MouseMsg(name, callback);
	mouse->m_pParentScene = App::getCurrentScene();
	// 添加新的按键回调函数
	s_vMouseMsg.push_back(mouse);
}

bool MouseMsg::delListener(TString name)
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

void MouseMsg::bindListenersWithScene(Scene * scene)
{
	for (auto m : s_vMouseMsg)
	{
		if (!m->m_pParentScene)
		{
			m->m_pParentScene = App::getCurrentScene();
		}
	}
}

void MouseMsg::clearAllSceneListeners(Scene * scene)
{
	// 创建迭代器
	std::vector<MouseMsg*>::iterator iter;
	// 循环遍历所有监听器
	for (iter = s_vMouseMsg.begin(); iter != s_vMouseMsg.end(); iter++)
	{
		// 查找相同名称的监听器
		if ((*iter)->m_pParentScene == scene)
		{
			// 删除该定时器
			delete (*iter);
			s_vMouseMsg.erase(iter);
		}
	}
}

void MouseMsg::clearAllListeners()
{
	// 删除所有监听器
	for (auto m : s_vMouseMsg)
	{
		delete m;
	}
	// 清空容器
	s_vMouseMsg.clear();
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

int MouseMsg::getX()
{
	return s_mouseMsg.x;
}

int MouseMsg::getY()
{
	return s_mouseMsg.y;
}

CPoint MouseMsg::getPos()
{
	return CPoint(s_mouseMsg.x, s_mouseMsg.y);
}

int MouseMsg::getWheel()
{
	return s_mouseMsg.wheel;
}

MouseMsg::MESSAGE MouseMsg::getMsg()
{
	return MESSAGE(s_mouseMsg.uMsg);
}
