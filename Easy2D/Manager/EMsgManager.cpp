#include "..\emsg.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"


// 鼠标消息
e2d::EMouseMsg s_MouseMsg;
// 按键消息
e2d::EKeyboardMsg s_KeyboardMsg;
// 鼠标消息监听器
e2d::EVector<e2d::EMouseListener*> s_vMouseListeners;
// 按键消息监听器
e2d::EVector<e2d::EKeyboardListener*> s_vKeyboardListeners;


void e2d::EMsgManager::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存鼠标消息
	s_MouseMsg.m_nMsg = message;
	s_MouseMsg.m_wParam = wParam;
	s_MouseMsg.m_lParam = lParam;
	// 执行鼠标消息监听函数
	for (size_t i = 0; i < s_vMouseListeners.size(); i++)
	{
		auto &mlistener = s_vMouseListeners[i];

		if (EApp::isPaused() && !mlistener->m_bAlways)
			continue;

		if (mlistener->isRunning())
		{
			if (mlistener->getParentScene() == EApp::getCurrentScene() ||
				(mlistener->getParentNode() && mlistener->getParentNode()->getParentScene() == EApp::getCurrentScene()))
			{
				mlistener->_callOn();
			}
		}
	}
}

void e2d::EMsgManager::KeyboardProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存按键消息
	s_KeyboardMsg.m_nMsg = message;
	s_KeyboardMsg.m_wParam = wParam;
	s_KeyboardMsg.m_lParam = lParam;
	// 执行按键消息监听函数
	for (size_t i = 0; i < s_vKeyboardListeners.size(); i++)
	{
		auto &klistener = s_vKeyboardListeners[i];

		if (EApp::isPaused() && !klistener->m_bAlways)
			continue;

		if (klistener->isRunning())
		{
			if (klistener->getParentScene() == EApp::getCurrentScene() ||
				(klistener->getParentNode() && klistener->getParentNode()->getParentScene() == EApp::getCurrentScene()))
			{
				klistener->_callOn();
			}
		}
	}
}


DWORD e2d::EMouseMsg::getPosX()
{
	return LOWORD(s_MouseMsg.m_lParam);
}

DWORD e2d::EMouseMsg::getPosY()
{
	return HIWORD(s_MouseMsg.m_lParam);
}

e2d::EPoint e2d::EMouseMsg::getPos()
{
	return EPoint(LOWORD(s_MouseMsg.m_lParam), HIWORD(s_MouseMsg.m_lParam));
}

bool e2d::EMouseMsg::isLButtonDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_LBUTTON;
}

bool e2d::EMouseMsg::isMButtonDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_MBUTTON;
}

bool e2d::EMouseMsg::isRButtonDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_RBUTTON;
}

bool e2d::EMouseMsg::isShiftDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_SHIFT;
}

bool e2d::EMouseMsg::isCtrlDown()
{
	return GET_KEYSTATE_WPARAM(s_MouseMsg.m_wParam) == MK_CONTROL;
}

DWORD e2d::EMouseMsg::getWheelDelta()
{
	return GET_WHEEL_DELTA_WPARAM(s_MouseMsg.m_wParam);
}

e2d::EMouseMsg::MOUSE_MSG e2d::EMouseMsg::getMsg()
{
	return MOUSE_MSG(s_MouseMsg.m_nMsg);
}



e2d::EKeyboardMsg::KEYBOARD_MSG e2d::EKeyboardMsg::getMsg()
{
	return KEYBOARD_MSG(s_KeyboardMsg.m_nMsg);
}

e2d::EKeyboardMsg::KEY e2d::EKeyboardMsg::getVal()
{
	return KEY(s_KeyboardMsg.m_wParam);
}

DWORD e2d::EKeyboardMsg::getCount()
{
	return (((DWORD)s_KeyboardMsg.m_lParam) & 0x0000FFFF);
}

