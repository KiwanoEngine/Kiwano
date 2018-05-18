#pragma once
#include "e2dnode.h"

namespace e2d
{


class ActionManager;
class Loop;
class Sequence;
class Spawn;


// 基础动作
class Action :
	public Object
{
	friend ActionManager;
	friend Loop;
	friend Sequence;
	friend Spawn;

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
	virtual String getName() const;

	// 设置动作名称
	virtual void setName(
		const String& name
	);

	// 获取动作的拷贝
	virtual Action * clone() const = 0;

	// 获取动作的倒转
	virtual Action * reverse() const = 0;

	// 重置动作
	virtual void reset();

	// 获取该动作的执行目标
	virtual Node * getTarget();

	// 销毁对象
	virtual void onDestroy() override;

protected:
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
	double	_last;
};


// 持续动作
class ActionGradual :
	public Action
{
public:
	// 创建特定时长的持续动作
	ActionGradual(
		double duration
	);

	// 重置动作
	virtual void reset() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	double _duration;
	double _delta;
};


// 相对位移动作
class MoveBy :
	public ActionGradual
{
public:
	MoveBy(
		double duration,	/* 持续时长 */
		Vector vector		/* 移动距离 */
	);

	// 创建相对位移动作
	static MoveBy * create(
		double duration,	/* 持续时长 */
		Vector vector		/* 移动距离 */
	);

	// 获取该动作的拷贝对象
	virtual MoveBy * clone() const override;

	// 获取该动作的倒转
	virtual MoveBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	Point	_startPos;
	Vector	_deltaPos;
};


// 位移动作
class MoveTo :
	public MoveBy
{
public:
	MoveTo(
		double duration,	/* 持续时长 */
		Point pos			/* 目的坐标 */
	);

	// 创建位移动作
	static MoveTo * create(
		double duration,	/* 持续时长 */
		Point pos			/* 目的坐标 */
	);

	// 获取该动作的拷贝对象
	virtual MoveTo * clone() const override;

	// 获取该动作的倒转
	virtual MoveTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in MoveTo");
		return nullptr;
	}

protected:
	// 初始化动作
	virtual void _init() override;

protected:
	Point _endPos;
};


// 相对跳跃动作
class JumpBy :
	public ActionGradual
{
public:
	JumpBy(
		double duration,		/* 持续时长 */
		const Vector& vec,		/* 跳跃距离 */
		double height,			/* 跳跃高度 */
		int jumps				/* 跳跃次数 */
	);

	// 创建相对跳跃动作
	static JumpBy * create(
		double duration,		/* 持续时长 */
		const Vector& vec,		/* 跳跃距离 */
		double height,			/* 跳跃高度 */
		int jumps				/* 跳跃次数 */
	);

	// 获取该动作的拷贝对象
	virtual JumpBy * clone() const override;

	// 获取该动作的倒转
	virtual JumpBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	Point	_startPos;
	Vector	_deltaPos;
	double	_height;
	int		_jumps;
	Point	_prevPos;
};


// 跳跃动作
class JumpTo :
	public JumpBy
{
public:
	JumpTo(
		double duration,		/* 持续时长 */
		const Point& pos,		/* 目的坐标 */
		double height,			/* 跳跃高度 */
		int jumps				/* 跳跃次数 */
	);

	// 创建位移动作
	static JumpTo * create(
		double duration,		/* 持续时长 */
		const Point& pos,		/* 目的坐标 */
		double height,			/* 跳跃高度 */
		int jumps				/* 跳跃次数 */
	);

	// 获取该动作的拷贝对象
	virtual JumpTo * clone() const override;

	// 获取该动作的倒转
	virtual JumpTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in JumpTo");
		return nullptr;
	}

protected:
	// 初始化动作
	virtual void _init() override;

protected:
	Point _endPos;
};


