#pragma once
#include "e2dcommon.h"
#include "e2dbase.h"

namespace e2d
{


class Node;
class Loop;
class Sequence;
class Spawn;
class ActionManager;


// 基础动作
class Action :
	public Ref
{
	friend class ActionManager;
	friend class Loop;
	friend class Sequence;
	friend class Spawn;

public:
	Action();

	virtual ~Action();

	// 获取动作运行状态
	virtual bool isRunning();

	// 继续动作
	virtual void resume();

	// 暂停动作
	virtual void pause();

	// 停止动作
	virtual void stop();

	// 获取动作名称
	virtual const String& name() const;

	// 设置动作名称
	virtual void name(
		const String& name
	);

	// 获取动作的拷贝
	virtual Action * clone() const = 0;

	// 获取动作的倒转
	virtual Action * reverse() const = 0;

	// 重置动作
	virtual void reset();

	// 获取该动作的执行目标
	virtual Node * target();

protected:
	E2D_DISABLE_COPY(Action);

	// 初始化动作
	virtual void _init();

	// 更新动作
	virtual void _update();

	// 获取动作结束状态
	virtual bool _isDone();

	// 重置动作时间
	virtual void _resetTime();

	// 开始动作
	virtual void _startWithTarget(
		Node* target
	);

protected:
	String	_name;
	bool	_running;
	bool	_done;
	bool	_initialized;
	Node *	_target;
	Time	_started;
};


// 持续动作
class FiniteTimeAction :
	public Action
{
public:
	// 创建特定时长的持续动作
	explicit FiniteTimeAction(
		float duration
	);

	// 重置动作
	virtual void reset() override;

protected:
	E2D_DISABLE_COPY(FiniteTimeAction);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	float _duration;
	float _delta;
};


// 相对位移动作
class MoveBy :
	public FiniteTimeAction
{
public:
	explicit MoveBy(
		float duration,		/* 持续时长 */
		Vector2 vector		/* 移动距离 */
	);

	// 获取该动作的拷贝对象
	virtual MoveBy * clone() const override;

	// 获取该动作的倒转
	virtual MoveBy * reverse() const override;

protected:
	E2D_DISABLE_COPY(MoveBy);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	Point	_startPos;
	Point	_prevPos;
	Vector2	_deltaPos;
};


// 位移动作
class MoveTo :
	public MoveBy
{
public:
	explicit MoveTo(
		float duration,		/* 持续时长 */
		Point pos			/* 目的坐标 */
	);

	// 获取该动作的拷贝对象
	virtual MoveTo * clone() const override;

	// 获取该动作的倒转
	virtual MoveTo * reverse() const override
	{
		WARN("reverse() not supported in MoveTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(MoveTo);

	// 初始化动作
	virtual void _init() override;

protected:
	Point _endPos;
};


// 相对跳跃动作
class JumpBy :
	public FiniteTimeAction
{
public:
	explicit JumpBy(
		float duration,		/* 持续时长 */
		const Vector2& vec,		/* 跳跃距离 */
		float height,			/* 跳跃高度 */
		int jumps = 1			/* 跳跃次数 */
	);

	// 获取该动作的拷贝对象
	virtual JumpBy * clone() const override;

	// 获取该动作的倒转
	virtual JumpBy * reverse() const override;

protected:
	E2D_DISABLE_COPY(JumpBy);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	Point	_startPos;
	Vector2	_deltaPos;
	float	_height;
	int		_jumps;
	Point	_prevPos;
};


// 跳跃动作
class JumpTo :
	public JumpBy
{
public:
	explicit JumpTo(
		float duration,		/* 持续时长 */
		const Point& pos,		/* 目的坐标 */
		float height,			/* 跳跃高度 */
		int jumps = 1			/* 跳跃次数 */
	);

	// 获取该动作的拷贝对象
	virtual JumpTo * clone() const override;

	// 获取该动作的倒转
	virtual JumpTo * reverse() const override
	{
		WARN("reverse() not supported in JumpTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(JumpTo);

	// 初始化动作
	virtual void _init() override;

protected:
	Point _endPos;
};


// 相对缩放动作
class ScaleBy :
	public FiniteTimeAction
{
public:
	explicit ScaleBy(
		float duration,		/* 持续时长 */
		float scale			/* 相对变化值 */
	);

	explicit ScaleBy(
		float duration,		/* 持续时长 */
		float scaleX,		/* 横向缩放相对变化值 */
		float scaleY		/* 纵向缩放相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleBy * clone() const override;

	// 获取该动作的倒转
	virtual ScaleBy * reverse() const override;

protected:
	E2D_DISABLE_COPY(ScaleBy);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	float	_startScaleX;
	float	_startScaleY;
	float	_deltaX;
	float	_deltaY;
};


// 缩放动作
class ScaleTo :
	public ScaleBy
{
public:
	explicit ScaleTo(
		float duration,		/* 持续时长 */
		float scale			/* 目标值 */
	);

	explicit ScaleTo(
		float duration,		/* 持续时长 */
		float scaleX,		/* 横向缩放目标值 */
		float scaleY		/* 纵向缩放目标值 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleTo * clone() const override;

	// 获取该动作的倒转
	virtual ScaleTo * reverse() const override
	{
		WARN("reverse() not supported in ScaleTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(ScaleTo);

	// 初始化动作
	virtual void _init() override;

protected:
	float	_endScaleX;
	float	_endScaleY;
};


// 透明度相对渐变动作
class OpacityBy :
	public FiniteTimeAction
{
public:
	explicit OpacityBy(
		float duration,		/* 持续时长 */
		float opacity		/* 相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual OpacityBy * clone() const override;

	// 获取该动作的倒转
	virtual OpacityBy * reverse() const override;

protected:
	E2D_DISABLE_COPY(OpacityBy);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	float _startVal;
	float _deltaVal;
};


// 透明度渐变动作
class OpacityTo :
	public OpacityBy
{
public:
	explicit OpacityTo(
		float duration,		/* 持续时长 */
		float opacity		/* 目标值 */
	);

	// 获取该动作的拷贝对象
	virtual OpacityTo * clone() const override;

	// 获取该动作的倒转
	virtual OpacityTo * reverse() const override
	{
		WARN("reverse() not supported in OpacityTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(OpacityTo);

	// 初始化动作
	virtual void _init() override;

protected:
	float _endVal;
};


// 淡入动作
class FadeIn :
	public OpacityTo
{
public:
	// 创建淡入动作
	explicit FadeIn(
		float duration		/* 持续时长 */
	)
	: OpacityTo(duration, 1) 
	{
	}

protected:
	E2D_DISABLE_COPY(FadeIn);
};


// 淡出动作
class FadeOut :
	public OpacityTo
{
public:
	// 创建淡出动作
	explicit FadeOut(
		float duration		/* 持续时长 */
	)
	: OpacityTo(duration, 0) 
	{
	}

protected:
	E2D_DISABLE_COPY(FadeOut);
};


// 相对旋转动作
class RotateBy :
	public FiniteTimeAction
{
public:
	explicit RotateBy(
		float duration,		/* 持续时长 */
		float rotation		/* 相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual RotateBy * clone() const override;

	// 获取该动作的倒转
	virtual RotateBy * reverse() const override;

protected:
	E2D_DISABLE_COPY(RotateBy);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	float _startVal;
	float _deltaVal;
};


// 旋转动作
class RotateTo :
	public RotateBy
{
public:
	explicit RotateTo(
		float duration,		/* 持续时长 */
		float rotation		/* 目标值 */
	);

	// 获取该动作的拷贝对象
	virtual RotateTo * clone() const override;

	// 获取该动作的倒转
	virtual RotateTo * reverse() const override
	{
		WARN("reverse() not supported in RotateTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(RotateTo);

	// 初始化动作
	virtual void _init() override;

protected:
	float _endVal;
};


// 延时动作
class Delay :
	public Action
{
public:
	explicit Delay(
		float duration	/* 延迟时长（秒） */
	);

	// 获取该动作的拷贝对象
	virtual Delay * clone() const override;

	// 获取该动作的倒转
	virtual Delay * reverse() const override;

	// 重置动作
	virtual void reset() override;

protected:
	E2D_DISABLE_COPY(Delay);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	float _delay;
	float _delta;
};


// 循环动作
class Loop :
	public Action
{
public:
	explicit Loop(
		Action * action,	/* 执行循环的动作 */
		int times = -1		/* 循环次数 */
	);

	virtual ~Loop();

	// 获取该动作的拷贝对象
	virtual Loop * clone() const override;

	// 获取该动作的倒转
	virtual Loop * reverse() const override;

	// 重置动作
	virtual void reset() override;

protected:
	E2D_DISABLE_COPY(Loop);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	Action * _action;
	int _times;
	int _totalTimes;
};


// 回调动作
class CallFunc :
	public Action
{
public:
	explicit CallFunc(
		const Function& func /* 函数对象 */
	);

	// 获取该动作的拷贝对象
	virtual CallFunc * clone() const override;

	// 获取该动作的倒转
	virtual CallFunc * reverse() const override;

protected:
	E2D_DISABLE_COPY(CallFunc);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	Function _func;
};


// 顺序动作
class Sequence :
	public Action
{
public:
	Sequence();

	explicit Sequence(
		const std::vector<Action*>& actions	/* 动作列表 */
	);

	virtual ~Sequence();

	// 在结尾添加动作
	void add(
		Action * action
	);

	// 在结尾添加多个动作
	void add(
		const std::vector<Action*>& actions	/* 动作列表 */
	);

	// 获取该动作的拷贝对象
	virtual Sequence * clone() const override;

	// 获取该动作的倒转
	virtual Sequence * reverse() const;

	// 重置动作
	virtual void reset() override;

protected:
	E2D_DISABLE_COPY(Sequence);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	UINT _currIndex;
	std::vector<Action*> _actions;
};


// 同步动作
class Spawn :
	public Action
{
public:
	Spawn();

	explicit Spawn(
		const std::vector<Action*>& actions	/* 动作列表 */
	);

	virtual ~Spawn();

	// 在结尾添加动作
	void add(
		Action * action
	);

	// 在结尾添加多个动作
	void add(
		const std::vector<Action*>& actions	/* 动作列表 */
	);

	// 获取该动作的拷贝对象
	virtual Spawn * clone() const override;

	// 获取该动作的倒转
	virtual Spawn * reverse() const;

	// 重置动作
	virtual void reset() override;

protected:
	E2D_DISABLE_COPY(Spawn);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	std::vector<Action*> _actions;
};


// 帧动画
class Animation :
	public Ref
{
public:
	Animation();

	explicit Animation(
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	explicit Animation(
		float interval						/* 帧间隔（秒） */
	);

	explicit Animation(
		float interval,						/* 帧间隔（秒） */
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	virtual ~Animation();

	// 添加关键帧
	Animation& add(
		Image * frame	/* 关键帧 */
	);

	// 添加多个关键帧
	Animation& add(
		const std::vector<Image*>& frames	/* 关键帧列表 */
	);

	// 设置每一帧的时间间隔
	Animation& interval(
		float interval		/* 帧间隔（秒） */
	);

	// 获取帧间隔
	float interval() const;

	// 获取关键帧
	const std::vector<Image*>& frames() const;

	// 获取帧动画的拷贝对象
	Animation * clone() const;

	// 获取帧动画的倒转
	Animation * reverse() const;

protected:
	E2D_DISABLE_COPY(Animation);

protected:
	float	_interval;
	std::vector<Image*> _frames;
};


// 精灵动作
class Animate :
	public Action
{
public:
	Animate();

	explicit Animate(
		Animation * animation
	);

	virtual ~Animate();

	// 获取动画
	Animation * animation() const;

	// 设置动画
	void animation(
		Animation * animation
	);

	// 获取该动作的拷贝对象
	virtual Animate * clone() const override;

	// 获取该动作的倒转
	virtual Animate * reverse() const override;

	// 重置动作
	virtual void reset() override;

protected:
	E2D_DISABLE_COPY(Animate);

	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	UINT _frameIndex;
	Animation * _animation;
};


}