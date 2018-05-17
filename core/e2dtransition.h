#pragma once
#include "e2dcommon.h"

namespace e2d
{


class SceneManager;


// 基础过渡动作
class Transition :
	public Object
{
	friend SceneManager;

public:
	Transition(double duration);

	virtual ~Transition();

	// 场景切换动作是否结束
	bool isDone();

	// 销毁对象
	virtual void onDestroy() override;

protected:
	// 初始化场景动作
	virtual void _init(
		Scene * prev,
		Scene * next
	);

	// 更新场景动作
	virtual void _update();

	// 更新场景动作
	virtual void _updateCustom() = 0;

	// 渲染场景动作
	virtual void _render();

	// 重置场景动作
	virtual void _reset() = 0;

	// 停止场景动作
	virtual void _stop();

protected:
	bool _end;
	double _last;
	double _duration;
	double _delta;
	Size _windowSize;
	Scene * _outScene;
	Scene * _inScene;
	ID2D1Layer * _outLayer;
	ID2D1Layer * _inLayer;
	D2D1_LAYER_PARAMETERS _outLayerParam;
	D2D1_LAYER_PARAMETERS _inLayerParam;
};


class TransitionFade :
	public Transition
{
public:
	TransitionFade(
		double duration	/* 动作持续时长 */
	);

	TransitionFade(
		double fadeOutDuration,	/* 前一场景淡出动作持续时长 */
		double fadeInDuration	/* 后一场景淡入动作持续时长 */
	);

	// 创建淡入淡出式的场景切换动作
	static TransitionFade * create(
		double duration	/* 动作持续时长 */
	);

	// 创建淡入淡出式的场景切换动作
	static TransitionFade * create(
		double fadeOutDuration,	/* 前一场景淡出动作持续时长 */
		double fadeInDuration	/* 后一场景淡入动作持续时长 */
	);

protected:
	// 更新动作
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;

protected:
	double _fadeOutDuration;
	double _fadeInDuration;
	bool _fadeOutTransioning;
};


class TransitionEmerge :
	public Transition
{
public:
	TransitionEmerge(
		double duration	/* 浮现动作持续时长 */
	);

	// 创建浮现式的场景切换动作
	static TransitionEmerge * create(
		double duration	/* 浮现动作持续时长 */
	);

protected:
	// 更新动作
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;
};


class TransitionMove :
	public Transition
{
public:
	TransitionMove(
		double moveDuration,					/* 场景移动动作持续时长 */
		Direction direction = Direction::LEFT	/* 场景移动方向 */
	);

	// 创建移动式的场景切换动作
	static TransitionMove * create(
		double moveDuration,					/* 场景移动动作持续时长 */
		Direction direction = Direction::LEFT	/* 场景移动方向 */
	);

protected:
	// 更新动作
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;

protected:
	Direction _direction;
	Vector _posDelta;
	Point _startPos;
};

}