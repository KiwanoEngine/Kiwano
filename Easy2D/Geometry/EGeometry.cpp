#include "..\egeometry.h"
#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"

e2d::EGeometry::EGeometry()
	: m_bTransformed(false)
	, m_nCategoryBitmask(0)
	, m_nCollisionBitmask(0)
	, m_bIsVisiable(true)
	, m_nColor(EColor::RED)
	, m_fOpacity(1)
	, m_pParentNode(nullptr)
	, m_pTransformedGeometry(nullptr)
{
}

e2d::EGeometry::~EGeometry()
{
	SafeReleaseInterface(&m_pTransformedGeometry);
}

e2d::ENode * e2d::EGeometry::getParentNode() const
{
	return m_pParentNode;
}

UINT32 e2d::EGeometry::getCategoryBitmask() const
{
	return m_nCategoryBitmask;
}

UINT32 e2d::EGeometry::getCollisionBitmask() const
{
	return m_nCollisionBitmask;
}

void e2d::EGeometry::setCategoryBitmask(UINT32 mask)
{
	m_nCategoryBitmask = mask;
}

void e2d::EGeometry::setCollisionBitmask(UINT32 mask)
{
	m_nCollisionBitmask = mask;
}

void e2d::EGeometry::setVisiable(bool bVisiable)
{
	m_bIsVisiable = bVisiable;
}

void e2d::EGeometry::setColor(UINT32 color)
{
	m_nColor = color;
}

void e2d::EGeometry::setOpacity(float opacity)
{
	m_fOpacity = min(max(opacity, 0), 1);
}

void e2d::EGeometry::_onRender()
{
	if (m_pTransformedGeometry)
	{
		// 创建画刷
		GetRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(
				m_nColor,
				m_fOpacity),
			&GetSolidColorBrush()
		);
		// 绘制几何形状
		GetRenderTarget()->DrawGeometry(m_pTransformedGeometry, GetSolidColorBrush());
		// 释放临时资源
		SafeReleaseInterface(&GetSolidColorBrush());
	}
}

e2d::EPhysicsMsg::INTERSECT_RELATION e2d::EGeometry::_intersectWith(EGeometry * pGeometry)
{
	if (m_pTransformedGeometry && pGeometry->m_pTransformedGeometry)
	{
		D2D1_GEOMETRY_RELATION relation;

		m_pTransformedGeometry->CompareWithGeometry(
			pGeometry->m_pTransformedGeometry,
			D2D1::Matrix3x2F::Identity(),
			&relation
		);

		return EPhysicsMsg::INTERSECT_RELATION(relation);
	}
	return EPhysicsMsg::INTERSECT_RELATION::UNKNOWN;
}

void e2d::EGeometry::_transform()
{
	if (m_pParentNode)
	{
		// 释放原形状
		SafeReleaseInterface(&m_pTransformedGeometry);

		// 根据父节点转换几何图形
		GetFactory()->CreateTransformedGeometry(
			_getD2dGeometry(),
			m_pParentNode->m_Matri,
			&m_pTransformedGeometry
		);

		// 判断形状变换后的情况
		EPhysicsManager::PhysicsGeometryProc(this);
	}
}
