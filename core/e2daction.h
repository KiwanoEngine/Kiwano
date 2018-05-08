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

	// 重置动画时间
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
	double	_fLast;
};


// 持续动作
class ActionGradual :
	public Action
{
public:
	// 创建特定时长的持续动画
	ActionGradual(
		double duration
	);

protected:
	// 初始化动画
	virtual void _init() override;

	// 更新动画
	virtual void _update() override;

protected:
	double _duration;
	double _delta;
};


// 相对位移动画
class MoveBy :
	public ActionGradual
{
public:
	// 创建相对位移动画
	MoveBy(
		double duration,	/* 动画持续时长 */
		Vector vector		/* 位移向量 */
	);

	// 获取该动画的拷贝对象
	virtual MoveBy * clone() const override;

	// 获取该动画的倒转
	virtual MoveBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	Point	_startPos;
	Vector	_deltaPos;
};


// 位移动画
class MoveTo :
	public MoveBy
{
public:
	// 创建位移动画
	MoveTo(
		double duration,	/* 动画持续时长 */
		Point pos			/* 位移至目标点的坐标 */
	);

	// 获取该动画的拷贝对象
	virtual MoveTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	Point _endPos;
};


// 相对缩放动画
class ScaleBy :
	public ActionGradual
{
public:
	// 创建相对缩放动画
	ScaleBy(
		double duration,	/* 动画持续时长 */
		double scale		/* 缩放比例变化 */
	);

	// 创建相对缩放动画
	ScaleBy(
		double duration,	/* 动画持续时长 */
		double scaleX,		/* 横向缩放比例变化 */
		double scaleY		/* 纵向缩放比例变化 */
	);

	// 获取该动画的拷贝对象
	virtual ScaleBy * clone() const override;

	// 获取该动画的倒转
	virtual ScaleBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	double	_startScaleX;
	double	_startScaleY;
	double	_deltaX;
	double	_deltaY;
};


// 缩放动画
class ScaleTo :
	public ScaleBy
{
public:
	// 创建缩放动画
	ScaleTo(
		double duration,	/* 动画持续时长 */
		double scale		/* 缩放至目标比例 */
	);

	// 创建缩放动画
	ScaleTo(
		double duration,	/* 动画持续时长 */
		double scaleX,		/* 横向缩放至目标比例 */
		double scaleY		/* 纵向缩放至目标比例 */
	);

	// 获取该动画的拷贝对象
	virtual ScaleTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	double	_endScaleX;
	double	_endScaleY;
};


// 透明度相对渐变动画
class OpacityBy :
	public ActionGradual
{
public:
	// 创建透明度相对渐变动画
	OpacityBy(
		double duration,	/* 动画持续时长 */
		double opacity		/* 透明度相对变化值 */
	);

	// 获取该动画的拷贝对象
	virtual OpacityBy * clone() const override;

	// 获取该动画的倒转
	virtual OpacityBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	double _startVal;
	double _deltaVal;
};


// 透明度渐变动画
class OpacityTo :
	public OpacityBy
{
public:
	// 创建透明度渐变动画
	OpacityTo(
		double duration,	/* 动画持续时长 */
		double opacity		/* 透明度渐变至目标值 */
	);

	// 获取该动画的拷贝对象
	virtual OpacityTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	double _endVal;
};


// 淡入动画
class FadeIn :
	public OpacityTo
{
public:
	// 创建淡入动画
	FadeIn(
		double duration		/* 动画持续时长 */
	)
	: OpacityTo(duration, 1) 
	{
	}
};


// 淡出动画
class FadeOut :
	public OpacityTo
{
public:
	// 创建淡出动画
	FadeOut(
		double duration		/* 动画持续时长 */
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
	// 创建相对旋转动画
	RotateBy(
		double duration,	/* 动画持续时长 */
		double rotation		/* 旋转角度变化值 */
	);

	// 获取该动画的拷贝对象
	virtual RotateBy * clone() const override;

	// 获取该动画的倒转
	virtual RotateBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
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
	// 创建旋转动画
	RotateTo(
		double duration,	/* 动画持续时长 */
		double rotation		/* 旋转角度至目标值 */
	);

	// 获取该动画的拷贝对象
	virtual RotateTo * clone() const override;

protected:
	// 初始化动画
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

	// 重置动画时间
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

#ifdef HIGHER_THAN_VS2012
	// 创建顺序动作
	Sequence(
		const std::initializer_list<Action*>& vActions	/* 动作列表 */
	);
#else
	// 创建顺序动作
	Sequence(
		int number,				/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
	);
#endif

	virtual ~Sequence();

	// 在结尾添加动作
	void add(
		Action * action
	);

#ifdef HIGHER_THAN_VS2012
	// 在结尾添加多个动作
	void add(
		const std::initializer_list<Action*>& vActions	/* 动作列表 */
	);
#else
	// 在结尾添加多个动作
	void add(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
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

	// 重置动画时间
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

#ifdef HIGHER_THAN_VS2012
	// 创建同步动作
	Spawn(
		const std::initializer_list<Action*>& vActions	/* 动作列表 */
	);
#else
	// 创建同步动作
	Spawn(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
	);
#endif

	virtual ~Spawn();

	// 在结尾添加动作
	void add(
		Action * action
	);

#ifdef HIGHER_THAN_VS2012
	// 在结尾添加多个动作
	void add(
		const std::initializer_list<Action*>& vActions	/* 动作列表 */
	);
#else
	// 在结尾添加多个动作
	void add(
		int number,			/* 动作数量 */
		Action * action,	/* 第一个动作 */
		...
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

	// 重置动画时间
	virtual void _resetTime() override;

protected:
	std::vector<Action*> _actions;
};


// 帧动画
class Animation :
	public Action
{
public:
	// 创建帧动画
	Animation();

	// 创建特定帧间隔的帧动画
	Animation(
		double interval		/* 帧间隔（秒） */
	);

#ifdef HIGHER_THAN_VS2012
	// 创建帧动画
	Animation(
		const std::initializer_list<Image*>& vImages	/* 关键帧列表 */
	);

	// 创建特定帧间隔的帧动画
	Animation(
		double interval,								/* 帧间隔（秒） */
		const std::initializer_list<Image*>& vImages	/* 关键帧列表 */
	);
#else
	// 创建帧动画
	Animation(
		int number,			/* 帧数量 */
		Image * frame,		/* 第一帧 */
		...
	);

	// 创建特定帧间隔的帧动画
	Animation(
		double interval,	/* 帧间隔（秒） */
		int number,			/* 帧数量 */
		Image * frame,		/* 第一帧 */
		...
	);
#endif

	virtual ~Animation();

	// 添加关键帧
	void add(
		Image * frame	/* 关键帧 */
	);

#ifdef HIGHER_THAN_VS2012
	// 添加多个关键帧
	void add(
		const std::initializer_list<Image*>& vImages	/* 关键帧列表 */
	);
#else
	// 添加多个关键帧
	void add(
		int number,			/* 帧数量 */
		Image * frame,		/* 第一帧 */
		...
	);
#endif

	// 设置每一帧的时间间隔
	void setInterval(
		double interval	/* 帧间隔（秒） */
	);

	// 获取该动画的拷贝对象
	virtual Animation * clone() const override;

	// 获取该动画的倒转
	virtual Animation * reverse() const override;

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
	double	_interval;
	UINT	_frameIndex;
	std::vector<Image*> _frames;
};


}