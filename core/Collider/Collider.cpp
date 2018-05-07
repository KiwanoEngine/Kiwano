#include "..\e2dcollider.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider()
	: m_bIsVisiable(true)
	, m_nColor(Color::RED, 0.7f)
	, m_pParentNode(nullptr)
	, m_pTransformedGeometry(nullptr)
	, m_bEnable(true)
	, m_bAutoResize(true)
{
}

e2d::Collider::~Collider()
{
	SafeReleaseInterface(&m_pTransformedGeometry);
}

e2d::Node * e2d::Collider::getParentNode() const
{
	return m_pParentNode;
}

e2d::Color e2d::Collider::getColor() const
{
	return m_nColor;
}

void e2d::Collider::setEnable(bool enable)
{
	m_bEnable = enable;
}

void e2d::Collider::setVisiable(bool bVisiable)
{
	m_bIsVisiable = bVisiable;
}

void e2d::Collider::setColor(Color color)
{
	m_nColor = color;
}

void e2d::Collider::setAutoResize(bool enable)
{
	m_bAutoResize = enable;
}

void e2d::Collider::_render()
{
	if (m_pTransformedGeometry && m_bEnable)
	{
		// 获取纯色画刷
		ID2D1SolidColorBrush * pBrush = Renderer::getSolidColorBrush();
		// 设置画刷颜色和透明度
		pBrush->SetColor(m_nColor.toColorF());
		// 绘制几何碰撞体
		Renderer::getRenderTarget()->DrawGeometry(m_pTransformedGeometry, pBrush);
	}
}

e2d::Relation e2d::Collider::getRelationWith(Collider * pCollider) const
{
	if (m_pTransformedGeometry && pCollider->m_pTransformedGeometry)
	{
		if (m_bEnable && pCollider->m_bEnable)
		{
			D2D1_GEOMETRY_RELATION relation;

			m_pTransformedGeometry->CompareWithGeometry(
				pCollider->m_pTransformedGeometry,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return Relation(relation);
		}
	}
	return Relation::UNKNOWN;
}

void e2d::Collider::_transform()
{
	if (m_pParentNode && m_bEnable)
	{
		if (m_bAutoResize)
		{
			this->_resize();
		}

		// 释放原碰撞体
		SafeReleaseInterface(&m_pTransformedGeometry);

		// 根据父节点转换几何图形
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			getD2dGeometry(),
			m_pParentNode->m_MatriFinal,
			&m_pTransformedGeometry
		);

		ColliderManager::__updateCollider(this);
	}
}
