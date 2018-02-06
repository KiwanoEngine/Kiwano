#include "..\eactions.h"

e2d::EActionMoveTo::EActionMoveTo(float duration, EPoint pos) :
	EActionMoveBy(duration, EVector2())
{
	m_EndPos = pos;
}

e2d::EActionMoveTo * e2d::EActionMoveTo::clone() const
{
	return new EActionMoveTo(m_fDuration, m_EndPos);
}

void e2d::EActionMoveTo::_init()
{
	EActionMoveBy::_init();
	m_MoveVec = m_EndPos - m_BeginPos;
}
