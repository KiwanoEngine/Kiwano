#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\eshape.h"
#include "..\etools.h"

// ��״����
static std::vector<e2d::Shape*> s_vShapes;
// ����������
static std::vector<e2d::CollisionListener*> s_vListeners;
// ��ײ����״̬
static bool s_bCollisionEnable = false;
// ������ײ�Ľڵ�
static e2d::Node * s_pActiveNode = nullptr;
static e2d::Node * s_pPassiveNode = nullptr;


void e2d::CollisionManager::setEnable(bool bEnable)
{
	s_bCollisionEnable = bEnable;
}

void e2d::CollisionManager::__update()
{
	if (s_vListeners.size() == 0)
		return;

	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto pListener = s_vListeners[i];
		// ���¼�����
		if (pListener->m_bClear)
		{
			pListener->release();
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else if (pListener->isRunning())
		{
			pListener->_update();
		}
	}
}

void e2d::CollisionManager::__updateShape(e2d::Shape * pActiveShape)
{
	// �ж���ײ�����Ƿ��
	if (!s_bCollisionEnable)
		return;

	Node* pActiveNode = pActiveShape->m_pParentNode;
	if (pActiveNode)
	{
		// ��ȡ�ڵ����ڳ���
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// �ж���������״�Ľ������
		for (size_t i = 0; i < s_vShapes.size(); i++)
		{
			auto pPassiveShape = s_vShapes[i];
			// �ж�������״�Ƿ���ͬһ������
			if (pActiveShape == pPassiveShape)
				continue;

			// ��ȡ����ײ�ڵ�
			Node* pPassiveNode = pPassiveShape->m_pParentNode;
			// �ж����ڵ��Ƿ���ͬһ������
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				// �ж��������Ƿ����໥��ͻ������
				auto IsCollideWith = [](Node * active, unsigned int hash)
				{
					for (auto collider : active->m_vColliders)
						if (collider == hash)
							return true;
					return false;
				};

				if (IsCollideWith(pActiveNode, pPassiveNode->getHashName()))
				{
					// �ж�����״�������
					Relation relation = pActiveShape->getRelationWith(pPassiveShape);
					// ���� UNKNOWN �� DISJOINT ���
					if (relation != Relation::UNKNOWN && relation != Relation::DISJOINT)
					{
						s_pActiveNode = pActiveNode;
						s_pPassiveNode = pPassiveNode;
						pActiveNode->onCollide(pPassiveNode);
						pPassiveNode->onCollide(pActiveNode);
						pCurrentScene->onCollide();
						CollisionManager::__update();
					}
				}
			}
		}
	}
	s_pActiveNode = nullptr;
	s_pPassiveNode = nullptr;
}

void e2d::CollisionManager::__add(CollisionListener * pListener)
{
	WARN_IF(pListener == nullptr, "CollisionListener NULL pointer exception!");

	if (pListener)
	{
		auto findListener = [](CollisionListener * pListener) -> bool
		{
			for (const auto &l : s_vListeners)
			{
				if (pListener == l)
				{
					return true;
				}
			}
			return false;
		};

		bool bHasListener = findListener(pListener);
		WARN_IF(bHasListener, "The CollisionListener is already added, cannot be added again!");

		if (!bHasListener)
		{
			pListener->retain();
			s_vListeners.push_back(pListener);
		}
	}
}

void e2d::CollisionManager::add(Function func, String name)
{
	(new CollisionListener(func, name))->start();
}

void e2d::CollisionManager::start(String name)
{
	for (const auto & pListener : s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->start();
		}
	}
}

void e2d::CollisionManager::stop(String name)
{
	for (const auto & pListener : s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->stop();
		}
	}
}

void e2d::CollisionManager::clear(String name)
{
	for (const auto & pListener : s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->stopAndClear();
		}
	}
}

void e2d::CollisionManager::startAll()
{
	for (const auto & pListener : s_vListeners)
	{
		pListener->start();
	}
}

void e2d::CollisionManager::stopAll()
{
	for (const auto & pListener : s_vListeners)
	{
		pListener->stop();
	}
}

void e2d::CollisionManager::clearAll()
{
	for (const auto & pListener : s_vListeners)
	{
		pListener->stopAndClear();
	}
}

std::vector<e2d::CollisionListener*> e2d::CollisionManager::get(String name)
{
	std::vector<CollisionListener*> vListeners;
	for (auto pListener : s_vListeners)
	{
		if (pListener->getName() == name)
		{
			vListeners.push_back(pListener);
		}
	}
	return std::move(vListeners);
}

std::vector<e2d::CollisionListener*> e2d::CollisionManager::getAll()
{
	return s_vListeners;
}

e2d::Node * e2d::CollisionManager::getNode1()
{
	return s_pActiveNode;
}

e2d::Node * e2d::CollisionManager::getNode2()
{
	return s_pPassiveNode;
}

void e2d::CollisionManager::__addShape(Shape * pShape)
{
	if (pShape)
	{
		if (pShape->m_pParentNode)
		{
			WARN_IF(true, "CollisionManager::__add Failed! The shape is already added.");
			return;
		}
		pShape->retain();
		s_vShapes.push_back(pShape);
	}
}

void e2d::CollisionManager::__removeShape(Shape * pShape)
{
	if (pShape)
	{
		for (size_t i = 0; i < s_vShapes.size(); i++)
		{
			if (s_vShapes[i] == pShape)
			{
				SafeRelease(&pShape);
				s_vShapes.erase(s_vShapes.begin() + i);
				return;
			}
		}
	}
}