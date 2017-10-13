#include "..\emsg.h"
#include "..\Win\winbase.h"
#include <vector>


// 鼠标消息
e2d::EMouseMsg mouseMsg;
// 按键消息
e2d::EKeyMsg keyMsg;
// 鼠标消息监听器
std::vector<e2d::EMouseListener*> s_vMouseListeners;
// 按键消息监听器
std::vector<e2d::EKeyboardListener*> s_vKeyListeners;


DWORD e2d::EMouseMsg::getX()
{
	return LOWORD(mouseMsg.m_lParam);
}

DWORD e2d::EMouseMsg::getY()
{
	return HIWORD(mouseMsg.m_lParam);
}

e2d::EPoint e2d::EMouseMsg::getPos()
{
	return EPoint(LOWORD(mouseMsg.m_lParam), HIWORD(mouseMsg.m_lParam));
}

bool e2d::EMouseMsg::isLButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_LBUTTON;
}

bool e2d::EMouseMsg::isMButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_MBUTTON;
}

bool e2d::EMouseMsg::isRButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_RBUTTON;
}

bool e2d::EMouseMsg::isShiftDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_SHIFT;
}

bool e2d::EMouseMsg::isCtrlDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_CONTROL;
}

DWORD e2d::EMouseMsg::getWheelDelta()
{
	return GET_WHEEL_DELTA_WPARAM(mouseMsg.m_wParam);
}

e2d::EMouseMsg::MOUSE_MSG e2d::EMouseMsg::getMsg()
{
	return MOUSE_MSG(mouseMsg.m_nMsg);
}



e2d::EKeyMsg::KEYBOARD_MSG e2d::EKeyMsg::getMsg()
{
	return KEYBOARD_MSG(keyMsg.m_nMsg);
}

e2d::EKeyMsg::KEY e2d::EKeyMsg::getVal()
{
	return KEY(keyMsg.m_wParam);
}

DWORD e2d::EKeyMsg::getCount()
{
	return (((DWORD)keyMsg.m_lParam) & 0x0000FFFF);
}

