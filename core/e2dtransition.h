#pragma once
#include "e2dcommon.h"

namespace e2d
{


class SceneManager;
class TransitionEmerge;
class TransitionFade;
class TransitionMove;


// 场景过渡动画生成器
class Transition
{
public:
	// 创建淡入淡出式的场景切换动画
	static TransitionFade * Fade(
		double duration				/* 动画持续时长 */
	);

	// 创建淡入淡出式的场景切换动画
	static TransitionFade * Fade(
		double fadeOutDuration,		/* 前一场景淡出动画持续时长 */
		double fadeInDuration		/* 后一场景淡入动画持续时长 */
	);

	// 创建浮现式的场景切换动画
	static TransitionEmerge * Emerge(
		double duration				/* 动画持续时长 */
	);

	// 创建移动式的场景切换动画
	static TransitionMove * Move(
		double duration,				/* 动画持续时长 */
		Direct direct = Direct::LEFT	/* 场景移动方向 */
	);
};


// 基础过渡动画
class TransitionBase :
	public Object
{
	friend SceneManager;

public:
	TransitionBase(double duration);

	virtual ~TransitionBase();

	// 场景切换动画是否结束
	bool isDone();

	// 销毁对象
	virtual void destroy() override;

protected:
	// 初始化场景动画
	virtual void _init(
		Scene * prev,
		Scene * next
	);

	// 更新场景动画
	virtual void _update();

	// 更新场景动画
	virtual void _updateCustom() = 0;

	// 渲染场景动画
	virtual void _render();

	// 重置场景动画
	virtual void _reset() = 0;

	// 停止场景动画
	virtual void _stop();

protected:
	bool m_bEnd;
	double m_fLast;
	double m_fDuration;
	double m_fRateOfProgress;
	Size m_WindowSize;
	Scene * m_pPrevScene;
	Scene * m_pNextScene;
	ID2D1Layer * m_pPrevLayer;
	ID2D1Layer * m_pNextLayer;
	D2D1_LAYER_PARAMETERS m_sPrevLayerParam;
	D2D1_LAYER_PARAMETERS m_sNextLayerParam;
};


class TransitionFade :
	public TransitionBase
{
public:
	// 创建淡入淡出式的场景切换动画
	TransitionFade(
		double duration	/* 动画持续时长 */
	);

	// 创建淡入淡出式的场景切换动画
	TransitionFade(
		double fadeOutDuration,	/* 前一场景淡出动画持续时长 */
		double fadeInDuration	/* 后一场景淡入动画持续时长 */
	);

protected:
	// 更新动画
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;

protected:
	double m_fFadeOutDuration;
	double m_fFadeInDuration;
	bool m_bFadeOutTransioning;
};


class TransitionEmerge :
	public TransitionBase
{
public:
	// 创建浮现式的场景切换动画
	TransitionEmerge(
		double duration	/* 浮现动画持续时长 */
	);

protected:
	// 更新动画
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;
};


class TransitionMove :
	public TransitionBase
{
public:
	// 创建移动式的场景切换动画
	TransitionMove(
		double moveDuration,			/* 场景移动动画持续时长 */
		Direct direct = Direct::LEFT	/* 场景移动方向 */
	);

protected:
	// 更新动画
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;

protected:
	Direct m_Direct;
	Vector m_Vector;
	Point m_NextPos;
};

}