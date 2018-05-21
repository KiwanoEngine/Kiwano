#include "..\e2dcollider.h"
#include "..\e2dnode.h"

typedef std::pair<UINT, UINT> HashPair;

e2d::Node * e2d::Collision::__activeNode = nullptr;
e2d::Node * e2d::Collision::__passiveNode = nullptr;
static std::set<HashPair> __collisionList;


void e2d::Collision::addName(const String & name1, const String & name2)
{
	if (!name1.isEmpty() && !name2.isEmpty())
	{
		__collisionList.insert(HashPair(name1.getHashCode(), name2.getHashCode()));
	}
}

void e2d::Collision::addName(const std::vector<std::pair<String, String> >& names)
{
	for (auto& name : names)
	{
		if (!name.first.isEmpty() && !name.second.isEmpty())
		{
			__collisionList.insert(HashPair(name.first.getHashCode(), name.second.getHashCode()));
		}
	}
}

bool e2d::Collision::isCollidable(Node * node1, Node * node2)
{
	return Collision::isCollidable(node1->getName(), node2->getName());
}

bool e2d::Collision::isCollidable(const String & name1, const String & name2)
{
	UINT hashName1 = name1.getHashCode(), hashName2 = name2.getHashCode();
	HashPair pair1 = HashPair(hashName1, hashName2), pair2 = HashPair(hashName2, hashName1);
	for (auto& pair : __collisionList)
	{
		if (pair == pair1 || pair == pair2)
		{
			return true;
		}
	}
	return false;
}

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