#pragma once
#include "e2dcommon.h"
#include "e2dmodule.h"

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
	virtual bool IsRunning();

	// 继续动作
	virtual void Resume();

	// 暂停动作
	virtual void Pause();

	// 停止动作
	virtual void Stop();

	// 获取动作名称
	virtual const String& GetName() const;

	// 设置动作名称
	virtual void SetName(
		const String& name
	);

	// 获取动作的拷贝
	virtual Action * Clone() const = 0;

	// 获取动作的倒转
	virtual Action * Reverse() const = 0;

	// 重置动作
	virtual void Reset();

	// 获取该动作的执行目标
	virtual Node * GetTarget();

	// 开始动作
	virtual void StartWithTarget(
		Node* target
	);

	// 初始化动作
	virtual void Init();

	// 更新动作
	virtual void Update();

	// 重置动作时间
	virtual void ResetTime();

	// 获取动作结束状态
	virtual bool IsDone() const;

protected:
	E2D_DISABLE_COPY(Action);

protected:
	String	name_;
	bool	running_;
	bool	done_;
	bool	initialized_;
	Node *	target_;
	Time	started_;
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
	virtual void Reset() override;

protected:
	E2D_DISABLE_COPY(FiniteTimeAction);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

	// 重置动作时间
	virtual void ResetTime() override;

protected:
	float duration_;
	float delta_;
};