bool e2d::EKeyboardMsg::isKeyDown(KEY key)
{
	if (::GetAsyncKeyState((int)key) & 0x8000)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyboardMsg::isCapitalLockOn()
{
	if (::GetKeyState(VK_CAPITAL) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyboardMsg::isNumpadLockOn()
{
	if (::GetKeyState(VK_NUMLOCK) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyboardMsg::isScrollLockOn()
{
	if (::GetKeyState(VK_SCROLL) & 0x0001)
	{
		return true;
	}
	return false;
}

void e2d::EMsgManager::bindListener(e2d::EMouseListener * listener, EScene * pParentScene, bool always /* = false */)
{
	ASSERT(
		(!listener->m_pParentNode) && (!listener->m_pParentScene),
		"The listener is already binded, it cannot bind again!"
	);
	WARN_IF(listener == nullptr, "EMouseListener NULL pointer exception!");
	WARN_IF(pParentScene == nullptr, "Bind EMouseListener with a NULL EScene pointer!");

	if (listener && pParentScene)
	{
		listener->start();
		listener->retain();
		listener->m_pParentScene = pParentScene;
		listener->m_bAlways = always;
		s_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::bindListener(EKeyboardListener * listener, EScene * pParentScene, bool always /* = false */)
{
	ASSERT(
		(!listener->m_pParentNode) && (!listener->m_pParentScene),
		"The listener is already binded, it cannot bind again!"
	);
	WARN_IF(listener == nullptr, "EKeyboardListener NULL pointer exception!");
	WARN_IF(pParentScene == nullptr, "Bind EKeyboardListener with a NULL EScene pointer!");

	if (listener && pParentScene)
	{
		listener->start();
		listener->retain();
		listener->m_pParentScene = pParentScene;
		listener->m_bAlways = always;
		s_vKeyboardListeners.push_back(listener);
	}
}

void e2d::EMsgManager::bindListener(EMouseListener * listener, ENode * pParentNode, bool always /* = false */)
{
	ASSERT(
		(!listener->m_pParentNode) && (!listener->m_pParentScene),
		"The listener is already binded, it cannot bind again!"
	);
	WARN_IF(listener == nullptr, "EMouseListener NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind EMouseListener with a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		listener->start();
		listener->retain();
		listener->m_bAlways = always;
		listener->m_pParentNode = pParentNode;
		s_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::bindListener(EKeyboardListener * listener, ENode * pParentNode, bool always /* = false */)
{
	ASSERT(
		(!listener->m_pParentNode) && (!listener->m_pParentScene),
		"The listener is already binded, it cannot bind again!"
	);
	WARN_IF(listener == nullptr, "EKeyboardListener NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind EKeyboardListener with a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		listener->start();
		listener->retain();
		listener->m_pParentNode = pParentNode;
		listener->m_bAlways = always;
		s_vKeyboardListeners.push_back(listener);
	}
}

void e2d::EMsgManager::startMouseListeners(const EString & name)
{
	for (auto l : s_vMouseListeners)
	{
		if (l->getName() == name)
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopMouseListeners(const EString & name)
{
	for (auto l : s_vMouseListeners)
	{
		if (l->getName() == name)
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::delMouseListeners(const EString & name)
{
	// 删除鼠标消息监听器
	EVector<EMouseListener*>::iterator mIter;
	for (mIter = s_vMouseListeners.begin(); mIter != s_vMouseListeners.end();)
	{
		if ((*mIter)->getName() == name)
		{
			SafeReleaseAndClear(&(*mIter));
			mIter = s_vMouseListeners.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
}

void e2d::EMsgManager::startKeyboardListeners(const EString & name)
{
	// 启动按键消息监听器
	for (auto l : s_vKeyboardListeners)
	{
		if (l->getName() == name)
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopKeyboardListeners(const EString & name)
{
	// 停止按键消息监听器
	for (auto l : s_vKeyboardListeners)
	{
		if (l->getName() == name)
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::delKeyboardListeners(const EString & name)
{
	// 删除按键消息监听器
	EVector<EKeyboardListener*>::iterator kIter;
	for (kIter = s_vKeyboardListeners.begin(); kIter != s_vKeyboardListeners.end();)
	{
		if ((*kIter)->getName() == name)
		{
			SafeReleaseAndClear(&(*kIter));
			kIter = s_vKeyboardListeners.erase(kIter);
		}
		else
		{
			kIter++;
		}
	}
}

void e2d::EMsgManager::startAllMouseListenersBindedWith(EScene * pParentScene)
{
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->start();
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		EMsgManager::startAllMouseListenersBindedWith(child);
	}
}

void e2d::EMsgManager::stopAllMouseListenersBindedWith(EScene * pParentScene)
{
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->stop();
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		EMsgManager::stopAllMouseListenersBindedWith(child);
	}
}

void e2d::EMsgManager::startAllMouseListenersBindedWith(ENode * pParentNode)
{
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->start();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		EMsgManager::startAllMouseListenersBindedWith(child);
	}
}

void e2d::EMsgManager::stopAllMouseListenersBindedWith(ENode * pParentNode)
{
	for (auto l : s_vMouseListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->stop();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		EMsgManager::stopAllMouseListenersBindedWith(child);
	}
}

void e2d::EMsgManager::startAllKeyboardListenersBindedWith(EScene * pParentScene)
{
	for (auto l : s_vKeyboardListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->start();
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		EMsgManager::startAllKeyboardListenersBindedWith(child);
	}
}

void e2d::EMsgManager::stopAllKeyboardListenersBindedWith(EScene * pParentScene)
{
	for (auto l : s_vKeyboardListeners)
	{
		if (l->getParentScene() == pParentScene)
		{
			l->stop();
		}
	}
	for (auto child : pParentScene->getChildren())
	{
		EMsgManager::stopAllKeyboardListenersBindedWith(child);
	}
}

void e2d::EMsgManager::startAllKeyboardListenersBindedWith(ENode * pParentNode)
{
	for (auto l : s_vKeyboardListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->start();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		EMsgManager::startAllKeyboardListenersBindedWith(child);
	}
}

void e2d::EMsgManager::stopAllKeyboardListenersBindedWith(ENode * pParentNode)
{
	for (auto l : s_vKeyboardListeners)
	{
		if (l->getParentNode() == pParentNode)
		{
			l->stop();
		}
	}
	for (auto child : pParentNode->getChildren())
	{
		EMsgManager::stopAllKeyboardListenersBindedWith(child);
	}
}

void e2d::EMsgManager::_clearAllMouseListenersBindedWith(EScene * pParentScene)
{
	for (size_t i = 0; i < s_vMouseListeners.size();)
	{
		auto t = s_vMouseListeners[i];
		if (t->getParentScene() == pParentScene)
		{
			SafeReleaseAndClear(&t);
			s_vMouseListeners.erase(s_vMouseListeners.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void e2d::EMsgManager::_clearAllKeyboardListenersBindedWith(EScene * pParentScene)
{
	for (size_t i = 0; i < s_vKeyboardListeners.size();)
	{
		auto t = s_vKeyboardListeners[i];
		if (t->getParentScene() == pParentScene)
		{
			SafeReleaseAndClear(&t);
			s_vKeyboardListeners.erase(s_vKeyboardListeners.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void e2d::EMsgManager::_clearAllMouseListenersBindedWith(ENode * pParentNode)
{
	for (size_t i = 0; i < s_vMouseListeners.size();)
	{
		auto t = s_vMouseListeners[i];
		if (t->getParentNode() == pParentNode)
		{
			SafeReleaseAndClear(&t);
			s_vMouseListeners.erase(s_vMouseListeners.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void e2d::EMsgManager::_clearAllKeyboardListenersBindedWith(ENode * pParentNode)
{
	for (size_t i = 0; i < s_vKeyboardListeners.size();)
	{
		auto t = s_vKeyboardListeners[i];
		if (t->getParentNode() == pParentNode)
		{
			SafeReleaseAndClear(&t);
			s_vKeyboardListeners.erase(s_vKeyboardListeners.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void e2d::EMsgManager::_clearManager()
{
	s_vMouseListeners.clear();
	s_vKeyboardListeners.clear();
}

void e2d::EMsgManager::startAllMouseListeners()
{
	EMsgManager::startAllMouseListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EMsgManager::stopAllMouseListeners()
{
	EMsgManager::stopAllMouseListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EMsgManager::startAllKeyboardListeners()
{
	EMsgManager::startAllKeyboardListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EMsgManager::stopAllKeyboardListeners()
{
	EMsgManager::stopAllKeyboardListenersBindedWith(EApp::getCurrentScene());
}