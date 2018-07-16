#include "..\e2dcommon.h"

e2d::Collision::Collision()
	: _node(nullptr)
	, _relation(Collider::Relation::Unknown)
{
}

e2d::Collision::Collision(Node* node, Collider::Relation relation)
	: _node(node)
	, _relation(relation)
{
}

e2d::Collision::~Collision()
{
}

e2d::Node * e2d::Collision::getNode() const
{
	return _node;
}

e2d::Collider::Relation e2d::Collision::getRelation() const
{
	return _relation;
}
