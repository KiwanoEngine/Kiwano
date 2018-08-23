#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dtool.h"


e2d::CollisionManager * e2d::CollisionManager::instance()
{
	static CollisionManager instance;
	return &instance;
}

e2d::CollisionManager::CollisionManager()
	: _collisionEnabled(false)
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
	if (!_collisionEnabled ||
		Game::instance()->transitioning())
		return;

	std::vector<Collider*> currColliders;
	currColliders.reserve(_colliders.size());
	std::copy_if(
		_colliders.begin(),
		_colliders.end(),
		std::back_inserter(currColliders),
		[this, collider](Collider* passive) -> bool
		{
			return collider != passive &&
				passive->node()->visible() &&
				collider->node()->parentScene() == passive->node()->parentScene() &&
				this->isCollidable(collider->node(), passive->node());
		}
	);

	for (const auto& passive : currColliders)
	{
		// 判断两碰撞体交集情况
		Collider::Relation relation = collider->relationWith(passive);
		// 忽略 UNKNOWN 和 DISJOIN 情况
		if (relation != Collider::Relation::Unknown &&
			relation != Collider::Relation::Disjoin)
		{
			auto activeNode = collider->node();
			auto passiveNode = passive->node();
			// 触发两次碰撞事件
			Collision activeCollision(passiveNode, relation);
			if (dynamic_cast<CollisionHandler*>(activeNode->parentScene()))
				dynamic_cast<CollisionHandler*>(activeNode->parentScene())->handle(activeCollision);
			if (dynamic_cast<CollisionHandler*>(activeNode))
				dynamic_cast<CollisionHandler*>(activeNode)->handle(activeCollision);

			Collision passiveCollision(activeNode, passive->relationWith(collider));
			if (dynamic_cast<CollisionHandler*>(passiveNode->parentScene()))
				dynamic_cast<CollisionHandler*>(passiveNode->parentScene())->handle(passiveCollision);
			if (dynamic_cast<CollisionHandler*>(passiveNode))
				dynamic_cast<CollisionHandler*>(passiveNode)->handle(passiveCollision);
		}
	}
}

void e2d::CollisionManager::enabled(bool enabled)
{
	_collisionEnabled = enabled;
}

void e2d::CollisionManager::addName(const String & name1, const String & name2)
{
	if (!name1.empty() && !name2.empty())
	{
		_collisionList.insert(std::make_pair(name1.hash(), name2.hash()));
	}
}

void e2d::CollisionManager::addName(const std::vector<std::pair<String, String> >& names)
{
	for (const auto& name : names)
	{
		if (!name.first.empty() && !name.second.empty())
		{
			_collisionList.insert(std::make_pair(name.first.hash(), name.second.hash()));
		}
	}
}

bool e2d::CollisionManager::isCollidable(Node * node1, Node * node2)
{
	return CollisionManager::isCollidable(node1->name(), node2->name());
}

bool e2d::CollisionManager::isCollidable(const String & name1, const String & name2)
{
	size_t hashName1 = name1.hash(), 
		hashName2 = name2.hash();
	auto pair1 = std::make_pair(hashName1, hashName2), 
		pair2 = std::make_pair(hashName2, hashName1);
	for (const auto& pair : _collisionList)
	{
		if (pair == pair1 || pair == pair2)
		{
			return true;
		}
	}
	return false;
}
