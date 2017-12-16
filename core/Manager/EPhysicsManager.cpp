#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\elisteners.h"
#include "..\egeometry.h"

// 监听器集合
std::vector<e2d::EListenerPhysics*> s_vListeners;
// 形状集合
std::vector<e2d::EGeometry*> s_vGeometries;


void e2d::EPhysicsManager::PhysicsGeometryProc(EGeometry * pActiveGeometry)
{
	if (s_vListeners.empty() || s_vGeometries.empty() || EApp::isPaused())
		return;

	// pActiveGeometry 为主动方
	EPhysicsMsg::s_pActiveGeometry = pActiveGeometry;
	// 判断变化后的状态
	for (UINT i = 0; i < s_vGeometries.size(); i++)
	{
		auto pPassiveGeometry = s_vGeometries[i];
		// 不与其他场景的物体判断
		if (!pPassiveGeometry->getParentNode() || 
			(pPassiveGeometry->getParentNode()->getParentScene() != EApp::getCurrentScene()))
			continue;

		if (pActiveGeometry != pPassiveGeometry)
		{
			// 判断两物体是否是相互冲突的物体
			if (pActiveGeometry->m_nCollisionBitmask & pPassiveGeometry->m_nCategoryBitmask)
			{
				// pPassiveGeometry 为被动方
				EPhysicsMsg::s_pPassiveGeometry = pPassiveGeometry;
				// 获取两方的关系
				EPhysicsMsg::s_nRelation = pActiveGeometry->_intersectWith(pPassiveGeometry);
				// 如果关系不为未知或无交集，响应监听器
				if (EPhysicsMsg::s_nRelation != EPhysicsMsg::UNKNOWN &&
					EPhysicsMsg::s_nRelation != EPhysicsMsg::DISJOINT)
				{
					// 执行监听器
					PhysicsListenerProc();
				}
			}
		}
	}
}

void e2d::EPhysicsManager::PhysicsListenerProc()
{
	// 执行鼠标消息监听函数
	size_t i = s_vListeners.size();

	do
	{
		auto listener = s_vListeners[--i];

		if (listener->_isReady())
		{
			listener->_callOn();
			if (listener->m_bSwallow)
				break;
		}
	} while (i != 0);
}

void e2d::EPhysicsManager::bindListener(EListenerPhysics * listener, EScene * pParentScene)
{
	EPhysicsManager::bindListener(listener, pParentScene->getRoot());
}

void e2d::EPhysicsManager::bindListener(EListenerPhysics * listener, ENode * pParentNode)
{
	WARN_IF(listener == nullptr, "EListenerPhysics NULL pointer exception!");
	WARN_IF(pParentNode == nullptr, "EListenerPhysics add to a NULL ENode pointer!");

	if (listener && pParentNode)
	{
		ASSERT(
			!listener->m_pParentNode,
			"The listener is already binded, it cannot bind again!"
		);

		listener->retain();
		listener->start();
		listener->m_pParentNode = pParentNode;
		s_vListeners.push_back(listener);
	}
}

void e2d::EPhysicsManager::_addGeometry(EGeometry * geometry)
{
	if (geometry)
	{
		geometry->retain();
		s_vGeometries.push_back(geometry);
	}
}

void e2d::EPhysicsManager::_delGeometry(EGeometry * geometry)
{
	if (geometry)
	{
		for (UINT i = 0; i < s_vGeometries.size(); i++)
		{
			if (s_vGeometries[i] == geometry)
			{
				SafeRelease(&geometry);
				s_vGeometries.erase(s_vGeometries.begin() + i);
				return;
			}
		}
	}
}

void e2d::EPhysicsManager::startListeners(const EString & name)
{
	for (auto listener = s_vListeners.begin(); listener != s_vListeners.end(); listener++)
	{
		if ((*listener)->getName() == name)
		{
			(*listener)->start();
		}
	}
}

void e2d::EPhysicsManager::stopListeners(const EString & name)
{
	for (auto listener = s_vListeners.begin(); listener != s_vListeners.end(); listener++)
	{
		if ((*listener)->getName() == name)
		{
			(*listener)->stop();
		}
	}
}

void e2d::EPhysicsManager::delListeners(const EString & name)
{
	std::vector<EListenerPhysics*>::iterator iter;
	for (iter = s_vListeners.begin(); iter != s_vListeners.end();)
	{
		if ((*iter)->getName() == name)
		{
			SafeRelease(&(*iter));
			iter = s_vListeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void e2d::EPhysicsManager::startAllListenersBindedWith(EScene * pParentScene)
{
	EPhysicsManager::startAllListenersBindedWith(pParentScene->getRoot());
}

void e2d::EPhysicsManager::stopAllListenersBindedWith(EScene * pParentScene)
{
	EPhysicsManager::stopAllListenersBindedWith(pParentScene->getRoot());
}

void e2d::EPhysicsManager::startAllListenersBindedWith(ENode * pParentNode)
{
	for (auto listener = s_vListeners.begin(); listener != s_vListeners.end(); listener++)
	{
		if ((*listener)->getParentNode() == pParentNode)
		{
			(*listener)->start();
		}
	}
	for (auto child = pParentNode->getChildren().begin(); child != pParentNode->getChildren().end(); child++)
	{
		EPhysicsManager::startAllListenersBindedWith((*child));
	}
}

void e2d::EPhysicsManager::stopAllListenersBindedWith(ENode * pParentNode)
{
	for (auto listener = s_vListeners.begin(); listener != s_vListeners.end(); listener++)
	{
		if ((*listener)->getParentNode() == pParentNode)
		{
			(*listener)->stop();
		}
	}
	for (auto child = pParentNode->getChildren().begin(); child != pParentNode->getChildren().end(); child++)
	{
		EPhysicsManager::stopAllListenersBindedWith((*child));
	}
}

void e2d::EPhysicsManager::startAllListeners()
{
	EPhysicsManager::startAllListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EPhysicsManager::stopAllListeners()
{
	EPhysicsManager::stopAllListenersBindedWith(EApp::getCurrentScene());
}

void e2d::EPhysicsManager::_clearManager()
{
	s_vListeners.clear();
}

void e2d::EPhysicsManager::_clearAllListenersBindedWith(ENode * pParentNode)
{
	for (UINT i = 0; i < s_vListeners.size();)
	{
		auto listener = s_vListeners[i];
		if (listener->getParentNode() == pParentNode)
		{
			SafeRelease(&listener);
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			i++;
		}
	}
}
