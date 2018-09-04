#include "..\e2devent.h"

e2d::Collision::Collision()
	: node_(nullptr)
	, relation_(Collider::Relation::Unknown)
{
}

e2d::Collision::Collision(Node* node, Collider::Relation relation)
	: node_(node)
	, relation_(relation)
{
}

e2d::Collision::~Collision()
{
}

e2d::Node * e2d::Collision::GetNode() const
{
	return node_;
}

e2d::Collider::Relation e2d::Collision::GetRelation() const
{
	return relation_;
}
