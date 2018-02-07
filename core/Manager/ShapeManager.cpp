#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\eshape.h"

// 形状集合
std::vector<e2d::Shape*> s_vShapes;


void e2d::ShapeManager::__updateShape(e2d::Shape * pActiveShape)
{
	Node* pActiveNode = pActiveShape->m_pParentNode;
	if (pActiveNode)
	{
		// 获取节点所在场景
		Scene* pCurrentScene = pActiveNode->getParentScene();
		// 判断与其他形状的交集情况
		for (auto pPassiveShape : s_vShapes)
		{
			// 判断两物体是否是相互冲突的物体
			if (pActiveShape->m_nCollisionBitmask & pPassiveShape->m_nCategoryBitmask)
			{
				// 获取被碰撞节点
				Node* pPassiveNode = pPassiveShape->m_pParentNode;
				// 判断两节点是否处于同一场景中
				if (pPassiveNode &&
					pPassiveNode != pActiveNode &&
					pPassiveNode->getParentScene() == pCurrentScene)
				{
					// 判断两形状交集情况
					int relation = pActiveShape->getRelationWith(pPassiveShape);
					// 忽略 UNKNOWN 和 DISJOINT 情况
					if (relation != Relation::UNKNOWN && relation != Relation::DISJOINT)
					{
						pActiveNode->onCollide(pPassiveNode, relation);
						pPassiveNode->onCollide(pActiveNode, pPassiveShape->getRelationWith(pActiveShape));
						pCurrentScene->onCollide(pActiveNode, pPassiveNode);
					}
				}
			}
		}
	}
}

void e2d::ShapeManager::__addShape(Shape * pShape)
{
	if (pShape)
	{
		pShape->retain();
		s_vShapes.push_back(pShape);
	}
}

void e2d::ShapeManager::__delShape(Shape * pShape)
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
