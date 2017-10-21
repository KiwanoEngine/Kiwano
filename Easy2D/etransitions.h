#pragma once
#include "ebase.h"

namespace e2d
{

class ETransition :
	public EObject
{
	friend EApp;
public:
	ETransition() { this->autoRelease(); }

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
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
		bool &transitional
	) override;

protected:
	float m_fFadeOutDuration;
	float m_fFadeInDuration;
};


class ETransitionEmerge :
	public ETransition
{
public:
	// 创建浮现式的场景切换动画
	ETransitionEmerge(
		float emergeDuration	/* 浮现动画持续时长 */
	);

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fEmergeDuration;
};


class ETransitionMove :
	public ETransition
{
public:
	enum MOVE_DIRECT
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	// 创建移动式的场景切换动画
	ETransitionMove(
		float moveDuration,	/* 场景移动动画持续时长 */
		MOVE_DIRECT direct	/* 场景移动方向 */
	);

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fMoveDuration;
	MOVE_DIRECT m_Direct;
};


class ETransitionScale :
	public ETransition
{
public:
	// 创建缩放式的场景切换动画
	ETransitionScale(
		float scaleOutDuration,	/* 第一个场景缩放动画持续时长 */
		float scaleInDuration	/* 第二个场景缩放动画持续时长 */
	);

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fScaleOutDuration;
	float m_fScaleInDuration;
};


class ETransitionScaleEmerge :
	public ETransition
{
public:
	enum SCALE_EMERGE_MODE
	{
		ENTER,
		BACK
	};

	// 创建缩放浮现式的场景切换动画
	ETransitionScaleEmerge(
		float duration,			/* 场景动画持续时长 */
		SCALE_EMERGE_MODE mode	/* 场景移动方向 */
	);

protected:
	// 保存当前场景和下一场景的指针，和控制场景切换的变量
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fDuration;
	SCALE_EMERGE_MODE m_Mode;
};

}