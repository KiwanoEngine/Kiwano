#include "..\easy2d.h"

ActionScaleBy::ActionScaleBy(float duration, float scaleX, float scaleY) :
	Animation(duration)
{
	m_nVariationX = scaleX;
	m_nVariationY = scaleY;
}

ActionScaleBy::~ActionScaleBy()
{
}

void ActionScaleBy::_init()
{
	Animation::_init();
	m_nBeginScaleX = m_pParent->getScaleX();
	m_nBeginScaleY = m_pParent->getScaleY();
}

bool ActionScaleBy::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	while (Animation::_exec(nNow))
	{
		// 计算移动位置
		float scale = float(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pParent->setScale(m_nBeginScaleX + m_nVariationX * scale, m_nBeginScaleX + m_nVariationX * scale);
		// 判断动作是否结束
		if (_isEnd())
		{
			return true;
		}
	}
	return false;
}

void ActionScaleBy::_reset()
{
	Animation::_reset();
}

ActionScaleBy * ActionScaleBy::copy() const
{
	return new ActionScaleBy(m_nMilliSeconds / 1000.0f, m_nVariationX, m_nVariationY);
}

ActionScaleBy * ActionScaleBy::reverse() const
{
	return new ActionScaleBy(m_nTotalDuration / 1000.0f, -m_nVariationX, -m_nVariationY);
}