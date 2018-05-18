#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::Node * e2d::Collision::__activeNode = nullptr;
e2d::Node * e2d::Collision::__passiveNode = nullptr;


e2d::Node * e2d::Collision::getActiveNode()
{
	return __activeNode;
}

e2d::Node * e2d::Collision::getPassiveNode()
{
	return __passiveNode;
}

e2d::Node* e2d::Collision::isCausedBy(Node * node)
{
	if (__activeNode == node)
		return __passiveNode;
	if (__passiveNode == node)
		return __activeNode;
	return nullptr;
}

e2d::Node* e2d::Collision::isCausedBy(const String& name)
{
	if (__activeNode->getName() == name)
		return __activeNode;
	if (__passiveNode->getName() == name)
		return __passiveNode;
	return nullptr;
}