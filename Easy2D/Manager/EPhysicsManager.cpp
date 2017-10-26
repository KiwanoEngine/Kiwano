#include "..\emanagers.h"
#include "..\egeometry.h"

e2d::EVector<e2d::EGeometry*> s_vGeometries;

void e2d::EPhysicsManager::bindWith(EGeometry * geometry, ENode * node)
{
	WARN_IF(geometry == nullptr, "EGeometry NULL pointer exception!");
	WARN_IF(node == nullptr, "EGeometry add to a NULL ENode pointer!");

	if (geometry && node)
	{
		ASSERT(
			!geometry->m_pParentNode,
			"The geometry is already added, it cannot bind again!"
		);

		geometry->retain();
		geometry->m_pParentNode = node;
		s_vGeometries.push_back(geometry);
	}
}

void e2d::EPhysicsManager::PhysicsProc()
{
}
