#pragma once
#include "ecommon.h"

namespace e2d
{


class ESceneManager;

class ETransition :
	public EObject
{
	friend ESceneManager;

public:
	ETransition(float duration);

	// 场景切换动画是否结束
	bool isEnding();

protected:
	// 更新场景动画
	virtual void _update() = 0;

	// 初始化场景动画
	virtual void _init() = 0;

	// 重置场景动画
	virtual void _reset() = 0;

	// 停止场景动画
	virtual void _stop();

	// 计算场景动画进度
	void _calcRateOfProgress();

	// 保存当前场景和下一场景的指针
	void _setTarget(
		EScene * prev,
		EScene * next
	);

protected:
	bool m_bEnd;
	float m_fLast;
	float m_fDuration;
	float m_fRateOfProgress;
	EScene * m_pPrevScene;
	EScene * m_pNextScene;
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
	// 更新动画
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;

protected:
	float m_fFadeOutDuration;
	float m_fFadeInDuration;
	bool m_bFadeOutTransioning;
};


class ETransitionEmerge :
	public ETransition
{
public:
	// 创建浮现式的场景切换动画
	ETransitionEmerge(
		float duration	/* 浮现动画持续时长 */
	);

protected:
	// 更新动画
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;
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
		float moveDuration,			/* 场景移动动画持续时长 */
		MOVE_DIRECT direct = LEFT	/* 场景移动方向 */
	);

protected:
	// 更新动画
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;

protected:
	MOVE_DIRECT m_Direct;
	EVector2 m_Vec;
	EPoint m_NextPos;
};

}