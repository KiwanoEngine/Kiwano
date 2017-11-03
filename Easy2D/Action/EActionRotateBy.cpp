#include "..\eactions.h"


e2d::EActionRotateBy::EActionRotateBy(float duration, float rotation) :
	EActionGradual(duration)
{
	m_nVariation = rotation;
}

void e2d::EActionRotateBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginVal = m_pTarget->getRotation();
	}
}

void e2d::EActionRotateBy::_callOn()
{
	EAction::_callOn();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	while (EActionGradual::_isDelayEnough())
	{
		// 旋转节点
		m_pTarget->setRotation(m_nBeginVal + m_nVariation * m_fRateOfProgress);
		// 判断动作是否结束
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionRotateBy::_reset()
{
	EActionGradual::_reset();
}

e2d::EActionRotateBy * e2d::EActionRotateBy::clone() const
{
	return new EActionRotateBy(m_fTotalDuration / 1000, m_nVariation);
}

e2d::EActionRotateBy * e2d::EActionRotateBy::reverse() const
{
	return new EActionRotateBy(m_fTotalDuration / 1000, -m_nVariation);
}