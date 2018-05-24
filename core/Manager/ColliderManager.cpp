#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dcollider.h"
#include "..\e2dtool.h"


// 碰撞体集合
static std::vector<e2d::Collider*> s_vColliders;

void e2d::ColliderManager::__updateCollider(e2d::Collider * pActiveCollider)
{
	// 判断碰撞触发是否打开
	if (!Collision::isEnable())
		return;

	Node* pActiveNode = pActiveCollider->_parentNode;
	if (pActiveNode)
	{
		// 获取节点所在场景
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// 判断与其他碰撞体的交集情况
		for (size_t i = 0; i < s_vColliders.size(); ++i)
		{
			auto pPassiveCollider = s_vColliders[i];
			// 判断两个碰撞体是否是同一个对象
			if (pActiveCollider == pPassiveCollider)
				continue;

			// 获取被碰撞节点
			Node* pPassiveNode = pPassiveCollider->_parentNode;
			// 判断两节点是否处于同一场景中
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				// 判断两物体是否是相互冲突的物体
				if (Collision::isCollidable(pActiveNode, pPassiveNode))
				{
					// 判断两碰撞体交集情况
					Collider::Relation relation = pActiveCollider->getRelationWith(pPassiveCollider);
					// 忽略 UNKNOWN 和 DISJOIN 情况
					if (relation != Collider::Relation::UNKNOWN && relation != Collider::Relation::DISJOIN)
					{
						// 更新碰撞监听器
						Collision::__update(pActiveNode, pPassiveNode);
					}
				}
			}
		}
	}
}

void e2d::ColliderManager::__addCollider(Collider * pCollider)
{
	if (pCollider)
	{
		if (pCollider->_parentNode)
		{
			WARN("ColliderManager::__add Failed! The shape is already added.");
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
		for (size_t i = 0; i < s_vColliders.size(); ++i)
		{
			if (s_vColliders[i] == pCollider)
			{
				GC::release(pCollider);
				s_vColliders.erase(s_vColliders.begin() + i);
				return;
			}
		}
	}
}
