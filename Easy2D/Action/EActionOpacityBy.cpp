#include "..\eactions.h"


e2d::EActionOpacityBy::EActionOpacityBy(float duration, float opacity) :
	EActionGradual(duration)
{
	m_nVariation = opacity;
}

void e2d::EActionOpacityBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginVal = m_pTarget->getOpacity();
	}
}

void e2d::EActionOpacityBy::_callOn()
{
	EAction::_callOn();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	while (EActionGradual::_isDelayEnough())
	{
		// 设置节点透明度
		m_pTarget->setOpacity(m_nBeginVal + m_nVariation * m_fRateOfProgress);
		// 判断动作是否结束
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionOpacityBy::_reset()
{
	EActionGradual::_reset();
}

e2d::EActionOpacityBy * e2d::EActionOpacityBy::clone() const
{
	return new EActionOpacityBy(m_fTotalDuration / 1000, m_nVariation);
}

e2d::EActionOpacityBy * e2d::EActionOpacityBy::reverse() const
{
	return new EActionOpacityBy(m_fTotalDuration / 1000, -m_nVariation);
}