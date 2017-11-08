#include "..\emanagers.h"
#include "..\elisteners.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"


// 鼠标消息监听器
e2d::EVector<e2d::EListenerMouse*> s_vMouseListeners;
// 按键消息监听器
e2d::EVector<e2d::EListenerKeyboard*> s_vKeyboardListeners;


void e2d::EMsgManager::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存鼠标消息
	EMouseMsg::s_nMsg = message;
	EMouseMsg::s_wParam = wParam;
	EMouseMsg::s_lParam = lParam;

	if (s_vMouseListeners.empty()) return;

	// 执行鼠标消息监听函数
	EVector<EListenerMouse*>::size_type i = s_vMouseListeners.size();

	do
	{
		auto mlistener = s_vMouseListeners[--i];

		if (mlistener->_isReady())
		{
			mlistener->_callOn();
			if (mlistener->m_bSwallow)
				break;
		}
	} while (i != 0);
}

void e2d::EMsgManager::KeyboardProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 保存按键消息
	EKeyboardMsg::s_nMsg = message;
	EKeyboardMsg::s_wParam = wParam;
	EKeyboardMsg::s_lParam = lParam;

	if (s_vKeyboardListeners.empty()) return;

	// 执行按键消息监听函数
	EVector<EListenerMouse*>::size_type i = s_vKeyboardListeners.size();

	do
	{
		auto klistener = s_vKeyboardListeners[--i];

		if (klistener->_isReady())
		{
			klistener->_callOn();
			if (klistener->m_bSwallow)
				break;
		}
	} while (i != 0);
}

void e2d::EMsgManager::bindListener(e2d::EListenerMouse * listener, EScene * pParentScene)
{
	EMsgManager::bindListener(listener, pParentScene->getRoot());
}

void e2d::EMsgManager::bindListener(EListenerKeyboard * listener, EScene * pParentScene)
{
	EMsgManager::bindListener(listener, pParentScene->getRoot());
}

void e2d::EMsgManager::bindListener(EListenerMouse * listener, ENode * pParentNode)
{
	WARN_IF(listener == nullptr, "EListenerMouse NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind EListenerMouse with a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		ASSERT(
			!listener->m_pParentNode,
			"The listener is already binded, it cannot bind again!"
		);

		listener->start();
		listener->retain();
		listener->m_pParentNode = pParentNode;
		s_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::bindListener(EListenerKeyboard * listener, ENode * pParentNode)
{
	WARN_IF(listener == nullptr, "EListenerKeyboard NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "Bind EListenerKeyboard with a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		ASSERT(
			!listener->m_pParentNode,
			"The listener is already binded, it cannot bind again!"
		);

		listener->start();
		listener->retain();
		listener->m_pParentNode = pParentNode;
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
	EVector<EListenerMouse*>::iterator mIter;
	for (mIter = s_vMouseListeners.begin(); mIter != s_vMouseListeners.end();)
	{
		if ((*mIter)->getName() == name)
		{
			SafeRelease(&(*mIter));
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
	EVector<EListenerKeyboard*>::iterator kIter;
	for (kIter = s_vKeyboardListeners.begin(); kIter != s_vKeyboardListeners.end();)
	{
		if ((*kIter)->getName() == name)
		{
			SafeRelease(&(*kIter));
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
			SafeRelease(&t);
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
			SafeRelease(&t);
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