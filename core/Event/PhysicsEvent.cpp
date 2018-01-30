#include "..\ecommon.h"

e2d::EPhysicsEvent::INTERSECT_RELATION e2d::EPhysicsEvent::s_nRelation = e2d::EPhysicsEvent::UNKNOWN;
e2d::EGeometry * e2d::EPhysicsEvent::s_pActiveGeometry = nullptr;
e2d::EGeometry * e2d::EPhysicsEvent::s_pPassiveGeometry = nullptr;

e2d::EPhysicsEvent::INTERSECT_RELATION e2d::EPhysicsEvent::getMsg()
{
	return EPhysicsEvent::s_nRelation;
}

e2d::EGeometry * e2d::EPhysicsEvent::getActiveGeometry()
{
	return EPhysicsEvent::s_pActiveGeometry;
}

e2d::EGeometry * e2d::EPhysicsEvent::getPassiveGeometry()
{
	return EPhysicsEvent::s_pPassiveGeometry;
}
