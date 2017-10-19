#pragma once
#include "ebase.h"

namespace e2d
{

class ETransition :
	public EObject
{
	friend EApp;
public:
	ETransition() {}

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &enterNextScene,
		bool &transitional
	) = 0;
};


class ETransitionFade :
	public ETransition
{
public:
	// 创建淡入淡出式的场景切换动画
	ETransitionFade(
		float fadeOutDuration,	/* 前一场景淡出动画持续时长 */
		float fadeInDuration	/* 后一场景淡入动画持续时长 */
	);

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &enterNextScene,
		bool &transitional
	) override;

protected:
	float m_fFadeOutDuration;
	float m_fFadeInDuration;
};

}