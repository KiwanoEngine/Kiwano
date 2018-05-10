#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dcollider.h"
#include "..\e2dtool.h"

// 监听器
class Listener
{
public:
	Listener(
		const e2d::Function& func,
		const e2d::String& name,
		bool paused
	)
		: name(name)
		, callback(func)
		, running(!paused)
		, stopped(false)
	{
	}

	// 更新监听器状态
	virtual void update()
	{
		if (callback)
		{
			callback();
		}
	}

public:
	bool running;
	bool stopped;
	e2d::String name;
	e2d::Function callback;
};


// 碰撞体集合
static std::vector<e2d::Collider*> s_vColliders;
// 监听器容器
static std::vector<Listener*> s_vListeners;
// 碰撞触发状态
static bool s_bCollisionEnable = false;
// 发生碰撞的节点
static e2d::Node * s_pActiveNode = nullptr;
static e2d::Node * s_pPassiveNode = nullptr;


void e2d::ColliderManager::setEnable(bool enable)
{
	s_bCollisionEnable = enable;
}

void e2d::ColliderManager::__update()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto listener = s_vListeners[i];
		// 清除已停止的监听器
		if (listener->stopped)
		{
			delete listener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// 更新监听器
			listener->update();
			++i;
		}
	}
}

void e2d::ColliderManager::__updateCollider(e2d::Collider * pActiveCollider)
{
	// 判断碰撞触发是否打开
	if (!s_bCollisionEnable)
		return;

	Node* pActiveNode = pActiveCollider->_parentNode;
	if (pActiveNode)
	{
		// 获取节点所在场景
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// 判断与其他碰撞体的交集情况
		for (size_t i = 0; i < s_vColliders.size(); i++)
		{
			auto pPassiveCollider = s_vColliders[i];
			// 判断两个碰撞体是否是同一个对象
			if (pActiveCollider == pPassiveCollider)
				continue;

			// 获取被碰撞节点
			Node* pPassiveNode = pPassiveCollider->_parentNode;
			// 判断两节点是否处于同一场景中
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				// 判断两物体是否是相互冲突的物体
				auto IsCollideWith = [](Node * active, Node * passive) -> bool
				{
					unsigned int hash = passive->getHashName();
					for (auto collider : active->_colliders)
						if (collider == hash)
							return true;
					return false;
				};

				if (IsCollideWith(pActiveNode, pPassiveNode))
				{
					// 判断两碰撞体交集情况
					Relation relation = pActiveCollider->getRelationWith(pPassiveCollider);
					// 忽略 UNKNOWN 和 DISJOINT 情况
					if (relation != Relation::UNKNOWN && relation != Relation::DISJOINT)
					{
						s_pActiveNode = pActiveNode;
						s_pPassiveNode = pPassiveNode;
						pActiveNode->onCollide(pPassiveNode);
						pPassiveNode->onCollide(pActiveNode);
						pCurrentScene->onCollide(pActiveNode, pPassiveNode);
						ColliderManager::__update();
					}
				}
			}
		}
	}
	s_pActiveNode = nullptr;
	s_pPassiveNode = nullptr;
}

void e2d::ColliderManager::add(const Function& func, const String& name, bool paused)
{
	auto listener = new Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::ColliderManager::pause(const String& name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->running = false;
		}
	}
}

void e2d::ColliderManager::resume(const String& name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->running = true;
		}
	}
}

void e2d::ColliderManager::stop(const String& name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->stopped = true;
		}
	}
}

void e2d::ColliderManager::pauseAll()
{
	for (auto listener : s_vListeners)
	{
		listener->running = false;
	}
}

void e2d::ColliderManager::resumeAll()
{
	for (auto listener : s_vListeners)
	{
		listener->running = true;
	}
}

void e2d::ColliderManager::stopAll()
{
	for (auto listener : s_vListeners)
	{
		listener->stopped = true;
	}
}

e2d::Node * e2d::ColliderManager::getActiveNode()
{
	return s_pActiveNode;
}

e2d::Node * e2d::ColliderManager::getPassiveNode()
{
	return s_pPassiveNode;
}

e2d::Node* e2d::ColliderManager::isCausedBy(Node * node)
{
	if (s_pActiveNode == node)
		return s_pPassiveNode;
	if (s_pPassiveNode == node)
		return s_pActiveNode;
	return nullptr;
}

e2d::Node* e2d::ColliderManager::isCausedBy(const String& name)
{
	if (s_pActiveNode->getName() == name)
		return s_pActiveNode;
	if (s_pPassiveNode->getName() == name)
		return s_pPassiveNode;
	return nullptr;
}

void e2d::ColliderManager::__addCollider(Collider * pCollider)
{
	if (pCollider)
	{
		if (pCollider->_parentNode)
		{
			WARN_IF(true, "ColliderManager::__add Failed! The shape is already added.");
			return;
		}
		pCollider->retain();
		s_vColliders.push_back(pCollider);
	}
}

void e2d::ColliderManager::__removeCollider(Collider * pCollider)
{
	if (pCollider)
	{
		for (size_t i = 0; i < s_vColliders.size(); i++)
		{
			if (s_vColliders[i] == pCollider)
			{
				SafeRelease(pCollider);
				s_vColliders.erase(s_vColliders.begin() + i);
				return;
			}
		}
	}
}

void e2d::ColliderManager::__uninit()
{
	for (auto listener : s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}
