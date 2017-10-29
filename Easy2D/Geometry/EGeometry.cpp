#include "..\egeometry.h"
#include "..\Win\winbase.h"
#include "..\enodes.h"

e2d::EGeometry::EGeometry()
	: m_bTransformed(false)
	, m_nColor(EColor::RED)
	, m_fOpacity(1)
	, m_pParentNode(nullptr)
	, m_pTransformedGeometry(nullptr)
{
	this->autoRelease();
}

e2d::EGeometry::~EGeometry()
{
	SafeReleaseInterface(&m_pTransformedGeometry);
}

e2d::ENode * e2d::EGeometry::getParentNode() const
{
	return m_pParentNode;
}

void e2d::EGeometry::setColor(UINT32 color)
{
	m_nColor = color;
}

void e2d::EGeometry::setOpacity(float opacity)
{
	m_fOpacity = opacity;
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
		SafeReleaseInterface(&m_pTransformedGeometry);

		GetFactory()->CreateTransformedGeometry(
			_getD2dGeometry(),
			m_pParentNode->m_Matri,
			&m_pTransformedGeometry
		);

		this->m_bTransformed = true;
	}
}
