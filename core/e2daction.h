#pragma once
#include "e2dnode.h"

namespace e2d
{


class ActionManager;
class ActionBase;
class ActionMoveBy;
class ActionMoveTo;
class ActionScaleBy;
class ActionScaleTo;
class ActionOpacityBy;
class ActionOpacityTo;
class ActionFadeIn;
class ActionFadeOut;
class ActionRotateBy;
class ActionRotateTo;
class ActionTwo;
class ActionDelay;
class ActionLoop;
class ActionFunc;
class ActionSequence;
class Animation;

// 动作生成器
class Action
{
public:
	// 创建相对位移动画
	static e2d::ActionMoveBy* MoveBy(
		double duration,	/* 动画持续时长 */
		Vector vector		/* 位移向量 */
	);

	// 创建位移动画
	static e2d::ActionMoveTo* MoveTo(
		double duration,	/* 动画持续时长 */
		Point pos			/* 位移至目标点的坐标 */
	);

	// 创建相对缩放动画
	static e2d::ActionScaleBy* ScaleBy(
		double duration,	/* 动画持续时长 */
		double scale		/* 缩放比例变化 */
	);

	// 创建相对缩放动画
	static e2d::ActionScaleBy* ScaleBy(
		double duration,	/* 动画持续时长 */
		double scaleX,		/* 横向缩放比例变化 */
		double scaleY		/* 纵向缩放比例变化 */
	);

	// 创建缩放动画
	static e2d::ActionScaleTo* ScaleTo(
		double duration,	/* 动画持续时长 */
		double scale		/* 缩放至目标比例 */
	);

	// 创建缩放动画
	static e2d::ActionScaleTo* ScaleTo(
		double duration,	/* 动画持续时长 */
		double scaleX,		/* 横向缩放至目标比例 */
		double scaleY		/* 纵向缩放至目标比例 */
	);

	// 创建透明度相对渐变动画
	static e2d::ActionOpacityBy* OpacityBy(
		double duration,	/* 动画持续时长 */
		double opacity		/* 透明度相对变化值 */
	);

	// 创建透明度渐变动画
	static e2d::ActionOpacityTo* OpacityTo(
		double duration,	/* 动画持续时长 */
		double opacity		/* 透明度渐变至目标值 */
	);

	// 创建淡入动画
	static e2d::ActionFadeIn* FadeIn(
		double duration		/* 动画持续时长 */
	);

	// 创建淡出动画
	static e2d::ActionFadeOut* FadeOut(
		double duration		/* 动画持续时长 */
	);

	// 创建相对旋转动画
	static e2d::ActionRotateBy* RotateBy(
		double duration,	/* 动画持续时长 */
		double rotation		/* 旋转角度变化值 */
	);

	// 创建旋转动画
	static e2d::ActionRotateTo* RotateTo(
		double duration,	/* 动画持续时长 */
		double rotation		/* 旋转角度至目标值 */
	);

	// 创建两个动作的连续动作
	static e2d::ActionTwo* Two(
		ActionBase * pActionFirst,		/* 第一个动作 */
		ActionBase * pActionSecond,		/* 第二个动作 */
		bool bAtSameTime = false		/* 同时开始 */
	);

	// 创建延时动作
	static e2d::ActionDelay* Delay(
		double duration		/* 延迟时长（秒） */
	);

	// 创建循环动作
	static e2d::ActionLoop* Loop(
		ActionBase * action,	/* 执行循环的动作 */
		int times = -1			/* 循环次数 */
	);

	// 创建执行函数对象的动作
	static e2d::ActionFunc* Func(
		const Function& func		/* 函数对象 */
	);

#ifdef HIGHER_THAN_VS2012
	// 创建顺序动作
	static e2d::ActionSequence* Sequence(
		const std::initializer_list<ActionBase*>& vActions	/* 动作列表 */
	);

	// 创建特定帧间隔的帧动画
	static e2d::Animation* Animation(
		double interval,									/* 帧间隔（秒） */
		const std::initializer_list<Image*>& vFrames		/* 关键帧列表 */
	);
#else
	// 创建顺序动作
	static e2d::ActionSequence* Sequence(
		int number,			/* 动作数量 */
		ActionBase * action1,	/* 第一个动作 */
		...
	);

