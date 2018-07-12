#include "..\e2dcommon.h"

e2d::Collision::Collision()
	: _active(nullptr)
	, _passive(nullptr)
	, _relation(Collider::Relation::Unknown)
{
}

e2d::Collision::Collision(Node* active, Node* passive, Collider::Relation relation)
	: _active(active)
	, _passive(passive)
	, _relation(relation)
{
}

e2d::Collision::~Collision()
{
}

e2d::Node * e2d::Collision::getActive() const
{
	return _active;
}

e2d::Node * e2d::Collision::getPassive() const
{
	return _passive;
}

e2d::Collider::Relation e2d::Collision::getRelation() const
{
	return _relation;
}
