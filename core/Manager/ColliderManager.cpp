#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dcollider.h"
#include "..\e2dtool.h"


e2d::ColliderManager * e2d::ColliderManager::_instance = nullptr;

e2d::ColliderManager * e2d::ColliderManager::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) ColliderManager;
	return _instance;
}

void e2d::ColliderManager::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

e2d::ColliderManager::ColliderManager()
	: _colliders()
{
}

e2d::ColliderManager::~ColliderManager()
{
}

void e2d::ColliderManager::clearAll()
{
	for (auto collder : _colliders)
	{
		collder->release();
	}
	_colliders.clear();
}

void e2d::ColliderManager::__updateCollider(e2d::Collider * pActiveCollider)
{
	// 判断碰撞监听是否打开
	if (!Game::getInstance()->getConfig().isCollisionEnabled())
		return;

	Node* pActiveNode = pActiveCollider->_parentNode;
	if (pActiveNode)
	{
		// 获取节点所在场景
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// 判断与其他碰撞体的交集情况
		for (size_t i = 0; i < _colliders.size(); ++i)
		{
			auto pPassiveCollider = _colliders[i];
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
					if (relation != Collider::Relation::Unknown && relation != Collider::Relation::Disjoin)
					{
						// 更新碰撞监听器
						Collision::__update(pActiveNode, pPassiveNode);
					}
				}
			}
		}
	}
}

void e2d::ColliderManager::__add(Collider * pCollider)
{
	if (pCollider)
	{
		pCollider->retain();
		_colliders.push_back(pCollider);
	}
}

void e2d::ColliderManager::__remove(Collider * pCollider)
{
	if (pCollider)
	{
		for (size_t i = 0; i < _colliders.size(); ++i)
		{
			if (_colliders[i] == pCollider)
			{
				GC::safeRelease(pCollider);
				_colliders.erase(_colliders.begin() + i);
				return;
			}
		}
	}
}
