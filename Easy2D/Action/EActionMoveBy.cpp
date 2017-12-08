#include "..\eactions.h"


e2d::EActionMoveBy::EActionMoveBy(float duration, EVec vector) :
	EActionGradual(duration)
{
	m_MoveVector = vector;
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
	EAction::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	while (EActionGradual::_isDelayEnough())
	{
		// 移动节点
		m_pTarget->setPos(
			m_BeginPos.x + m_MoveVector.x * m_fRateOfProgress,
			m_BeginPos.y + m_MoveVector.y * m_fRateOfProgress
		);
		// 判断动作是否结束
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionMoveBy::_reset()
{
	EActionGradual::_reset();
}

e2d::EActionMoveBy * e2d::EActionMoveBy::clone() const
{
	return new EActionMoveBy(m_fTotalDuration / 1000, m_MoveVector);
}

e2d::EActionMoveBy * e2d::EActionMoveBy::reverse() const
{
	return new EActionMoveBy(m_fTotalDuration / 1000, EVec(-m_MoveVector.x, -m_MoveVector.y));
}