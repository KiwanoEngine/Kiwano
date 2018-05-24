#pragma once
#include "e2dcommon.h"

namespace e2d
{


class SceneManager;


// 场景过渡
class Transition :
	public Object
{
	friend class SceneManager;

public:
	explicit Transition(double duration);

	virtual ~Transition();

	// 场景过渡动画是否结束
	bool isDone();

	// 销毁对象
	virtual void onDestroy() override;

protected:
	// 初始化场景过渡动画
	virtual void _init(
		Scene * prev,
		Scene * next
	);

	// 更新场景过渡动画
	virtual void _update();

	// 更新场景过渡动画
	virtual void _updateCustom() = 0;

	// 渲染场景过渡动画
	virtual void _render();

	// 重置场景过渡动画
	virtual void _reset() = 0;

	// 停止场景过渡动画
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


// 淡入淡出过渡
class FadeTransition :
	public Transition
{
public:
	explicit FadeTransition(
		double duration	/* 动画持续时长 */
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


// 渐变过渡
class EmergeTransition :
	public Transition
{
public:
	explicit EmergeTransition(
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


// 盒状过渡
class BoxTransition :
	public Transition
{
public:
	explicit BoxTransition(
		double duration	/* 动画持续时长 */
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


// 移入过渡
class MoveTransition :
	public Transition
{
public:
	explicit MoveTransition(
		double moveDuration,					/* 场景移动动画持续时长 */
		Direction direction = Direction::LEFT	/* 场景移动方向 */
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
	Direction _direction;
	Vector _posDelta;
	Point _startPos;
};

}