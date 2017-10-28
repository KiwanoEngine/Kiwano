#include "..\egeometry.h"
#include "..\Win\winbase.h"
#include "..\enodes.h"

e2d::EGeometry::EGeometry()
	: m_bTransformNeeded(true)
	, m_pParentNode(nullptr)
	, m_pTransformedGeometry(nullptr)
{
}

e2d::ENode * e2d::EGeometry::getParentNode() const
{
	return m_pParentNode;
}

e2d::EPhysicsMsg::INTERSECT_RELATION e2d::EGeometry::_intersectWith(EGeometry * pGeometry)
{
	D2D1_GEOMETRY_RELATION relation;

	m_pTransformedGeometry->CompareWithGeometry(
		pGeometry->m_pTransformedGeometry,
		D2D1::Matrix3x2F::Identity(),
		&relation
	);

	return EPhysicsMsg::INTERSECT_RELATION(relation);
}

void e2d::EGeometry::_transform()
{
	if (m_bTransformNeeded && m_pParentNode)
	{
		SafeReleaseInterface(&m_pTransformedGeometry);

		GetFactory()->CreateTransformedGeometry(
			_getD2dGeometry(),
			m_pParentNode->m_Matri,
			&m_pTransformedGeometry
		);
	}
	m_bTransformNeeded = false;
}