// 相对缩放动作
class ScaleBy :
	public ActionGradual
{
public:
	ScaleBy(
		double duration,	/* 持续时长 */
		double scale		/* 相对变化值 */
	);

	ScaleBy(
		double duration,	/* 持续时长 */
		double scaleX,		/* 横向缩放相对变化值 */
		double scaleY		/* 纵向缩放相对变化值 */
	);

	// 创建相对缩放动作
	static ScaleBy * create(
		double duration,	/* 持续时长 */
		double scale		/* 相对变化值 */
	);

	// 创建相对缩放动作
	static ScaleBy * create(
		double duration,	/* 持续时长 */
		double scaleX,		/* 横向缩放相对变化值 */
		double scaleY		/* 纵向缩放相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleBy * clone() const override;

	// 获取该动作的倒转
	virtual ScaleBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	double	_startScaleX;
	double	_startScaleY;
	double	_deltaX;
	double	_deltaY;
};


// 缩放动作
class ScaleTo :
	public ScaleBy
{
public:
	ScaleTo(
		double duration,	/* 持续时长 */
		double scale		/* 目标值 */
	);

	ScaleTo(
		double duration,	/* 持续时长 */
		double scaleX,		/* 横向缩放目标值 */
		double scaleY		/* 纵向缩放目标值 */
	);

	// 创建缩放动作
	static ScaleTo * create(
		double duration,	/* 持续时长 */
		double scale		/* 目标值 */
	);

	// 创建缩放动作
	static ScaleTo * create(
		double duration,	/* 持续时长 */
		double scaleX,		/* 横向缩放目标值 */
		double scaleY		/* 纵向缩放目标值 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleTo * clone() const override;

	// 获取该动作的倒转
	virtual ScaleTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in ScaleTo");
		return nullptr;
	}

protected:
	// 初始化动作
	virtual void _init() override;

protected:
	double	_endScaleX;
	double	_endScaleY;
};


// 透明度相对渐变动作
class OpacityBy :
	public ActionGradual
{
public:
	OpacityBy(
		double duration,	/* 持续时长 */
		double opacity		/* 相对变化值 */
	);

	// 创建透明度相对渐变动作
	static OpacityBy * create(
		double duration,	/* 持续时长 */
		double opacity		/* 相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual OpacityBy * clone() const override;

	// 获取该动作的倒转
	virtual OpacityBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	double _startVal;
	double _deltaVal;
};


// 透明度渐变动作
class OpacityTo :
	public OpacityBy
{
public:
	OpacityTo(
		double duration,	/* 持续时长 */
		double opacity		/* 目标值 */
	);

	// 创建透明度渐变动作
	static OpacityTo * create(
		double duration,	/* 持续时长 */
		double opacity		/* 目标值 */
	);

	// 获取该动作的拷贝对象
	virtual OpacityTo * clone() const override;

	// 获取该动作的倒转
	virtual OpacityTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in OpacityTo");
		return nullptr;
	}

protected:
	// 初始化动作
	virtual void _init() override;

protected:
	double _endVal;
};


// 淡入动作
class FadeIn :
	public OpacityTo
{
public:
	// 创建淡入动作
	FadeIn(
		double duration		/* 持续时长 */
	)
	: OpacityTo(duration, 1) 
	{
	}
};


// 淡出动作
class FadeOut :
	public OpacityTo
{
public:
	// 创建淡出动作
	FadeOut(
		double duration		/* 持续时长 */
	)
	: OpacityTo(duration, 0) 
	{
	}
};


// 相对旋转动作
class RotateBy :
	public ActionGradual
{
public:
	RotateBy(
		double duration,	/* 持续时长 */
		double rotation		/* 相对变化值 */
	);

	// 创建相对旋转动作
	static RotateBy * create(
		double duration,	/* 持续时长 */
		double rotation		/* 相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual RotateBy * clone() const override;

	// 获取该动作的倒转
	virtual RotateBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	double _startVal;
	double _deltaVal;
};


// 旋转动作
class RotateTo :
	public RotateBy
{
public:
	RotateTo(
		double duration,	/* 持续时长 */
		double rotation		/* 目标值 */
	);

	// 创建旋转动作
	static RotateTo * create(
		double duration,	/* 持续时长 */
		double rotation		/* 目标值 */
	);

	// 获取该动作的拷贝对象
	virtual RotateTo * clone() const override;

	// 获取该动作的倒转
	virtual RotateTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in RotateTo");
		return nullptr;
	}

