#include "..\eactions.h"


e2d::EActionOpacityBy::EActionOpacityBy(float duration, float opacity) :
	EAnimation(duration)
{
	m_nVariation = opacity;
}

void e2d::EActionOpacityBy::_init()
{
	EAnimation::_init();
	m_nBeginVal = m_pTarget->getOpacity();
}

void e2d::EActionOpacityBy::_callOn()
{
	while (EAnimation::_isDelayEnough())
	{
		// 计算移动位置
		float scale = static_cast<float>(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pTarget->setOpacity(m_nBeginVal + m_nVariation * scale);
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
	EAnimation::_reset();
}

e2d::EActionOpacityBy * e2d::EActionOpacityBy::clone() const
{
	return new EActionOpacityBy(m_nAnimationInterval / 1000.0f, m_nVariation);
}

e2d::EActionOpacityBy * e2d::EActionOpacityBy::reverse() const
{
	return new EActionOpacityBy(m_nTotalDuration / 1000.0f, -m_nVariation);
}