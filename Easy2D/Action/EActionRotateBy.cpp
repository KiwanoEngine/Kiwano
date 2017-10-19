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
		m_nBeginVal = m_pTarget->getOpacity();
	}
}

void e2d::EActionRotateBy::_callOn()
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
		m_pTarget->setRotation(m_nBeginVal + m_nVariation * scale);
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
	return new EActionRotateBy(m_nAnimationInterval / 1000.0f, m_nVariation);
}

e2d::EActionRotateBy * e2d::EActionRotateBy::reverse() const
{
	return new EActionRotateBy(m_nTotalDuration / 1000.0f, -m_nVariation);
}