protected:
	// 初始化动作
	virtual void _init() override;

protected:
	double _endVal;
};


// 延时动作
class Delay :
	public Action
{
public:
	Delay(
		double duration	/* 延迟时长（秒） */
	);

	// 创建延时动作
	static Delay * create(
		double duration	/* 延迟时长（秒） */
	);

	// 获取该动作的拷贝对象
	virtual Delay * clone() const override;

	// 获取该动作的倒转
	virtual Delay * reverse() const override;

	// 重置动作
	virtual void reset() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	double _delay;
	double _delta;
};


// 循环动作
class Loop :
	public Action
{
public:
	Loop(
		Action * action,	/* 执行循环的动作 */
		int times = -1		/* 循环次数 */
	);

	// 创建循环动作
	static Loop * create(
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

	// 销毁对象
	virtual void onDestroy() override;

protected:
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
	CallFunc(
		const Function& func /* 函数对象 */
	);

	// 创建执行函数对象的动作
	static CallFunc * create(
		const Function& func /* 函数对象 */
	);

	// 获取该动作的拷贝对象
	virtual CallFunc * clone() const override;

	// 获取该动作的倒转
	virtual CallFunc * reverse() const override;

protected:
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

	Sequence(
		const std::vector<Action*>& actions	/* 动作列表 */
	);

	// 创建顺序动作
	static Sequence * create();

	// 创建顺序动作
	static Sequence * create(
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

	// 销毁对象
	virtual void onDestroy() override;

protected:
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

	Spawn(
		const std::vector<Action*>& actions	/* 动作列表 */
	);

	// 创建同步动作
	static Spawn * create();

	// 创建同步动作
	static Spawn * create(
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

	// 销毁对象
	virtual void onDestroy() override;

protected:
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
	public Object
{
public:
	Animation();

	Animation(
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	Animation(
		double interval						/* 帧间隔（秒） */
	);

	Animation(
		double interval,					/* 帧间隔（秒） */
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	// 创建帧动画
	static Animation * create();

	// 创建帧动画
	static Animation * create(
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	// 创建特定帧间隔的帧动画
	static Animation * create(
		double interval						/* 帧间隔（秒） */
	);

	// 创建特定帧间隔的帧动画
	static Animation * create(
		double interval,					/* 帧间隔（秒） */
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	virtual ~Animation();

	// 添加关键帧
	void add(
		Image * frame	/* 关键帧 */
	);

	// 添加多个关键帧
	void add(
		const std::vector<Image*>& frames	/* 关键帧列表 */
	);

	// 获取帧间隔
	double getInterval() const;

	// 获取关键帧
	const std::vector<Image*>& getFrames() const;

	// 设置每一帧的时间间隔
	void setInterval(
		double interval		/* 帧间隔（秒） */
	);

	// 获取动画的拷贝对象
	virtual Animation * clone() const;

	// 销毁对象
	virtual void onDestroy() override;

protected:
	double	_interval;
	std::vector<Image*> _frames;
};


// 精灵动作
class Animate :
	public Action
{
public:
	Animate();

	Animate(
		Animation * animation
	);

	// 创建精灵动作
	static Animate * create();

	// 创建精灵动作
	static Animate * create(
		Animation * animation
	);

	virtual ~Animate();

	// 获取动画
	virtual Animation * getAnimation() const;

	// 设置动画
	virtual void setAnimation(
		Animation * animation
	);

	// 获取该动作的拷贝对象
	virtual Animate * clone() const override;

	// 获取该动作的倒转
	virtual Animate * reverse() const override;

	// 重置动作
	virtual void reset() override;

	// 销毁对象
	virtual void onDestroy() override;

protected:
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