#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// 鼠标监听回调函数的容器
static std::vector<MouseMsg*> s_vListeners;

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
		for (auto l : s_vListeners)	// 循环遍历所有的鼠标监听
		{
			if (!l->m_bWaiting && l->m_bRunning)
			{
				l->onMouseMsg();	// 执行回调函数
			}
		}
	}
}

MouseMsg::MouseMsg() :
	m_callback([]() {}),
	m_pParentScene(nullptr),
	m_bRunning(true),
	m_bWaiting(false)
{
}

MouseMsg::MouseMsg(TString name, const MOUSE_CALLBACK & callback) :
	m_sName(name),
	m_callback(callback),
	m_pParentScene(nullptr),
	m_bRunning(true),
	m_bWaiting(false)
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
	auto listener = new MouseMsg(name, callback);
	// 绑定在场景上
	listener->m_pParentScene = App::getLoadingScene();
	// 添加新的按键回调函数
	s_vListeners.push_back(listener);
}

void MouseMsg::startListener(TString name)
{
	// 查找名称相同的监听器
	for (auto l : s_vListeners)
	{
		if (l->m_sName == name)
		{
			l->start();
		}
	}
}

void MouseMsg::stopListener(TString name)
{
	// 查找名称相同的监听器
	for (auto l : s_vListeners)
	{
		if (l->m_sName == name)
		{
			l->stop();
		}
	}
}

void MouseMsg::delListener(TString name)
{
	// 创建迭代器
	std::vector<MouseMsg*>::iterator iter;
	// 循环遍历所有监听器
	for (iter = s_vListeners.begin(); iter != s_vListeners.end();)
	{
		// 查找相同名称的监听器
		if ((*iter)->m_sName == name)
		{
			// 删除该定时器
			delete (*iter);
			iter = s_vListeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void MouseMsg::start()
{
	m_bRunning = true;
}

void MouseMsg::stop()
{
	m_bRunning = false;
}

void MouseMsg::wait()
{
	m_bWaiting = true;
}

void MouseMsg::notify()
{
	m_bWaiting = false;
}

void MouseMsg::clearAllListeners()
{
	// 删除所有监听器
	for (auto l : s_vListeners)
	{
		delete l;
	}
	// 清空容器
	s_vListeners.clear();
}

void MouseMsg::notifyAllSceneListeners(Scene * scene)
{
	for (auto l : s_vListeners)
	{
		if (l->m_pParentScene == scene)
		{
			l->notify();
		}
	}
}

void MouseMsg::waitAllSceneListeners(Scene * scene)
{
	for (auto l : s_vListeners)
	{
		if (l->m_pParentScene == scene)
		{
			l->wait();
		}
	}
}

void MouseMsg::clearAllSceneListeners(Scene * scene)
{
	// 创建迭代器
	std::vector<MouseMsg*>::iterator iter;
	// 循环遍历所有监听器
	for (iter = s_vListeners.begin(); iter != s_vListeners.end(); iter++)
	{
		// 查找相同名称的监听器
		if ((*iter)->m_pParentScene == scene)
		{
			// 删除该定时器
			delete (*iter);
			s_vListeners.erase(iter);
		}
	}
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
