#include "..\easy2d.h"

ActionOpacityBy::ActionOpacityBy(float duration, float opacity) :
	Animation(duration)
{
	m_nVariation = opacity;
}

ActionOpacityBy::~ActionOpacityBy()
{
}

void ActionOpacityBy::_init()
{
	Animation::_init();
	m_nBeginVal = m_pTargetSprite->getOpacity();
}

bool ActionOpacityBy::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	while (Animation::_exec(nNow))
	{
		// 计算移动位置
		float scale = float(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pTargetSprite->setOpacity(m_nBeginVal + m_nVariation * scale);
		// 判断动作是否结束
		if (_isEnd())
		{
			return true;
		}
	}
	return false;
}

void ActionOpacityBy::_reset()
{
	Animation::_reset();
}

ActionOpacityBy * ActionOpacityBy::copy() const
{
	return new ActionOpacityBy(m_nMilliSeconds / 1000.0f, m_nVariation);
}

ActionOpacityBy * ActionOpacityBy::reverse() const
{
	return new ActionOpacityBy(m_nTotalDuration / 1000.0f, -m_nVariation);
}