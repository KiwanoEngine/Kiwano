#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dtool.h"


e2d::CollisionManager * e2d::CollisionManager::_instance = nullptr;

e2d::CollisionManager * e2d::CollisionManager::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) CollisionManager;
	return _instance;
}

void e2d::CollisionManager::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

e2d::CollisionManager::CollisionManager()
{
}

e2d::CollisionManager::~CollisionManager()
{
}

void e2d::CollisionManager::__addCollider(Collider * collider)
{
	_colliders.push_back(collider);
}

void e2d::CollisionManager::__removeCollider(Collider * collider)
{
	auto iter = std::find(_colliders.begin(), _colliders.end(), collider);
	if (iter != _colliders.end())
	{
		_colliders.erase(iter);
	}
}

void e2d::CollisionManager::__updateCollider(Collider* collider)
{
	if (Game::getInstance()->isPaused() ||
		!Game::getInstance()->getConfig().isCollisionEnabled() ||
		SceneManager::getInstance()->isTransitioning())
		return;

	for (size_t i = 0; i < _colliders.size(); i++)
	{
		// 判断与其他碰撞体的交集情况
		auto active = collider->getNode();
		auto passive = _colliders[i]->getNode();
		// 判断两物体是否是相互冲突的物体
		if (active == passive ||
			active->getParentScene() != passive->getParentScene() ||
			!CollisionManager::isCollidable(active, passive))
		{
			continue;
		}

		// 判断两碰撞体交集情况
		Collider::Relation relation = active->getCollider()->getRelationWith(passive->getCollider());
		// 忽略 UNKNOWN 和 DISJOIN 情况
		if (relation != Collider::Relation::Unknown &&
			relation != Collider::Relation::Disjoin)
		{
			// 触发两次碰撞事件
			Collision activeCollision(passive, relation);
			active->getParentScene()->onCollision(activeCollision);
			active->onCollision(activeCollision);

			Collision passiveCollision(active, passive->getCollider()->getRelationWith(active->getCollider()));
			passive->getParentScene()->onCollision(passiveCollision);
			passive->onCollision(passiveCollision);
		}
	}
}

void e2d::CollisionManager::addName(const String & name1, const String & name2)
{
	if (!name1.isEmpty() && !name2.isEmpty())
	{
		_collisionList.insert(std::make_pair(name1.getHashCode(), name2.getHashCode()));
	}
}

void e2d::CollisionManager::addName(const std::vector<std::pair<String, String> >& names)
{
	for (auto& name : names)
	{
		if (!name.first.isEmpty() && !name.second.isEmpty())
		{
			_collisionList.insert(std::make_pair(name.first.getHashCode(), name.second.getHashCode()));
		}
	}
}

bool e2d::CollisionManager::isCollidable(Node * node1, Node * node2)
{
	return CollisionManager::isCollidable(node1->getName(), node2->getName());
}

bool e2d::CollisionManager::isCollidable(const String & name1, const String & name2)
{
	size_t hashName1 = name1.getHashCode(), 
		hashName2 = name2.getHashCode();
	auto pair1 = std::make_pair(hashName1, hashName2), 
		pair2 = std::make_pair(hashName2, hashName1);
	for (auto& pair : _collisionList)
	{
		if (pair == pair1 || pair == pair2)
		{
			return true;
		}
	}
	return false;
}
