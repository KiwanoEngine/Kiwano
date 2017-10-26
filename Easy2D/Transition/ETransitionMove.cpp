#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\emanagers.h"

e2d::ETransitionMove::ETransitionMove(float moveDuration, MOVE_DIRECT direct)
	: m_fMoveDuration(moveDuration)
	, m_Direct(direct)
{
}

void e2d::ETransitionMove::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	// 初始化动作属性
	float distPosX;
	float distPosY;
	if (m_Direct == MOVE_DIRECT::UP)
	{
		distPosX = 0;
		distPosY = -EApp::getHeight();
	}
	else if (m_Direct == MOVE_DIRECT::DOWN)
	{
		distPosX = 0;
		distPosY = EApp::getHeight();
	}
	else if (m_Direct == MOVE_DIRECT::LEFT)
	{
		distPosX = -EApp::getWidth();
		distPosY = 0;
	}
	else if (m_Direct == MOVE_DIRECT::RIGHT)
	{
		distPosX = EApp::getWidth();
		distPosY = 0;
	}

	// 初始化场景属性
	next->getRoot()->move(-distPosX, -distPosY);

	// 第一个场景移出
	auto action1 = new EActionMoveBy(m_fMoveDuration, EVec(distPosX, distPosY));
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}

	// 第二个场景移入
	auto action2 = new EActionMoveBy(m_fMoveDuration, EVec(distPosX, distPosY));
	action2->setTarget(next->getRoot());

	// 标志动画结束
	auto action3 = new EActionCallback([&, prev, next] {
		transitional = false;
		// 还原场景状态
		if (prev)
		{
			prev->getRoot()->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		}
		next->getRoot()->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
	});

	// 添加顺序动作
	EActionManager::addAction(new EActionSequence(2, new EActionTwoAtSameTime(action1, action2), action3));
}
