#include "..\eactions.h"


e2d::ActionScaleBy::ActionScaleBy(float duration, float scaleX, float scaleY) :
	Animation(duration)
{
	m_nVariationX = scaleX;
	m_nVariationY = scaleY;
}

e2d::ActionScaleBy::~ActionScaleBy()
{
}

void e2d::ActionScaleBy::_init()
{
	Animation::_init();
	m_nBeginScaleX = m_pTarget->getScaleX();
	m_nBeginScaleY = m_pTarget->getScaleY();
}

void e2d::ActionScaleBy::_exec()
{
	while (Animation::_isDelayEnough())
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

void e2d::ActionScaleBy::_reset()
{
	Animation::_reset();
}

e2d::ActionScaleBy * e2d::ActionScaleBy::copy() const
{
	return new ActionScaleBy(m_nAnimationInterval / 1000.0f, m_nVariationX, m_nVariationY);
}

e2d::ActionScaleBy * e2d::ActionScaleBy::reverse() const
{
	return new ActionScaleBy(m_nTotalDuration / 1000.0f, -m_nVariationX, -m_nVariationY);
}