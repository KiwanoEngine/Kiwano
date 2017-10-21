#pragma once
#include "enodes.h"
#include <chrono>

namespace e2d
{

class EActionManager;
class EActionTwo;
class EActionLoop;
class EActionSequence;
class EActionTwoAtSameTime;
class ETransitionFade;

class EAction :
	public EObject
{
	friend EActionManager;
	friend EActionTwo;
	friend EActionLoop;
	friend EActionSequence;
	friend EActionTwoAtSameTime;

public:
	EAction();

	virtual ~EAction();

	// 获取动作运行状态
	virtual bool isRunning();

	// 继续动作
	virtual void start();

	// 继续动作
	virtual void resume();

	// 暂停动作
	virtual void pause();

	// 停止动作
	virtual void stop();

	// 获取一个新的拷贝动作
	virtual EAction * clone() const = 0;

	// 获取一个新的逆向动作
	virtual EAction * reverse() const;

	// 获取执行该动作的目标
	virtual ENode * getTarget();

	// 设置动作每一帧的时间间隔
	virtual void setInterval(
		LONGLONG milliSeconds
	);

	// 设置动作执行目标
	virtual void setTarget(
		ENode * node
	);

protected:
	// 初始化动作
	virtual void _init();

	// 执行动作
	virtual void _callOn() = 0;

	// 获取动作结束状态
	virtual bool _isEnding();

	// 重置动作
	virtual void _reset();

protected:
	bool		m_bRunning;
	bool		m_bEnding;
	bool		m_bInit;
	ENode *		m_pTarget;
	EScene *	m_pParentScene;
	LONGLONG	m_nAnimationInterval;
	std::chrono::steady_clock::time_point m_tLast;
};


class EActionGradual :
	public EAction
{
public:
	// 创建时长动画
	EActionGradual(
		float duration
	);

protected:
	// 判断动作是否结束
	bool _isEnd() const;

	// 判断延时是否足够
	bool _isDelayEnough();

	// 初始化动画
	virtual void _init() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float m_fDuration;
	float m_fTotalDuration;
	float m_fRateOfProgress;
};


class EActionMoveBy :
	public EActionGradual
{
public:
	// 创建相对位移动画
	EActionMoveBy(
		float duration, /* 动画持续时长 */
		EVec vector		/* 位移向量 */
	);

	// 获取该动画的拷贝对象
	virtual EActionMoveBy * clone() const override;

	// 获取该动画的逆动画
	virtual EActionMoveBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _callOn() override;

	// 重置动画
	virtual void _reset() override;

protected:
	EPoint	m_BeginPos;
	EVec	m_MoveVector;
};


class EActionMoveTo :
	public EActionMoveBy
{
public:
	// 创建位移动画
	EActionMoveTo(
		float duration, /* 动画持续时长 */
		EPoint pos		/* 位移至目标点的坐标 */
	);

	// 获取该动画的拷贝对象
	virtual EActionMoveTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 重置动画
	virtual void _reset() override;

protected:
	EPoint m_EndPos;
};


class EActionScaleBy :
	public EActionGradual
{
public:
	// 创建相对缩放动画
	EActionScaleBy(
		float duration, /* 动画持续时长 */
		float scale		/* 缩放比例变化 */
	);

	// 创建相对缩放动画
	EActionScaleBy(
		float duration, /* 动画持续时长 */
		float scaleX,	/* 横向缩放比例变化 */
		float scaleY	/* 纵向缩放比例变化 */
	);

	// 获取该动画的拷贝对象
	virtual EActionScaleBy * clone() const override;

	// 获取该动画的逆动画
	virtual EActionScaleBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _callOn() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float	m_nBeginScaleX;
	float	m_nBeginScaleY;
	float	m_nVariationX;
	float	m_nVariationY;
};


class EActionScaleTo :
	public EActionScaleBy
{
public:
	// 创建缩放动画
	EActionScaleTo(
		float duration, /* 动画持续时长 */
		float scale		/* 缩放至目标比例 */
	);

	// 创建缩放动画
	EActionScaleTo(
		float duration, /* 动画持续时长 */
		float scaleX,	/* 横向缩放至目标比例 */
		float scaleY	/* 纵向缩放至目标比例 */
	);

	// 获取该动画的拷贝对象
	virtual EActionScaleTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float	m_nEndScaleX;
	float	m_nEndScaleY;
};


class EActionOpacityBy :
	public EActionGradual
{
public:
	// 创建透明度相对渐变动画
	EActionOpacityBy(
		float duration, /* 动画持续时长 */
		float opacity	/* 透明度相对变化值 */
	);

	// 获取该动画的拷贝对象
	virtual EActionOpacityBy * clone() const override;

	// 获取该动画的逆动画
	virtual EActionOpacityBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _callOn() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float m_nBeginVal;
	float m_nVariation;
};


class EActionOpacityTo :
	public EActionOpacityBy
{
public:
	// 创建透明度渐变动画
	EActionOpacityTo(
		float duration,	/* 动画持续时长 */
		float opacity	/* 透明度渐变至目标值 */
	);

	// 获取该动画的拷贝对象
	virtual EActionOpacityTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float m_nEndVal;
};


class EActionFadeIn :
	public EActionOpacityTo
{
public:
	// 创建淡入动画
	EActionFadeIn(
		float duration	/* 动画持续时长 */
	) : EActionOpacityTo(duration, 1) {}
};


class EActionFadeOut :
	public EActionOpacityTo
{
public:
	// 创建淡出动画
	EActionFadeOut(
		float duration	/* 动画持续时长 */
	) : EActionOpacityTo(duration, 0) {}
};


class EActionRotateBy :
	public EActionGradual
{
public:
	// 创建相对旋转动画
	EActionRotateBy(
		float duration,	/* 动画持续时长 */
		float rotation	/* 旋转角度变化值 */
	);

	// 获取该动画的拷贝对象
	virtual EActionRotateBy * clone() const override;

	// 获取该动画的逆动画
	virtual EActionRotateBy * reverse() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 执行动画
	virtual void _callOn() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float m_nBeginVal;
	float m_nVariation;
};


class EActionRotateTo :
	public EActionRotateBy
{
public:
	// 创建旋转动画
	EActionRotateTo(
		float duration,	/* 动画持续时长 */
		float rotation	/* 旋转角度至目标值 */
	);

	// 获取该动画的拷贝对象
	virtual EActionRotateTo * clone() const override;

protected:
	// 初始化动画
	virtual void _init() override;

	// 重置动画
	virtual void _reset() override;

protected:
	float m_nEndVal;
};


class EActionTwo :
	public EAction
{
public:
	// 创建两个动作的连续动作
	EActionTwo(
		EAction * actionFirst,	/* 第一个动作 */
		EAction * actionSecond	/* 第二个动作 */
	);

	virtual ~EActionTwo();

	// 获取该动作的拷贝对象
	virtual EActionTwo * clone() const override;

	// 获取该动作的逆动作
	virtual EActionTwo * reverse(
		bool actionReverse = true	/* 子动作是否执行逆动作 */
	) const;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;

protected:
	EAction *	m_pFirstAction;
	EAction *	m_pSecondAction;
};


class EActionSequence :
	public EAction
{
public:
	// 创建顺序动作
	EActionSequence();

	// 创建顺序动作
	EActionSequence(
		int number,			/* 顺序动作数量 */
		EAction * action1,	/* 第一个动作 */
		...
	);

	virtual ~EActionSequence();

	// 向顺序动作中添加动作
	void addAction(
		EAction * action	/* 将动作添加至顺序动作尾部 */
	);

	// 获取该动作的拷贝对象
	virtual EActionSequence * clone() const override;

	// 获取该动作的逆动作
	virtual EActionSequence * reverse(
		bool actionReverse = true	/* 子动作是否执行逆动作 */
	) const;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;

protected:
	UINT					m_nActionIndex;
	std::vector<EAction*>	m_vActions;
};


class EActionDelay :
	public EAction
{
public:
	// 创建延时动作
	EActionDelay(
		float duration	/* 延迟时长（秒） */
	);

	// 获取该动作的拷贝对象
	virtual EActionDelay * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;
};


class EActionTwoAtSameTime :
	public EAction
{
public:
	// 创建两个动作同时执行的动作
	EActionTwoAtSameTime(
		EAction * actionFirst,	/* 第一个动作 */
		EAction * actionSecond	/* 第二个动作 */
	);

	virtual ~EActionTwoAtSameTime();

	// 获取该动作的拷贝对象
	virtual EActionTwoAtSameTime * clone() const override;

	// 获取该动作的逆动作
	virtual EActionTwoAtSameTime * reverse(
		bool actionReverse = true	/* 子动作是否执行逆动作 */
	) const;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;

protected:
	EAction *	m_pFirstAction;
	EAction *	m_pSecondAction;
};


class EActionLoop :
	public EAction
{
public:
	// 创建循环动作
	EActionLoop(
		EAction * action	/* 执行循环的动作 */
	);

	virtual ~EActionLoop();

	// 获取该动作的拷贝对象
	virtual EActionLoop * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;

protected:
	EAction * m_Action;
};


class EAnimation :
	public EAction
{
public:
	// 创建帧动画
	EAnimation();

	// 创建特定帧间隔的帧动画
	EAnimation(
		LONGLONG frameDelay	/* 帧间隔（毫秒） */
	);

	virtual ~EAnimation();

	// 添加帧
	void addFrame(
		ESpriteFrame * frame /* 添加帧 */
	);

	// 获取该动画的拷贝对象
	virtual EAnimation * clone() const override;

	// 获取该动画的逆动画
	virtual EAnimation * reverse() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;

protected:
	UINT m_nFrameIndex;
	EVector<ESpriteFrame*> m_vFrames;
};


class EActionCallback :
	public EAction
{
public:
	// 创建执行回调函数的动作
	EActionCallback(
		const std::function<void()> & callback /* 回调函数 */
	);

	// 获取该动作的拷贝对象
	virtual EActionCallback * clone() const override;

protected:
	// 初始化动作
	virtual void _init() override;

	// 执行动作
	virtual void _callOn() override;

	// 重置动作
	virtual void _reset() override;

protected:
	std::function<void()> m_Callback;
};

}