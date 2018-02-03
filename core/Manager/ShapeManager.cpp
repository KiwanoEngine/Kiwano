#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\eshape.h"

// 形状集合
std::vector<e2d::EShape*> s_vShapes;


void e2d::EShapeManager::__updateShape(e2d::EShape * pActiveShape)
{
	ENode* pActiveNode = pActiveShape->m_pParentNode;
	if (pActiveNode)
	{
		// 获取节点所在场景
		EScene* pCurrentScene = pActiveNode->getParentScene();
		// 判断与其他形状的交集情况
		for (auto pPassiveShape : s_vShapes)
		{
			// 判断两物体是否是相互冲突的物体
			if (pActiveShape->m_nCollisionBitmask & pPassiveShape->m_nCategoryBitmask)
			{
				// 获取被碰撞节点
				ENode* pPassiveNode = pPassiveShape->m_pParentNode;
				// 判断两节点是否处于同一场景中
				if (pPassiveNode &&
					pPassiveNode != pActiveNode &&
					pPassiveNode->getParentScene() == pCurrentScene)
				{
					// 判断两形状交集情况
					int relation = pActiveShape->getRelationWith(pPassiveShape);
					// 忽略 UNKNOWN 和 DISJOINT 情况
					if (relation != ERelation::UNKNOWN && relation != ERelation::DISJOINT)
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

void e2d::EShapeManager::__addShape(EShape * pShape)
{
	if (pShape)
	{
		pShape->retain();
		s_vShapes.push_back(pShape);
	}
}

void e2d::EShapeManager::__delShape(EShape * pShape)
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
