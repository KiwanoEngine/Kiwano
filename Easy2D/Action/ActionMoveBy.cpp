#include "..\easy2d.h"
#include "..\Win\winbase.h"

ActionMoveBy::ActionMoveBy(float duration, CVector vec) :
	Animation(duration)
{
	m_MoveVector = vec;
}

ActionMoveBy::~ActionMoveBy()
{
}

void ActionMoveBy::_init()
{
	Animation::_init();
	m_BeginPos = m_pTargetSprite->getPos();
}

void ActionMoveBy::_exec(steady_clock::time_point nNow)
{
	if (Animation::_isDelayEnough(nNow))
	{
		// 计算移动位置
		float scale = float(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pTargetSprite->setPos(int(m_BeginPos.x + m_MoveVector.x * scale), 
			int(m_BeginPos.y + m_MoveVector.y * scale));
		// 判断动作是否结束
		if (_isEnd())
		{
			this->stop();
		}
	}
}

void ActionMoveBy::_reset()
{
	Animation::_reset();
}

ActionMoveBy * ActionMoveBy::copy() const
{
	return new ActionMoveBy(m_nAnimationInterval / 1000.0f, m_MoveVector);
}

ActionMoveBy * ActionMoveBy::reverse() const
{
	return new ActionMoveBy(m_nTotalDuration / 1000.0f, CVector(-m_MoveVector.x, -m_MoveVector.y));
}