	// 创建特定帧间隔的帧动画
	static e2d::Animation* Animation(
		double interval,	/* 帧间隔（秒） */
		int number,			/* 帧数量 */
		Image * frame,		/* 第一帧 */
		...
	);
#endif
};


// 基础动作
class ActionBase :
	public Object
{
	friend ActionManager;
	friend ActionTwo;
	friend ActionLoop;
	friend ActionSequence;

public:
	ActionBase();

	virtual ~ActionBase();

	// 获取动作运行状态
	virtual bool isRunning();

	// 开始动作
	virtual void startWithTarget(
		Node* target	/* 执行该动作的目标 */
	);

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

	// 获取一个新的逆向动作
	virtual ActionBase * reverse() const;

	// 获取一个新的拷贝动作
	virtual ActionBase * clone() const = 0;

	// 重置动作
	virtual void reset();

	// 获取该动作的执行目标
	virtual Node * getTarget();

	// 销毁对象
	virtual void destroy() override;

protected:
	// 初始化动作
	virtual void _init();

	// 执行动作
	virtual void _update();

	// 获取动作结束状态
	virtual bool _isDone();

	// 重置动画时间
	virtual void _resetTime();

protected:
	String	m_sName;
	bool	m_bRunning;
	bool	m_bEnding;
	bool	m_bInit;
	Node *	m_pTarget;
	Scene * m_pParentScene;
	double	m_fLast;
};


// 持续动作
class ActionGradual :
	public ActionBase
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
	double m_fDuration;
	double m_fRateOfProgress;
};


// 相对位移动画
class ActionMoveBy :
	public ActionGradual
{
public:
	// 创建相对位移动画
	ActionMoveBy(
		double duration,	/* 动画持续时长 */
		Vector vector		/* 位移向量 */
	);

	// 获取该动画的拷贝对象
	virtual ActionMoveBy * clone() const override;

	// 获取该动画的逆动画
	virtual ActionMoveBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	Point		m_BeginPos;
	Vector	m_MoveVec;
};


// 位移动画
class ActionMoveTo :
	public ActionMoveBy
{
public:
	// 创建位移动画
	ActionMoveTo(
		double duration,	/* 动画持续时长 */
		Point pos			/* 位移至目标点的坐标 */
	);

	// 获取该动画的拷贝对象
	virtual ActionMoveTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	Point m_EndPos;
};


// 相对缩放动画
class ActionScaleBy :
	public ActionGradual
{
public:
	// 创建相对缩放动画
	ActionScaleBy(
		double duration,	/* 动画持续时长 */
		double scale		/* 缩放比例变化 */
	);

	// 创建相对缩放动画
	ActionScaleBy(
		double duration,	/* 动画持续时长 */
		double scaleX,		/* 横向缩放比例变化 */
		double scaleY		/* 纵向缩放比例变化 */
	);

	// 获取该动画的拷贝对象
	virtual ActionScaleBy * clone() const override;

	// 获取该动画的逆动画
	virtual ActionScaleBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	double	m_nBeginScaleX;
	double	m_nBeginScaleY;
	double	m_nVariationX;
	double	m_nVariationY;
};


// 缩放动画
class ActionScaleTo :
	public ActionScaleBy
{
public:
	// 创建缩放动画
	ActionScaleTo(
		double duration,	/* 动画持续时长 */
		double scale		/* 缩放至目标比例 */
	);

	// 创建缩放动画
	ActionScaleTo(
		double duration,	/* 动画持续时长 */
		double scaleX,		/* 横向缩放至目标比例 */
		double scaleY		/* 纵向缩放至目标比例 */
	);

	// 获取该动画的拷贝对象
	virtual ActionScaleTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	double	m_nEndScaleX;
	double	m_nEndScaleY;
};


// 透明度相对渐变动画
class ActionOpacityBy :
	public ActionGradual
{
public:
	// 创建透明度相对渐变动画
	ActionOpacityBy(
		double duration,	/* 动画持续时长 */
		double opacity		/* 透明度相对变化值 */
	);

	// 获取该动画的拷贝对象
	virtual ActionOpacityBy * clone() const override;

	// 获取该动画的逆动画
	virtual ActionOpacityBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	double m_nBeginVal;
	double m_nVariation;
};


// 透明度渐变动画
class ActionOpacityTo :
	public ActionOpacityBy
{
public:
	// 创建透明度渐变动画
	ActionOpacityTo(
		double duration,	/* 动画持续时长 */
		double opacity		/* 透明度渐变至目标值 */
	);

	// 获取该动画的拷贝对象
	virtual ActionOpacityTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	double m_nEndVal;
};


// 淡入动画
class ActionFadeIn :
	public ActionOpacityTo
{
public:
	// 创建淡入动画
	ActionFadeIn(
		double duration	/* 动画持续时长 */
	) : ActionOpacityTo(duration, 1) {}
};


// 淡出动画
class ActionFadeOut :
	public ActionOpacityTo
{
public:
	// 创建淡出动画
	ActionFadeOut(
		double duration	/* 动画持续时长 */
	) : ActionOpacityTo(duration, 0) {}
};


