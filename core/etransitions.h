#pragma once
#include "ecommon.h"

namespace e2d
{


class SceneManager;

class Transition :
	public Obj
{
	friend SceneManager;

public:
	Transition(float duration);

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
		Scene * prev,
		Scene * next
	);

protected:
	bool m_bEnd;
	float m_fLast;
	float m_fDuration;
	float m_fRateOfProgress;
	Scene * m_pPrevScene;
	Scene * m_pNextScene;
};


class TransitionFade :
	public Transition
{
public:
	// 创建淡入淡出式的场景切换动画
	TransitionFade(
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


class TransitionEmerge :
	public Transition
{
public:
	// 创建浮现式的场景切换动画
	TransitionEmerge(
		float duration	/* 浮现动画持续时长 */
	);

protected:
	// 更新动画
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;
};


class TransitionMove :
	public Transition
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
	TransitionMove(
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
	Vector m_Vec;
	Point m_NextPos;
};

}