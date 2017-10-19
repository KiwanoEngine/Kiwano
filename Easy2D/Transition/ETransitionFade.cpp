#include "..\etransitions.h"
#include "..\eactions.h"

e2d::ETransitionFade::ETransitionFade(float fadeOutDuration, float fadeInDuration)
	: m_fFadeOutDuration(fadeOutDuration)
	, m_fFadeInDuration(fadeInDuration)
{
}

void e2d::ETransitionFade::_setTarget(EScene * prev, EScene * next, bool & enterNextScene, bool & transitional)
{
	// 第一个场景淡出
	auto action1 = new EActionFadeOut(m_fFadeOutDuration);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}

	// 切换至第二场景，并将第二场景透明度设为 0
	auto action2 = new EActionCallback([&, next] {
		enterNextScene = true;
		next->getRoot()->setOpacity(0);
	});
	
	// 第二个场景淡入
	auto action3 = new EActionFadeIn(m_fFadeInDuration);
	action3->setTarget(next->getRoot());

	// 标志动画结束
	auto action4 = new EActionCallback([&] {
		transitional = false;
	});

	// 添加顺序动作
	EActionManager::addAction(new EActionSequence(4, action1, action2, action3, action4));
}