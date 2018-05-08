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
	virtual void onDestroy() override;

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
	bool _bEnd;
	double _fLast;
	double _duration;
	double _delta;
	Size _WindowSize;
	Scene * _pPrevScene;
	Scene * _pNextScene;
	ID2D1Layer * _pPrevLayer;
	ID2D1Layer * _pNextLayer;
	D2D1_LAYER_PARAMETERS _sPrevLayerParam;
	D2D1_LAYER_PARAMETERS _sNextLayerParam;
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
	double _fFadeOutDuration;
	double _fFadeInDuration;
	bool _bFadeOutTransioning;
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
	Direct _Direct;
	Vector _Vector;
	Point _NextPos;
};

}