// 相对位移动作
class MoveBy :
	public FiniteTimeAction
{
public:
	explicit MoveBy(
		float duration,		/* 持续时长 */
		Point vector		/* 移动距离 */
	);

	// 获取该动作的拷贝对象
	virtual MoveBy * Clone() const override;

	// 获取该动作的倒转
	virtual MoveBy * Reverse() const override;

protected:
	E2D_DISABLE_COPY(MoveBy);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

protected:
	Point	start_pos_;
	Point	prev_pos_;
	Point	delta_pos_;
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
	virtual MoveTo * Clone() const override;

	// 获取该动作的倒转
	virtual MoveTo * Reverse() const override
	{
		WARN("Reverse() not supported in MoveTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(MoveTo);

	// 初始化动作
	virtual void Init() override;

protected:
	Point end_pos_;
};


// 相对跳跃动作
class JumpBy :
	public FiniteTimeAction
{
public:
	explicit JumpBy(
		float duration,			/* 持续时长 */
		const Point& vec,		/* 跳跃距离 */
		float height,			/* 跳跃高度 */
		int jumps = 1			/* 跳跃次数 */
	);

	// 获取该动作的拷贝对象
	virtual JumpBy * Clone() const override;

	// 获取该动作的倒转
	virtual JumpBy * Reverse() const override;

protected:
	E2D_DISABLE_COPY(JumpBy);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

protected:
	Point	start_pos_;
	Point	delta_pos_;
	float	height_;
	int		jumps_;
	Point	prev_pos_;
};


// 跳跃动作
class JumpTo :
	public JumpBy
{
public:
	explicit JumpTo(
		float duration,			/* 持续时长 */
		const Point& pos,		/* 目的坐标 */
		float height,			/* 跳跃高度 */
		int jumps = 1			/* 跳跃次数 */
	);

	// 获取该动作的拷贝对象
	virtual JumpTo * Clone() const override;

	// 获取该动作的倒转
	virtual JumpTo * Reverse() const override
	{
		WARN("Reverse() not supported in JumpTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(JumpTo);

	// 初始化动作
	virtual void Init() override;

protected:
	Point end_pos_;
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
		float scale_x,		/* 横向缩放相对变化值 */
		float scale_y		/* 纵向缩放相对变化值 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleBy * Clone() const override;

	// 获取该动作的倒转
	virtual ScaleBy * Reverse() const override;

protected:
	E2D_DISABLE_COPY(ScaleBy);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

protected:
	float	start_scale_x_;
	float	start_scale_y_;
	float	delta_x_;
	float	delta_y_;
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
		float scale_x,		/* 横向缩放目标值 */
		float scale_y		/* 纵向缩放目标值 */
	);

	// 获取该动作的拷贝对象
	virtual ScaleTo * Clone() const override;

	// 获取该动作的倒转
	virtual ScaleTo * Reverse() const override
	{
		WARN("Reverse() not supported in ScaleTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(ScaleTo);

	// 初始化动作
	virtual void Init() override;

protected:
	float	end_scale_x_;
	float	end_scale_y_;
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
	virtual OpacityBy * Clone() const override;

	// 获取该动作的倒转
	virtual OpacityBy * Reverse() const override;

protected:
	E2D_DISABLE_COPY(OpacityBy);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

protected:
	float start_val_;
	float delta_val_;
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
	virtual OpacityTo * Clone() const override;

	// 获取该动作的倒转
	virtual OpacityTo * Reverse() const override
	{
		WARN("Reverse() not supported in OpacityTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(OpacityTo);

	// 初始化动作
	virtual void Init() override;

protected:
	float end_val_;
};


// 淡入动作
class FadeIn :
	public OpacityTo
{
public:
	// 创建淡入动作
	explicit FadeIn(
		float duration		/* 持续时长 */
	);

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
	);

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
	virtual RotateBy * Clone() const override;

	// 获取该动作的倒转
	virtual RotateBy * Reverse() const override;

protected:
	E2D_DISABLE_COPY(RotateBy);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

protected:
	float start_val_;
	float delta_val_;
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
	virtual RotateTo * Clone() const override;

	// 获取该动作的倒转
	virtual RotateTo * Reverse() const override
	{
		WARN("Reverse() not supported in RotateTo");
		return nullptr;
	}

protected:
	E2D_DISABLE_COPY(RotateTo);

	// 初始化动作
	virtual void Init() override;

protected:
	float end_val_;
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
	virtual Delay * Clone() const override;

	// 获取该动作的倒转
	virtual Delay * Reverse() const override;

	// 重置动作
	virtual void Reset() override;

protected:
	E2D_DISABLE_COPY(Delay);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

	// 重置动作时间
	virtual void ResetTime() override;

protected:
	float delay_;
	float delta_;
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
	virtual Loop * Clone() const override;

	// 获取该动作的倒转
	virtual Loop * Reverse() const override;

	// 重置动作
	virtual void Reset() override;

protected:
	E2D_DISABLE_COPY(Loop);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

	// 重置动作时间
	virtual void ResetTime() override;

protected:
	Action * action_;
	int times_;
	int total_times_;
};


// 回调动作
class Callback :
	public Action
{
public:
	explicit Callback(
		const Function& func /* 函数对象 */
	);

	// 获取该动作的拷贝对象
	virtual Callback * Clone() const override;

	// 获取该动作的倒转
	virtual Callback * Reverse() const override;

protected:
	E2D_DISABLE_COPY(Callback);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

protected:
	Function callback_;
};


// 顺序动作
class Sequence :
	public Action
{
public:
	typedef std::vector<Action*> Actions;

	Sequence();

	explicit Sequence(
		const Actions& actions	/* 动作列表 */
	);

	virtual ~Sequence();

	// 在结尾添加动作
	void Add(
		Action * action
	);

	// 在结尾添加多个动作
	void Add(
		const Actions& actions	/* 动作列表 */
	);

	// 获取该动作的拷贝对象
	virtual Sequence * Clone() const override;

	// 获取该动作的倒转
	virtual Sequence * Reverse() const;

	// 重置动作
	virtual void Reset() override;

protected:
	E2D_DISABLE_COPY(Sequence);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

	// 重置动作时间
	virtual void ResetTime() override;

protected:
	UINT action_index_;
	Actions actions_;
};


// 同步动作
class Spawn :
	public Action
{
public:
	typedef std::vector<Action*> Actions;

	Spawn();

	explicit Spawn(
		const Actions& actions	/* 动作列表 */
	);

	virtual ~Spawn();

	// 在结尾添加动作
	void Add(
		Action * action
	);

	// 在结尾添加多个动作
	void Add(
		const Actions& actions	/* 动作列表 */
	);

	// 获取该动作的拷贝对象
	virtual Spawn * Clone() const override;

	// 获取该动作的倒转
	virtual Spawn * Reverse() const;

	// 重置动作
	virtual void Reset() override;

protected:
	E2D_DISABLE_COPY(Spawn);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

	// 重置动作时间
	virtual void ResetTime() override;

protected:
	Actions actions_;
};


// 帧动画
class Animation :
	public Ref
{
public:
	typedef std::vector<Image*> Images;

	Animation();

	explicit Animation(
		const Images& frames	/* 关键帧数组 */
	);

	explicit Animation(
		float interval			/* 帧间隔（秒） */
	);

	explicit Animation(
		float interval,			/* 帧间隔（秒） */
		const Images& frames	/* 关键帧数组 */
	);

	virtual ~Animation();

	// 添加关键帧
	void Add(
		Image * frame			/* 关键帧 */
	);

	// 添加多个关键帧
	void Add(
		const Images& frames	/* 关键帧数组 */
	);

	// 获取帧间隔
	float GetInterval() const;

	// 获取关键帧
	const Images& GetFrames() const;

	// 设置每一帧的时间间隔
	void SetInterval(
		float interval		/* 帧间隔（秒） */
	);

	// 获取帧动画的拷贝对象
	Animation * Clone() const;

	// 获取帧动画的倒转
	Animation * Reverse() const;

protected:
	E2D_DISABLE_COPY(Animation);

protected:
	float	interval_;
	Images	frames_;
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
	virtual Animation * GetAnimation() const;

	// 设置动画
	virtual void SetAnimation(
		Animation * animation
	);

	// 获取该动作的拷贝对象
	virtual Animate * Clone() const override;

	// 获取该动作的倒转
	virtual Animate * Reverse() const override;

	// 重置动作
	virtual void Reset() override;

protected:
	E2D_DISABLE_COPY(Animate);

	// 初始化动作
	virtual void Init() override;

	// 更新动作
	virtual void Update() override;

	// 重置动作时间
	virtual void ResetTime() override;

protected:
	UINT frame_index_;
	Animation * animation_;
};


}