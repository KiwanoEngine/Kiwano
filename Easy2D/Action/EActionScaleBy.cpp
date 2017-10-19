#include "..\eactions.h"


e2d::EActionScaleBy::EActionScaleBy(float duration, float scaleX, float scaleY) :
	EActionGradual(duration)
{
	m_nVariationX = scaleX;
	m_nVariationY = scaleY;
}

void e2d::EActionScaleBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginScaleX = m_pTarget->getScaleX();
		m_nBeginScaleY = m_pTarget->getScaleY();
	}
}

void e2d::EActionScaleBy::_callOn()
{
	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}
	while (EActionGradual::_isDelayEnough())
	{
		// 计算移动位置
		float scale = static_cast<float>(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pTarget->setScale(m_nBeginScaleX + m_nVariationX * scale, m_nBeginScaleX + m_nVariationX * scale);
		// 判断动作是否结束
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionScaleBy::_reset()
{
	EActionGradual::_reset();
}

e2d::EActionScaleBy * e2d::EActionScaleBy::clone() const
{
	return new EActionScaleBy(m_nAnimationInterval / 1000.0f, m_nVariationX, m_nVariationY);
}

e2d::EActionScaleBy * e2d::EActionScaleBy::reverse() const
{
	return new EActionScaleBy(m_nTotalDuration / 1000.0f, -m_nVariationX, -m_nVariationY);
}