#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emsg.h"
#include "..\etools.h"
#include <algorithm>

e2d::EScene::EScene()
	: m_bWillSave(true)
	, m_bSortNeeded(false)
	, m_Root(new ENode())
{
	m_Root->_onEnter();
	m_Root->_setParentScene(this);
}

e2d::EScene::~EScene()
{
	m_Root->autoRelease();
}

void e2d::EScene::onEnter()
{
}

void e2d::EScene::onExit()
{
}

void e2d::EScene::onActivate()
{
}

bool e2d::EScene::onInactive()
{
	return true;
}

bool e2d::EScene::onCloseWindow()
{
	return true;
}

void e2d::EScene::_onRender()
{
	m_Root->_callOn();
}

void e2d::EScene::_onEnter()
{
	// 启用场景上的所有定时器、监听器和动画
	ETimerManager::_notifyAllTimersBindedWith(this);
	EMsgManager::_notifyAllMouseListenersBindedWith(this);
	EMsgManager::_notifyAllKeyboardListenersBindedWith(this);
	//ActionManager::notifyAllSceneActions(m_pNextScene);
}

void e2d::EScene::_onExit()
{
	if (m_bWillSave)
	{
		ETimerManager::_waitAllTimersBindedWith(this);
		EMsgManager::_waitAllMouseListenersBindedWith(this);
		EMsgManager::_waitAllKeyboardListenersBindedWith(this);
		//ActionManager::waitAllSceneActions(m_pCurrentScene);
	}
	else
	{
		ETimerManager::clearAllTimersBindedWith(this);
		EMsgManager::clearAllMouseListenersBindedWith(this);
		EMsgManager::clearAllKeyboardListenersBindedWith(this);
		//ActionManager::stopAllSceneActions(m_pCurrentScene);
	}
}

void e2d::EScene::add(ENode * child, int order /* = 0 */)
{
	m_Root->addChild(child, order);
}

bool e2d::EScene::remove(ENode * child, bool release /* = false */)
{
	return m_Root->removeChild(child, release);
}

void e2d::EScene::remove(const EString &childName, bool release /* = false */)
{
	return m_Root->removeChild(childName, release);
}

e2d::EVector<e2d::ENode*>& e2d::EScene::getChildren()
{
	return m_Root->m_vChildren;
}

size_t e2d::EScene::getChildrenCount() const
{
	return m_Root->getChildrenCount();
}

e2d::ENode * e2d::EScene::getChild(const EString &childName)
{
	return m_Root->getChild(childName);
}

void e2d::EScene::clearAllChildren()
{
	m_Root->clearAllChildren();
}

void e2d::EScene::bindListener(EMouseListener * listener)
{
	EMsgManager::bindListener(listener, this);
}

void e2d::EScene::bindListener(EKeyboardListener * listener)
{
	EMsgManager::bindListener(listener, this);
}
