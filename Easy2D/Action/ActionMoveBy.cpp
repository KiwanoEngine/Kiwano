#include "..\easy2d.h"

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
	m_BeginPos = m_pParent->getPos();
}

bool ActionMoveBy::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;
	
	while (Animation::_exec(nNow))
	{
		// 计算移动位置
		float scale = float(m_nDuration) / m_nTotalDuration;
		// 移动 Sprite
		m_pParent->setPos(int(m_BeginPos.x + m_MoveVector.x * scale), 
			int(m_BeginPos.y + m_MoveVector.y * scale));
		// 判断动作是否结束
		if (m_nDuration >= m_nTotalDuration)
		{
			return true;
		}
	}
	return false;
}

void ActionMoveBy::_reset()
{
	Animation::_reset();
}

ActionMoveBy * ActionMoveBy::copy()
{
	auto a = new ActionMoveBy(*this);
	a->_reset();
	return a;
}

ActionMoveBy * ActionMoveBy::reverse() const
{
	return new ActionMoveBy(m_nTotalDuration / 1000.0f, CVector(-m_MoveVector.x, -m_MoveVector.y));
}