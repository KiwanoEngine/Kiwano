#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emsg.h"
#include <algorithm>

e2d::EScene::EScene()
	: m_bWillSave(true)
	, m_bSortNeeded(false)
{
}

e2d::EScene::~EScene()
{
	clearAllChildren();
}

void e2d::EScene::_onRender()
{
	this->_sortChildren();

	// 访问所有节点
	for (auto child : m_vChildren)
	{
		child->callOn();
	}
}

void e2d::EScene::_sortChildren()
{
	if (m_bSortNeeded)
	{
		m_bSortNeeded = false;

		// 子节点排序
		std::sort(
			std::begin(m_vChildren),
			std::end(m_vChildren),
			[](ENode * n1, ENode * n2) {
				return n1->getOrder() < n2->getOrder();
			}
		);
	}
}

void e2d::EScene::onEnter()
{
	// 启用场景上的所有定时器、监听器和动画
	//Timer::notifyAllSceneTimers(m_pNextScene);
	EMsgManager::notifyAllListenersBindWithScene(this);
	//ActionManager::notifyAllSceneActions(m_pNextScene);
}

void e2d::EScene::onExit()
{
	if (m_bWillSave)
	{
		//Timer::waitAllSceneTimers(m_pCurrentScene);
		EMsgManager::waitAllListenersBindWithScene(this);
		//ActionManager::waitAllSceneActions(m_pCurrentScene);
	}
	else
	{
		//Timer::clearAllSceneTimers(m_pCurrentScene);
		EMsgManager::clearAllListenersBindWithScene(this);
		//ActionManager::stopAllSceneActions(m_pCurrentScene);
	}
}

void e2d::EScene::add(ENode * child, int order /* = 0 */)
{
	ASSERT(child != nullptr, "Scene::add NULL pointer exception.");
	ASSERT(child->getParentScene() == nullptr, "Child already added. It can't be added again!");

	if (child)
	{
		child->setParentScene(this);

		child->setOrder(order);

		child->retain();

		m_vChildren.push_back(child);

		m_bSortNeeded = true;
	}
}

bool e2d::EScene::remove(ENode * child, bool autoRelease /* = true */)
{
	if (child == nullptr) return false;

	// 寻找是否有相同节点
	std::vector<ENode*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// 找到相同节点
		if (*iter == child)
		{
			if (autoRelease) 
				(*iter)->autoRelease();
			// 对象的引用计数减一
			(*iter)->release();
			// 去掉该节点
			m_vChildren.erase(iter);
			return true;
		}
	}
	// 未找到该节点返回 false
	return false;
}

std::vector<e2d::ENode*>& e2d::EScene::getChildren()
{
	return m_vChildren;
}

size_t e2d::EScene::getChildrenCount() const
{
	return m_vChildren.size();
}

e2d::ENode * e2d::EScene::getChild(EString childName) const
{
	return ENode::getChild(childName, m_vChildren);
}

void e2d::EScene::clearAllChildren()
{
	// 所有节点的引用计数减一
	for (auto child : m_vChildren)
	{
		child->autoRelease();
		child->release();
	}
	// 清空储存节点的容器
	m_vChildren.clear();
}

void e2d::EScene::bindListener(EMouseListener * listener)
{
	EMsgManager::bindListenerWith(listener, this);
}

void e2d::EScene::bindListener(EKeyboardListener * listener)
{
	EMsgManager::bindListenerWith(listener, this);
}