// 相对旋转动作
class ActionRotateBy :
	public ActionGradual
{
public:
	// 创建相对旋转动画
	ActionRotateBy(
		double duration,	/* 动画持续时长 */
		double rotation		/* 旋转角度变化值 */
	);

	// 获取该动画的拷贝对象
	virtual ActionRotateBy * clone() const override;

	// 获取该动画的逆动画
	virtual ActionRotateBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _update() override;

protected:
	double m_nBeginVal;
	double m_nVariation;
};


// 旋转动作
class ActionRotateTo :
	public ActionRotateBy
{
public:
	// 创建旋转动画
	ActionRotateTo(
		double duration,	/* 动画持续时长 */
		double rotation		/* 旋转角度至目标值 */
	);

	// 获取该动画的拷贝对象
	virtual ActionRotateTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

protected:
	double m_nEndVal;
};


// 组合动作
class ActionTwo :
	public ActionBase
{
public:
	// 创建两个动作的连续动作
	ActionTwo(
		ActionBase * pActionFirst,		/* 第一个动作 */
		ActionBase * pActionSecond,		/* 第二个动作 */
		bool bAtSameTime = false	/* 同时开始 */
	);

	virtual ~ActionTwo();

	// 获取该动作的拷贝对象
	virtual ActionTwo * clone() const override;

	// 获取该动作的逆动作
	virtual ActionTwo * reverse(
		bool actionReverse = true	/* 子动作是否执行逆动作 */
	) const;

	// 重置动作
	virtual void reset() override;

	// 销毁对象
	virtual void destroy() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

	// 重置动画时间
	virtual void _resetTime() override;

protected:
	ActionBase*	m_pFirstAction;
	ActionBase*	m_pSecondAction;
	bool	m_bAtSameTime;
};


// 顺序动作
class ActionSequence :
	public ActionBase
{
public:
	// 创建顺序动作
	ActionSequence();

#ifdef HIGHER_THAN_VS2012
	// 创建顺序动作
	ActionSequence(
		const std::initializer_list<ActionBase*>& vActions	/* 动作列表 */
	);
#else
	// 创建顺序动作
	ActionSequence(
		int number,			/* 动作数量 */
		ActionBase * action,	/* 第一个动作 */
		...
	);
#endif

	virtual ~ActionSequence();

	// 在结尾添加动作
	void add(
		ActionBase * action
	);

#ifdef HIGHER_THAN_VS2012
	// 在结尾添加多个动作
	void add(
		const std::initializer_list<ActionBase*>& vActions	/* 动作列表 */
	);
#else
	// 在结尾添加多个动作
	void add(
		int number,			/* 动作数量 */
		ActionBase * action,	/* 第一个动作 */
		...
	);
#endif

	// 获取该动作的拷贝对象
	virtual ActionSequence * clone() const override;

	// 获取该动作的逆动作
	virtual ActionSequence * reverse(
		bool actionReverse = true	/* 子动作是否执行逆动作 */
	) const;

	// 重置动作
	virtual void reset() override;

	// 销毁对象
	virtual void destroy() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

	// 重置动画时间
	virtual void _resetTime() override;

protected:
	UINT					m_nActionIndex;
	std::vector<ActionBase*>	m_vActions;
};


// 延时动作
class ActionDelay :
	public ActionBase
{
public:
	// 创建延时动作
	ActionDelay(
		double duration	/* 延迟时长（秒） */
	);

	// 获取该动作的拷贝对象
	virtual ActionDelay * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

protected:
	double m_fDelayTime;
};


// 循环动作
class ActionLoop :
	public ActionBase
{
public:
	// 创建循环动作
	ActionLoop(
		ActionBase * action,	/* 执行循环的动作 */
		int times = -1		/* 循环次数 */
	);

	virtual ~ActionLoop();

	// 获取该动作的拷贝对象
	virtual ActionLoop * clone() const override;

	// 重置动作
	virtual void reset() override;

	// 销毁对象
	virtual void destroy() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

	// 重置动画时间
	virtual void _resetTime() override;

protected:
	ActionBase * m_pAction;
	int m_nTimes;
	int m_nTotalTimes;
};


// 帧动画
class Animation :
	public ActionBase
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

	// 获取该动画的逆动画
	virtual Animation * reverse() const override;

	// 重置动作
	virtual void reset() override;

	// 销毁对象
	virtual void destroy() override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

protected:
	double	m_fInterval;
	UINT	m_nFrameIndex;
	std::vector<Image*> m_vFrames;
};


// 回调动作
class ActionFunc :
	public ActionBase
{
public:
	// 创建执行函数对象的动作
	ActionFunc(
		const Function& func /* 函数对象 */
	);

	// 获取该动作的拷贝对象
	virtual ActionFunc * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _update() override;

protected:
	Function m_Callback;
};


}