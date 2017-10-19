#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emsg.h"
#include "..\etools.h"
#include <algorithm>

e2d::EScene::EScene()
	: m_bWillSave(true)
	, m_bSortNeeded(false)
	, m_pRoot(new ENode())
{
	m_pRoot->_onEnter();
	m_pRoot->_setParentScene(this);
}

e2d::EScene::~EScene()
{
	SafeRelease(&m_pRoot);
	ETimerManager::clearAllTimersBindedWith(this);
	EMsgManager::clearAllMouseListenersBindedWith(this);
	EMsgManager::clearAllKeyboardListenersBindedWith(this);
	EActionManager::clearAllActionsBindedWith(this);
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
	m_pRoot->_callOn();
}

void e2d::EScene::_onEnter()
{
	// 启用场景上的所有定时器、监听器和动画
	ETimerManager::_notifyAllTimersBindedWith(this);
	EMsgManager::_notifyAllMouseListenersBindedWith(this);
	EMsgManager::_notifyAllKeyboardListenersBindedWith(this);
	EActionManager::_notifyAllActionsBindedWith(this);
}

void e2d::EScene::_onExit()
{
	if (m_bWillSave)
	{
		ETimerManager::_waitAllTimersBindedWith(this);
		EMsgManager::_waitAllMouseListenersBindedWith(this);
		EMsgManager::_waitAllKeyboardListenersBindedWith(this);
		EActionManager::_waitAllActionsBindedWith(this);
	}
}

void e2d::EScene::add(ENode * child, int order /* = 0 */)
{
	m_pRoot->addChild(child, order);
}

bool e2d::EScene::remove(ENode * child, bool release /* = false */)
{
	return m_pRoot->removeChild(child, release);
}

void e2d::EScene::remove(const EString &childName, bool release /* = false */)
{
	return m_pRoot->removeChild(childName, release);
}

e2d::EVector<e2d::ENode*>& e2d::EScene::getChildren()
{
	return m_pRoot->m_vChildren;
}

size_t e2d::EScene::getChildrenCount() const
{
	return m_pRoot->getChildrenCount();
}

e2d::ENode * e2d::EScene::getChild(const EString &childName)
{
	return m_pRoot->getChild(childName);
}

void e2d::EScene::clearAllChildren()
{
	m_pRoot->clearAllChildren();
}

void e2d::EScene::bindListener(EMouseListener * listener)
{
	EMsgManager::bindListener(listener, this);
}

void e2d::EScene::bindListener(EKeyboardListener * listener)
{
	EMsgManager::bindListener(listener, this);
}
