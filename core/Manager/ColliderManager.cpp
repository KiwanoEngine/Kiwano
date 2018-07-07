#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dcollider.h"


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
{
}

e2d::ColliderManager::~ColliderManager()
{
}

void e2d::ColliderManager::__remove(Node * node)
{
	if (node)
	{
		auto iter = std::find(_nodes.begin(), _nodes.end(), node);
		if (iter != _nodes.end())
		{
			_nodes.erase(iter);
		}
	}
}

void e2d::ColliderManager::updateCollider(Node * node)
{
	if (node)
	{
		if (node->getCollider()->_type != Collider::Type::None)
		{
			node->getCollider()->_recreate();
			_nodes.insert(node);
		}
	}
}

void e2d::ColliderManager::update()
{
	// 判断碰撞监听是否打开
	if (Game::getInstance()->isPaused() ||
		!Game::getInstance()->getConfig()->isCollisionEnabled() ||
		SceneManager::getInstance()->isTransitioning())
	{
		_nodes.clear();
		return;
	}

	for (auto iter1 = _nodes.begin(); iter1 != _nodes.end(); iter1++)
	{
		auto node1 = (*iter1);
		// 判断与其他碰撞体的交集情况
		auto iter2 = iter1;
		iter2++;
		for (; iter2 != _nodes.end(); iter2++)
		{
			auto node2 = (*iter2);
			// 判断两物体是否是相互冲突的物体
			if (Collision::isCollidable(node1, node2))
			{
				// 判断两碰撞体交集情况
				Collider::Relation relation = node1->getCollider()->getRelationWith(node2->getCollider());
				// 忽略 UNKNOWN 和 DISJOIN 情况
				if (relation != Collider::Relation::Unknown && 
					relation != Collider::Relation::Disjoin)
				{
					// 更新碰撞监听器
					Collision::__update(node1, node2);
				}
			}
		}
	}
	_nodes.clear();
}
