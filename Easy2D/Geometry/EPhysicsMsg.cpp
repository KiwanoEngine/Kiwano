#include "..\egeometry.h"

e2d::EPhysicsMsg::INTERSECT_RELATION e2d::EPhysicsMsg::s_nRelation = e2d::EPhysicsMsg::INTERSECT_RELATION::UNKNOWN;
e2d::EGeometry * e2d::EPhysicsMsg::s_pActiveGeometry = nullptr;
e2d::EGeometry * e2d::EPhysicsMsg::s_pPassiveGeometry = nullptr;

e2d::EPhysicsMsg::INTERSECT_RELATION e2d::EPhysicsMsg::getMsg()
{
	return EPhysicsMsg::s_nRelation;
}

e2d::EGeometry * e2d::EPhysicsMsg::getActiveGeometry()
{
	return EPhysicsMsg::s_pActiveGeometry;
}

e2d::EGeometry * e2d::EPhysicsMsg::getPassiveGeometry()
{
	return EPhysicsMsg::s_pPassiveGeometry;
}
