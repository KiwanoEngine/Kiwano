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
	virtual Action * reverse() const;

	// 重置动作
	virtual void reset();

	// 获取该动作的执行目标
	virtual Node * getTarget();

	// 销毁对象
	virtual void onDestroy() override;

protected:
	// 初始化动作
	virtual void _init();

	// 执行动作
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

protected:
	// 初始化动作
	virtual void _init() override;

	// 更新动作
	virtual void _update() override;

protected:
	double _duration;
	double _delta;
};


// 相对位移动作
class MoveBy :
	public ActionGradual
{
public:
	// 创建相对位移动作
	MoveBy(
		double duration,	/* 动作持续时长 */
		Vector vector		/* 位移向量 */
	);

	// 获取该动作的拷贝对象
	virtual MoveBy * clone() const override;

	// 获取该动作的倒转
	virtual MoveBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
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
	// 创建位移动作
	MoveTo(
		double duration,	/* 动作持续时长 */
		Point pos			/* 位移至目标点的坐标 */
	);

	// 获取该动作的拷贝对象
	virtual MoveTo * clone() const override;

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
	// 创建相对缩放动作
	ScaleBy(
		double duration,	/* 动作持续时长 */
		double scale		/* 缩放比例变化 */
	);

	// 创建相对缩放动作
	ScaleBy(
		double duration,	/* 动作持续时长 */
		double scaleX,		/* 横向缩放比例变化 */
		double scaleY		/* 纵向缩放比例变化 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleBy * clone() const override;

	// 获取该动作的倒转
	virtual ScaleBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
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
	// 创建缩放动作
	ScaleTo(
		double duration,	/* 动作持续时长 */
		double scale		/* 缩放至目标比例 */
	);

	// 创建缩放动作
	ScaleTo(
		double duration,	/* 动作持续时长 */
		double scaleX,		/* 横向缩放至目标比例 */
		double scaleY		/* 纵向缩放至目标比例 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleTo * clone() const override;

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
	// 创建透明度相对渐变动作
	OpacityBy(
		double duration,	/* 动作持续时长 */
		double opacity		/* 透明度相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual OpacityBy * clone() const override;

	// 获取该动作的倒转
	virtual OpacityBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
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
	// 创建透明度渐变动作
	OpacityTo(
		double duration,	/* 动作持续时长 */
		double opacity		/* 透明度渐变至目标值 */
	);

	// 获取该动作的拷贝对象
	virtual OpacityTo * clone() const override;

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
		double duration		/* 动作持续时长 */
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
		double duration		/* 动作持续时长 */
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
	// 创建相对旋转动作
	RotateBy(
		double duration,	/* 动作持续时长 */
		double rotation		/* 旋转角度变化值 */
	);

	// 获取该动作的拷贝对象
	virtual RotateBy * clone() const override;

	// 获取该动作的倒转
	virtual RotateBy * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
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
	// 创建旋转动作
	RotateTo(
		double duration,	/* 动作持续时长 */
		double rotation		/* 旋转角度至目标值 */
	);

	// 获取该动作的拷贝对象
	virtual RotateTo * clone() const override;

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
	// 创建延时动作
	Delay(
		double duration	/* 延迟时长（秒） */
	);

	// 获取该动作的拷贝对象
	virtual Delay * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

protected:
	double _delay;
};


// 循环动作
class Loop :
	public Action
{
public:
	// 创建循环动作
	Loop(
		Action * action,	/* 执行循环的动作 */
		int times = -1		/* 循环次数 */
	);

	virtual ~Loop();

	// 获取该动作的拷贝对象
	virtual Loop * clone() const override;

	// 重置动作
	virtual void reset() override;

	// 销毁对象
	virtual void onDestroy() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
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
	// 创建执行函数对象的动作
	CallFunc(
		const Function& func /* 函数对象 */
	);

	// 获取该动作的拷贝对象
	virtual CallFunc * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

protected:
	Function _func;
};


// 顺序动作
class Sequence :
	public Action
{
public:
	// 创建顺序动作
	Sequence();

	// 创建顺序动作
	Sequence(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
	);

#ifdef HIGHER_THAN_VS2012
	// 创建顺序动作
	Sequence(
		const std::initializer_list<Action*>& actions	/* 动作列表 */
	);
#endif

	virtual ~Sequence();

	// 在结尾添加动作
	void add(
		Action * action
	);

	// 在结尾添加多个动作
	void add(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
	);

#ifdef HIGHER_THAN_VS2012
	// 在结尾添加多个动作
	void add(
		const std::initializer_list<Action*>& actions	/* 动作列表 */
	);
#endif

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

	// 执行动作
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
	// 创建同步动作
	Spawn();

	// 创建同步动作
	Spawn(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
	);

#ifdef HIGHER_THAN_VS2012
	// 创建同步动作
	Spawn(
		const std::initializer_list<Action*>& actions	/* 动作列表 */
	);
#endif

	virtual ~Spawn();

	// 在结尾添加动作
	void add(
		Action * action
	);

	// 在结尾添加多个动作
	void add(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
	);

#ifdef HIGHER_THAN_VS2012
	// 在结尾添加多个动作
	void add(
		const std::initializer_list<Action*>& actions	/* 动作列表 */
	);
#endif

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

	// 执行动作
	virtual void _update() override;

	// 重置动作时间
	virtual void _resetTime() override;

protected:
	std::vector<Action*> _actions;
};


// 精灵动作
class Animate :
	public Action
{
public:
	// 创建精灵动作
	Animate();

	// 创建精灵动作
	Animate(
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

	// 执行动作
	virtual void _update() override;

protected:
	UINT _frameIndex;
	Animation * _animation;
};


}