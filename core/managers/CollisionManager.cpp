#include "..\e2dmanager.h"
#include "..\e2dobject.h"
#include "..\e2dtool.h"
#include "..\e2dmodule.h"


e2d::CollisionManager * e2d::CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

e2d::CollisionManager::CollisionManager()
	: collision_enabled_(false)
{
}

e2d::CollisionManager::~CollisionManager()
{
}

void e2d::CollisionManager::AddCollider(Collider * collider)
{
	colliders_.push_back(collider);
}

void e2d::CollisionManager::RemoveCollider(Collider * collider)
{
	auto iter = std::find(colliders_.begin(), colliders_.end(), collider);
	if (iter != colliders_.end())
	{
		colliders_.erase(iter);
	}
}

void e2d::CollisionManager::UpdateCollider(Collider* active)
{
	if (!collision_enabled_ ||
		Game::Get()->IsTransitioning())
		return;

	auto currScene = Game::Get()->GetCurrentScene();
	if (active->GetNode()->GetParentScene() != currScene)
		return;

	std::vector<Collider*> currColliders;
	currColliders.reserve(colliders_.size());
	std::copy_if(
		colliders_.begin(),
		colliders_.end(),
		std::back_inserter(currColliders),
		[this, active, currScene](Collider* passive) -> bool
		{
			return active != passive &&
				passive->GetNode()->IsVisible() &&
				passive->GetNode()->GetParentScene() == currScene &&
				this->IsCollidable(active->GetNode(), passive->GetNode());
		}
	);

	for (const auto& passive : currColliders)
	{
		// 判断两碰撞体交集情况
		Collider::Relation relation = active->GetRelationWith(passive);
		// 忽略 UNKNOWN 和 DISJOIN 情况
		if (relation != Collider::Relation::Unknown &&
			relation != Collider::Relation::Disjoin)
		{
			auto activeNode = active->GetNode();
			auto passiveNode = passive->GetNode();
			// 触发两次碰撞事件
			Collision activeCollision(passiveNode, relation);
			if (dynamic_cast<CollisionHandler*>(activeNode))
				dynamic_cast<CollisionHandler*>(activeNode)->Handle(activeCollision);

			Collision passiveCollision(activeNode, passive->GetRelationWith(active));
			if (dynamic_cast<CollisionHandler*>(passiveNode))
				dynamic_cast<CollisionHandler*>(passiveNode)->Handle(passiveCollision);
		}
	}
}

void e2d::CollisionManager::SetCollisionEnabled(bool enabled)
{
	collision_enabled_ = enabled;
}

void e2d::CollisionManager::AddName(const String & name1, const String & name2)
{
	if (!name1.IsEmpty() && !name2.IsEmpty())
	{
		collision_list_.insert(std::make_pair(name1.GetHash(), name2.GetHash()));
	}
}

void e2d::CollisionManager::AddName(const std::vector<std::pair<String, String> >& names)
{
	for (const auto& name : names)
	{
		if (!name.first.IsEmpty() && !name.second.IsEmpty())
		{
			collision_list_.insert(std::make_pair(name.first.GetHash(), name.second.GetHash()));
		}
	}
}

bool e2d::CollisionManager::IsCollidable(Node * node1, Node * node2)
{
	return CollisionManager::IsCollidable(node1->GetName(), node2->GetName());
}

bool e2d::CollisionManager::IsCollidable(const String & name1, const String & name2)
{
	size_t hashName1 = name1.GetHash(), 
		hashName2 = name2.GetHash();
	auto pair1 = std::make_pair(hashName1, hashName2), 
		pair2 = std::make_pair(hashName2, hashName1);
	for (const auto& pair : collision_list_)
	{
		if (pair == pair1 || pair == pair2)
		{
			return true;
		}
	}
	return false;
}
