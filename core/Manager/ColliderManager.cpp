#include "..\emanager.h"
#include "..\enode.h"
#include "..\ecollider.h"
#include "..\etool.h"

// 碰撞体集合
static std::vector<e2d::Collider*> s_vColliders;
// 监听器容器
static std::vector<e2d::CollisionListener*> s_vListeners;
// 碰撞触发状态
static bool s_bCollisionEnable = false;
// 发生碰撞的节点
static e2d::Node * s_pActiveNode = nullptr;
static e2d::Node * s_pPassiveNode = nullptr;


void e2d::ColliderManager::setEnable(bool bEnable)
{
	s_bCollisionEnable = bEnable;
}

void e2d::ColliderManager::__update()
{
	if (s_vListeners.size() == 0)
		return;

	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto pListener = s_vListeners[i];
		// 更新监听器
		if (pListener->m_bClear)
		{
			pListener->release();
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else if (pListener->isRunning())
		{
			pListener->_update();
		}
	}
}

void e2d::ColliderManager::__updateCollider(e2d::Collider * pActiveCollider)
{
	// 判断碰撞触发是否打开
	if (!s_bCollisionEnable)
		return;

	Node* pActiveNode = pActiveCollider->m_pParentNode;
	if (pActiveNode)
	{
		// 获取节点所在场景
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// 判断与其他碰撞体的交集情况
		for (size_t i = 0; i < s_vColliders.size(); i++)
		{
			auto pPassiveCollider = s_vColliders[i];
			// 判断两个碰撞体是否是同一个对象
			if (pActiveCollider == pPassiveCollider)
				continue;

			// 获取被碰撞节点
			Node* pPassiveNode = pPassiveCollider->m_pParentNode;
			// 判断两节点是否处于同一场景中
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				// 判断两物体是否是相互冲突的物体
				auto IsCollideWith = [](Node * active, unsigned int hash) -> bool
				{
					FOR_LOOP(collider, active->m_vColliders)
						if (collider == hash)
							return true;
					return false;
				};

				if (IsCollideWith(pActiveNode, pPassiveNode->getHashName()))
				{
					// 判断两碰撞体交集情况
					int relation = pActiveCollider->getRelationWith(pPassiveCollider);
					// 忽略 UNKNOWN 和 DISJOINT 情况
					if (relation != Relation::UNKNOWN && relation != Relation::DISJOINT)
					{
						s_pActiveNode = pActiveNode;
						s_pPassiveNode = pPassiveNode;
						pActiveNode->onCollide(pPassiveNode);
						pPassiveNode->onCollide(pActiveNode);
						pCurrentScene->onCollide(pActiveNode, pPassiveNode);
						ColliderManager::__update();
					}
				}
			}
		}
	}
	s_pActiveNode = nullptr;
	s_pPassiveNode = nullptr;
}

void e2d::ColliderManager::__add(CollisionListener * pListener)
{
	WARN_IF(pListener == nullptr, "CollisionListener NULL pointer exception!");

	if (pListener)
	{
		auto findListener = [](CollisionListener * pListener) -> bool
		{
			FOR_LOOP(l, s_vListeners)
			{
				if (pListener == l)
				{
					return true;
				}
			}
			return false;
		};

		bool bHasListener = findListener(pListener);
		WARN_IF(bHasListener, "The CollisionListener is already added, cannot be added again!");

		if (!bHasListener)
		{
			pListener->retain();
			s_vListeners.push_back(pListener);
		}
	}
}

void e2d::ColliderManager::add(Function func, String name)
{
	(new CollisionListener(func, name))->start();
}

void e2d::ColliderManager::start(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->start();
		}
	}
}

void e2d::ColliderManager::stop(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->stop();
		}
	}
}

void e2d::ColliderManager::clear(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->stopAndClear();
		}
	}
}

void e2d::ColliderManager::startAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->start();
	}
}

void e2d::ColliderManager::stopAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->stop();
	}
}

void e2d::ColliderManager::clearAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->stopAndClear();
	}
}

std::vector<e2d::CollisionListener*> e2d::ColliderManager::get(String name)
{
	std::vector<CollisionListener*> vListeners;
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			vListeners.push_back(pListener);
		}
	}
	return std::move(vListeners);
}

std::vector<e2d::CollisionListener*> e2d::ColliderManager::getAll()
{
	return s_vListeners;
}

e2d::Node * e2d::ColliderManager::getActiveNode()
{
	return s_pActiveNode;
}

e2d::Node * e2d::ColliderManager::getPassiveNode()
{
	return s_pPassiveNode;
}

e2d::Node* e2d::ColliderManager::isCausedBy(Node * pNode)
{
	if (s_pActiveNode == pNode)
		return s_pPassiveNode;
	if (s_pPassiveNode == pNode)
		return s_pActiveNode;
	return nullptr;
}

e2d::Node* e2d::ColliderManager::isCausedBy(String name)
{
	if (s_pActiveNode->getName() == name)
		return s_pActiveNode;
	if (s_pPassiveNode->getName() == name)
		return s_pPassiveNode;
	return nullptr;
}

void e2d::ColliderManager::__addCollider(Collider * pCollider)
{
	if (pCollider)
	{
		if (pCollider->m_pParentNode)
		{
			WARN_IF(true, "ColliderManager::__add Failed! The shape is already added.");
			return;
		}
		pCollider->retain();
		s_vColliders.push_back(pCollider);
	}
}

void e2d::ColliderManager::__removeCollider(Collider * pCollider)
{
	if (pCollider)
	{
		for (size_t i = 0; i < s_vColliders.size(); i++)
		{
			if (s_vColliders[i] == pCollider)
			{
				SafeRelease(&pCollider);
				s_vColliders.erase(s_vColliders.begin() + i);
				return;
			}
		}
	}
}
