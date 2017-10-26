#include "..\egeometry.h"
#include "..\Win\winbase.h"

e2d::EGeometry::EGeometry()
	: m_bTransformed(true)
	, m_pParentNode(nullptr)
{
}

bool e2d::EGeometry::_isCollisionWith(EGeometry * pGeometry)
{
	D2D1_GEOMETRY_RELATION relation;

	HRESULT	hr = this->_getD2dGeometry()->CompareWithGeometry(
		pGeometry->_getD2dGeometry(),
		D2D1::Matrix3x2F::Identity(),
		&relation
	);

	if (SUCCEEDED(hr))
	{
		return (relation == D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_OVERLAP) || 
			(relation == D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_CONTAINS) ||
			(relation == D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_IS_CONTAINED);
	}

	return false;
}