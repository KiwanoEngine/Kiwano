#include "..\eactions.h"


e2d::ActionMoveBy::ActionMoveBy(float duration, Vector vector) :
	ActionGradual(duration)
{
	m_MoveVec = vector;
}

void e2d::ActionMoveBy::_init()
{
	ActionGradual::_init();
	if (m_pTarget)
	{
		m_BeginPos = m_pTarget->getPos();
	}
}

void e2d::ActionMoveBy::_update()
{
	ActionGradual::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// ÒÆ¶¯½Úµã
	m_pTarget->setPos(
		m_BeginPos.x + m_MoveVec.x * m_fRateOfProgress,
		m_BeginPos.y + m_MoveVec.y * m_fRateOfProgress
	);
}

e2d::ActionMoveBy * e2d::ActionMoveBy::clone() const
{
	return new ActionMoveBy(m_fDuration, m_MoveVec);
}

e2d::ActionMoveBy * e2d::ActionMoveBy::reverse() const
{
	return new ActionMoveBy(m_fDuration, Vector(-m_MoveVec.x, -m_MoveVec.y));
}