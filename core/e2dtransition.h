#pragma once
#include "e2dcommon.h"

namespace e2d
{


class Game;
class Scene;

// 场景过渡
class Transition :
	public Ref
{
	friend class Game;

public:
	explicit Transition(
		Scene* scene,
		float duration
	);

	virtual ~Transition();

	// 场景过渡动画是否结束
	bool done();

protected:
	// 初始化场景过渡动画
	virtual bool _init(
		Game * game,
		Scene * prev
	);

	// 更新场景过渡动画
	virtual void _update();

	// 渲染场景过渡动画
	virtual void _render(
		Game * game
	);

	// 停止场景过渡动画
	virtual void _stop();

	// 重置场景过渡动画
	virtual void _reset() { };

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
		Scene* scene,		/* 切换的场景 */
		float duration		/* 动画持续时长 */
	);

protected:
	// 更新动画
	virtual void _update() override;

	virtual bool _init(
		Game * game,
		Scene * prev
	) override;
};


// 渐变过渡
class EmergeTransition :
	public Transition
{
public:
	explicit EmergeTransition(
		Scene* scene,		/* 切换的场景 */
		float duration		/* 浮现动画持续时长 */
	);

protected:
	virtual void _update() override;

	virtual bool _init(
		Game * game,
		Scene * prev
	) override;
};


// 盒状过渡
class BoxTransition :
	public Transition
{
public:
	explicit BoxTransition(
		Scene* scene,		/* 切换的场景 */
		float duration		/* 动画持续时长 */
	);

protected:
	virtual void _update() override;

	virtual bool _init(
		Game * game,
		Scene * prev
	) override;
};


// 移入过渡
class MoveTransition :
	public Transition
{
public:
	explicit MoveTransition(
		Scene* scene,							/* 切换的场景 */
		float moveDuration,						/* 场景移动动画持续时长 */
		Direction direction = Direction::Left	/* 场景移动方向 */
	);

protected:
	virtual void _update() override;

	virtual bool _init(
		Game * game,
		Scene * prev
	) override;

	virtual void _reset() override;

protected:
	Direction _direction;
	Vector2 _posDelta;
	Point _startPos;
};

}