bool e2d::EKeyMsg::isKeyDown(KEY key)
{
	if (::GetAsyncKeyState((int)key) & 0x8000)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyMsg::isCapitalLockOn()
{
	if (::GetKeyState(VK_CAPITAL) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyMsg::isNumpadLockOn()
{
	if (::GetKeyState(VK_NUMLOCK) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyMsg::isScrollLockOn()
{
	if (::GetKeyState(VK_SCROLL) & 0x0001)
	{
		return true;
	}
	return false;
}



void e2d::EMsgManager::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存鼠标消息
	mouseMsg.m_nMsg = message;
	mouseMsg.m_wParam = wParam;
	mouseMsg.m_lParam = lParam;
	// 执行鼠标消息监听函数
	for (auto mlistener : s_vMouseListeners)
	{
		if (mlistener->isRunning())
		{
			mlistener->runCallback();
		}
	}
}

void e2d::EMsgManager::KeyboardProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存按键消息
	keyMsg.m_nMsg = message;
	keyMsg.m_wParam = wParam;
	keyMsg.m_lParam = lParam;
	// 执行按键消息监听函数
	for (auto klistener : s_vKeyListeners)
	{
		if (klistener->isRunning())
		{
			klistener->runCallback();
		}
	}
}

void e2d::EMsgManager::bindListenerWithScene(e2d::EMouseListener * listener, EScene * pParentScene)
{
	WARN_IF(listener == nullptr, "EMouseListener NULL pointer exception!");
	WARN_IF(pParentScene == nullptr, "Bind EMouseListener with a NULL EScene pointer!");

	if (listener && pParentScene)
	{
		listener->start();
		listener->retain();
		listener->bindWithScene(pParentScene);
		s_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::bindListenerWithScene(e2d::EKeyboardListener * listener, EScene * pParentScene)
{
	WARN_IF(listener == nullptr, "EKeyboardListener NULL pointer exception!");
	WARN_IF(pParentScene == nullptr, "Bind EKeyboardListener with a NULL EScene pointer!");

	if (listener && pParentScene)
	{
		listener->start();
		listener->retain();
		listener->bindWithScene(pParentScene);
		s_vKeyListeners.push_back(listener);
	}
}

void e2d::EMsgManager::startListener(EString name)
{
	// 启动鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getName() == name)
		{
			l->start();
		}
	}
	// 启动按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getName() == name)
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopListener(EString name)
{
	// 停止鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getName() == name)
		{
			l->stop();
		}
	}
	// 停止按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getName() == name)
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::delListener(EString name)
{
	// 删除鼠标消息监听器
	std::vector<EMouseListener*>::iterator mIter;
	for (mIter = s_vMouseListeners.begin(); mIter != s_vMouseListeners.end();)
	{
		if ((*mIter)->getName() == name)
		{
			(*mIter)->autoRelease();
			(*mIter)->release();
			mIter = s_vMouseListeners.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
	// 删除按键消息监听器
	std::vector<EKeyboardListener*>::iterator kIter;
	for (kIter = s_vKeyListeners.begin(); kIter != s_vKeyListeners.end();)
	{
		if ((*kIter)->getName() == name)
		{
			(*kIter)->autoRelease();
			(*kIter)->release();
			kIter = s_vKeyListeners.erase(kIter);
		}
		else
		{
			kIter++;
		}
	}
}

void e2d::EMsgManager::startAllMouseListener()
{
	for (auto l : s_vMouseListeners)
	{
		if (!l->isWaiting())
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopAllMouseListener()
{
	for (auto l : s_vMouseListeners)
	{
		if (!l->isWaiting())
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::clearAllMouseListeners()
{
	for (auto l : s_vMouseListeners)
	{
		l->autoRelease();
		l->release();
	}
	s_vMouseListeners.clear();
}

void e2d::EMsgManager::startAllKeyboardListener()
{
	for (auto l : s_vKeyListeners)
	{
		if (!l->isWaiting())
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopAllKeyboardListener()
{
	for (auto l : s_vKeyListeners)
	{
		if (!l->isWaiting())
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::clearAllKeyboardListeners()
{
	for (auto l : s_vKeyListeners)
	{
		l->autoRelease();
		l->release();
	}
	s_vKeyListeners.clear();
}

void e2d::EMsgManager::startAllMouseListenersBindWithScene(EScene * pParentScene)
{
	// 启动鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopAllMouseListenersBindWithScene(EScene * pParentScene)
{
	// 停止鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::startAllKeyboardListenersBindWithScene(EScene * pParentScene)
{
	// 启动按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopAllKeyboardListenersBindWithScene(EScene * pParentScene)
{
	// 停止按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::waitAllListenersBindWithScene(EScene * scene)
{
	// 挂起鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->wait();
		}
	}
	// 挂起按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->wait();
		}
	}
}

void e2d::EMsgManager::notifyAllListenersBindWithScene(EScene * scene)
{
	// 重启鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->notify();
		}
	}
	// 重启按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->notify();
		}
	}
}

void e2d::EMsgManager::clearAllListenersBindWithScene(EScene * scene)
{
	std::vector<EMouseListener*>::iterator mIter;
	for (mIter = s_vMouseListeners.begin(); mIter != s_vMouseListeners.end();)
	{
		if ((*mIter)->getParentScene() == scene)
		{
			(*mIter)->autoRelease();
			(*mIter)->release();
			mIter = s_vMouseListeners.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
	std::vector<EKeyboardListener*>::iterator kIter;
	for (kIter = s_vKeyListeners.begin(); kIter != s_vKeyListeners.end();)
	{
		if ((*kIter)->getParentScene() == scene)
		{
			(*kIter)->autoRelease();
			(*kIter)->release();
			kIter = s_vKeyListeners.erase(kIter);
		}
		else
		{
			kIter++;
		}
	}
}

void e2d::EMsgManager::waitAllListenersBindWithNode(ENode * pParentNode)
{
	// 挂起鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->wait();
		}
	}
	// 挂起按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->wait();
		}
	}
}

void e2d::EMsgManager::notifyAllListenersBindWithNode(ENode * pParentNode)
{
	// 重启鼠标消息监听器
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->notify();
		}
	}
	// 重启按键消息监听器
	for (auto l : s_vKeyListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->notify();
		}
	}
}

void e2d::EMsgManager::clearAllListenersBindWithNode(ENode * pParentNode)
{
	std::vector<EMouseListener*>::iterator mIter;
	for (mIter = s_vMouseListeners.begin(); mIter != s_vMouseListeners.end();)
	{
		if ((*mIter)->getParentNode() == pParentNode)
		{
			(*mIter)->autoRelease();
			(*mIter)->release();
			mIter = s_vMouseListeners.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
	std::vector<EKeyboardListener*>::iterator kIter;
	for (kIter = s_vKeyListeners.begin(); kIter != s_vKeyListeners.end();)
	{
		if ((*kIter)->getParentNode() == pParentNode)
		{
			(*kIter)->autoRelease();
			(*kIter)->release();
			kIter = s_vKeyListeners.erase(kIter);
		}
		else
		{
			kIter++;
		}
	}
}

