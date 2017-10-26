#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\emanagers.h"

e2d::ETransitionScale::ETransitionScale(float scaleOutDuration, float scaleInDuration)
	: m_fScaleOutDuration(scaleOutDuration)
	, m_fScaleInDuration(scaleInDuration)
{
}

void e2d::ETransitionScale::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	// 初始化场景属性
	next->getRoot()->setScale(0);

	// 第一个场景缩放至消失
	auto action1 = new EActionScaleTo(m_fScaleOutDuration, 0);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}

	// 第二个场景淡入
	auto action2 = new EActionScaleTo(m_fScaleInDuration, 1);
	action2->setTarget(next->getRoot());

	// 标志动画结束
	auto action3 = new EActionCallback([&, prev, next] {
		transitional = false;
		// 还原场景状态
		if (prev)
		{
			prev->getRoot()->setScale(1);
		}
		next->getRoot()->setScale(1);
	});

	// 添加顺序动作
	EActionManager::addAction(new EActionSequence(3, action1, action2, action3));
}
