#pragma once
#include "e2dcommon.h"

namespace e2d
{


class Scene;

// 场景过渡
class Transition :
	public Ref
{
public:
	explicit Transition(float duration);

	virtual ~Transition();

	// 场景过渡动画是否结束
	bool isDone();

	// 初始化场景过渡动画
	virtual bool init(
		Scene * prev,
		Scene * next
	);

	// 更新场景过渡动画
	virtual void update();

	// 渲染场景过渡动画
	virtual void render();

	// 停止场景过渡动画
	virtual void stop();

	// 重置场景过渡动画
	virtual void reset() = 0;

protected:
	bool	_end;
	float	_duration;
	float	_delta;
	Time	_started;
	Size	_windowSize;
	Scene*	_outScene;
	Scene*	_inScene;
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
		float duration	/* 动画持续时长 */
	);

	// 更新动画
	virtual void update() override;

	virtual bool init(
		Scene * prev,
		Scene * next
	) override;

	virtual void reset() override;
};


// 渐变过渡
class EmergeTransition :
	public Transition
{
public:
	explicit EmergeTransition(
		float duration	/* 浮现动画持续时长 */
	);

	// 更新动画
	virtual void update() override;

	virtual bool init(
		Scene * prev,
		Scene * next
	) override;

	virtual void reset() override;
};


// 盒状过渡
class BoxTransition :
	public Transition
{
public:
	explicit BoxTransition(
		float duration	/* 动画持续时长 */
	);

	// 更新动画
	virtual void update() override;

	virtual bool init(
		Scene * prev,
		Scene * next
	) override;

	virtual void reset() override;
};


// 移入过渡
class MoveTransition :
	public Transition
{
public:
	explicit MoveTransition(
		float moveDuration,					/* 场景移动动画持续时长 */
		Direction direction = Direction::Left	/* 场景移动方向 */
	);

	// 更新动画
	virtual void update() override;

	virtual bool init(
		Scene * prev,
		Scene * next
	) override;

	virtual void reset() override;

protected:
	Direction _direction;
	Vector2 _posDelta;
	Point _startPos;
};

}