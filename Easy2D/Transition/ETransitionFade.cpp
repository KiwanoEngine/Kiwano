#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\emanagers.h"

e2d::ETransitionFade::ETransitionFade(float fadeOutDuration, float fadeInDuration)
	: m_fFadeOutDuration(fadeOutDuration)
	, m_fFadeInDuration(fadeInDuration)
{
}

void e2d::ETransitionFade::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	// 初始化场景属性
	next->getRoot()->setOpacity(0);

	// 第一个场景淡出
	auto action1 = new EActionFadeOut(m_fFadeOutDuration);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}
	
	// 第二个场景淡入
	auto action2 = new EActionFadeIn(m_fFadeInDuration);
	action2->setTarget(next->getRoot());

	// 标志动画结束
	auto action3 = new EActionCallback([&, prev, next] {
		transitional = false;
		// 还原场景状态
		if (prev)
		{
			prev->getRoot()->setOpacity(1);
		}
		next->getRoot()->setOpacity(1);
	});

	// 添加顺序动作
	EActionManager::addAction(new EActionSequence(3, action1, action2, action3));
}