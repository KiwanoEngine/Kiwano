#include "..\emanagers.h"
#include "..\elisteners.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"


// 鼠标消息监听器
e2d::EVector<e2d::EMouseListener*> s_vMouseListeners;
// 按键消息监听器
e2d::EVector<e2d::EKeyboardListener*> s_vKeyboardListeners;


void e2d::EMsgManager::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存鼠标消息
	EMouseMsg::getMouseMsg().m_nMsg = message;
	EMouseMsg::getMouseMsg().m_wParam = wParam;
	EMouseMsg::getMouseMsg().m_lParam = lParam;
	// 执行鼠标消息监听函数
	for (size_t i = 0; i < s_vMouseListeners.size(); i++)
	{
		auto &mlistener = s_vMouseListeners[i];

		if (EApp::isPaused() && !mlistener->m_bAlways)
			continue;

		if (mlistener->isRunning())
		{
			if (mlistener->getParentNode() && 
				mlistener->getParentNode()->getParentScene() == EApp::getCurrentScene())
			{
				mlistener->_callOn();
			}
		}
	}
}

void e2d::EMsgManager::KeyboardProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存按键消息
	EKeyboardMsg::getKeyboardMsg().m_nMsg = message;
	EKeyboardMsg::getKeyboardMsg().m_wParam = wParam;
	EKeyboardMsg::getKeyboardMsg().m_lParam = lParam;
	// 执行按键消息监听函数
	for (size_t i = 0; i < s_vKeyboardListeners.size(); i++)
	{
		auto &klistener = s_vKeyboardListeners[i];

		if (EApp::isPaused() && !klistener->m_bAlways)
			continue;

		if (klistener->isRunning())
		{
			if (klistener->getParentNode() && 
				klistener->getParentNode()->getParentScene() == EApp::getCurrentScene())
			{
				klistener->_callOn();
			}
		}
	}
}

void e2d::EMsgManager::bindListener(e2d::EMouseListener * listener, EScene * pParentScene, bool always /* = false */)
{
	EMsgManager::bindListener(listener, pParentScene->getRoot(), always);
}

void e2d::EMsgManager::bindListener(EKeyboardListener * listener, EScene * pParentScene, bool always /* = false */)
{
	EMsgManager::bindListener(listener, pParentScene->getRoot(), always);
}

void e2d::EMsgManager::bindListener(EMouseListener * listener, ENode * pParentNode, bool always /* = false */)
{
	WARN_IF(listener == nullptr, "EMouseListener NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind EMouseListener with a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		ASSERT(
			!listener->m_pParentNode,
			"The listener is already binded, it cannot bind again!"
		);

		listener->start();
		listener->retain();
		listener->m_bAlways = always;
		listener->m_pParentNode = pParentNode;
		s_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::bindListener(EKeyboardListener * listener, ENode * pParentNode, bool always /* = false */)
{
	WARN_IF(listener == nullptr, "EKeyboardListener NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind EKeyboardListener with a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		ASSERT(
			!listener->m_pParentNode,
			"The listener is already binded, it cannot bind again!"
		);

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
	EMsgManager::startAllMouseListenersBindedWith(pParentScene->getRoot());
}

void e2d::EMsgManager::stopAllMouseListenersBindedWith(EScene * pParentScene)
{
	EMsgManager::stopAllMouseListenersBindedWith(pParentScene->getRoot());
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
	EMsgManager::startAllKeyboardListenersBindedWith(pParentScene->getRoot());
}

void e2d::EMsgManager::stopAllKeyboardListenersBindedWith(EScene * pParentScene)
{
	EMsgManager::stopAllKeyboardListenersBindedWith(pParentScene->getRoot());
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