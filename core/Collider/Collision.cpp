#include "..\e2dcollider.h"
#include "..\e2dnode.h"
#include "..\e2dtool.h"

typedef std::pair<UINT, UINT> HashPair;

// ¼àÌýÆ÷ÈÝÆ÷
static std::vector<e2d::Listener*> s_vListeners;
// Åö×²´¥·¢×´Ì¬
static bool s_bCollisionEnable = false;
static e2d::Node * s_pActiveNode = nullptr;
static e2d::Node * s_pPassiveNode = nullptr;
static std::set<HashPair> s_sCollisionList;


void e2d::Collision::addName(const String & name1, const String & name2)
{
	if (!name1.isEmpty() && !name2.isEmpty())
	{
		s_sCollisionList.insert(HashPair(name1.getHashCode(), name2.getHashCode()));
	}
}

void e2d::Collision::addName(const std::vector<std::pair<String, String> >& names)
{
	for (auto& name : names)
	{
		if (!name.first.isEmpty() && !name.second.isEmpty())
		{
			s_sCollisionList.insert(HashPair(name.first.getHashCode(), name.second.getHashCode()));
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
	for (auto& pair : s_sCollisionList)
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
	return s_pActiveNode;
}

e2d::Node * e2d::Collision::getPassiveNode()
{
	return s_pPassiveNode;
}

e2d::Node* e2d::Collision::isCausedBy(Node * node)
{
	if (s_pActiveNode == node)
		return s_pPassiveNode;
	if (s_pPassiveNode == node)
		return s_pActiveNode;
	return nullptr;
}

e2d::Node* e2d::Collision::isCausedBy(const String& name)
{
	if (s_pActiveNode->getName() == name)
		return s_pActiveNode;
	if (s_pPassiveNode->getName() == name)
		return s_pPassiveNode;
	return nullptr;
}


void e2d::Collision::setEnable(bool enable)
{
	s_bCollisionEnable = enable;
}

bool e2d::Collision::isEnable()
{
	return s_bCollisionEnable;
}

void e2d::Collision::__update(Node * active, Node * passive)
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	s_pActiveNode = active;
	s_pPassiveNode = passive;

	for (size_t i = 0; i < s_vListeners.size(); ++i)
	{
		auto listener = s_vListeners[i];
		// Çå³ýÒÑÍ£Ö¹µÄ¼àÌýÆ÷
		if (listener->_stopped)
		{
			delete listener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ¸üÐÂ¼àÌýÆ÷
			listener->_update();
			++i;
		}
	}

	s_pActiveNode = nullptr;
	s_pPassiveNode = nullptr;
}

void e2d::Collision::addListener(const Function& func, const String& name, bool paused)
{
	auto listener = new (std::nothrow) Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::Collision::stopListener(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->stop();
		}
	}
}

void e2d::Collision::startListener(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->start();
		}
	}
}

void e2d::Collision::clearListener(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->_stopped = true;
		}
	}
}

void e2d::Collision::stopAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->stop();
	}
}

void e2d::Collision::startAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->start();
	}
}

void e2d::Collision::clearAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->_stopped = true;
	}
}

void e2d::Collision::__clearListeners()
{
	for (auto listener : s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}