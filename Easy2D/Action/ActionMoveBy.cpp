#include "..\eactions.h"


e2d::ActionMoveBy::ActionMoveBy(float duration, EVec vector) :
	Animation(duration)
{
	m_MoveVector = vector;
}

e2d::ActionMoveBy::~ActionMoveBy()
{
}

void e2d::ActionMoveBy::_init()
{
	Animation::_init();
	m_BeginPos = m_pTarget->getPos();
}

void e2d::ActionMoveBy::_exec()
{
	while (Animation::_isDelayEnough())
	{
		// 计算移动位置
		float scale = static_cast<float>(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pTarget->setPos(
			m_BeginPos.x + m_MoveVector.x * scale, 
			m_BeginPos.y + m_MoveVector.y * scale
		);
		// 判断动作是否结束
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::ActionMoveBy::_reset()
{
	Animation::_reset();
}

e2d::ActionMoveBy * e2d::ActionMoveBy::copy() const
{
	return new ActionMoveBy(m_nAnimationInterval / 1000.0f, m_MoveVector);
}

e2d::ActionMoveBy * e2d::ActionMoveBy::reverse() const
{
	return new ActionMoveBy(m_nTotalDuration / 1000.0f, EVec(-m_MoveVector.x, -m_MoveVector.y));
}