#include "..\eactions.h"


e2d::EActionMoveBy::EActionMoveBy(float duration, EVector2 vector) :
	EActionGradual(duration)
{
	m_MoveVec = vector;
}

void e2d::EActionMoveBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_BeginPos = m_pTarget->getPos();
	}
}

void e2d::EActionMoveBy::_update()
{
	EActionGradual::_update();

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

void e2d::EActionMoveBy::_reset()
{
	EActionGradual::_reset();
}

e2d::EActionMoveBy * e2d::EActionMoveBy::clone() const
{
	return new EActionMoveBy(m_fDuration, m_MoveVec);
}

e2d::EActionMoveBy * e2d::EActionMoveBy::reverse() const
{
	return new EActionMoveBy(m_fDuration, EVector2(-m_MoveVec.x, -m_MoveVec.y));
}