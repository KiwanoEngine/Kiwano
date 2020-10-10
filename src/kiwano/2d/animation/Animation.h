// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <kiwano/core/Common.h>
#include <kiwano/core/Cloneable.h>
#include <kiwano/base/ObjectBase.h>
#include <kiwano/core/Time.h>
#include <kiwano/core/IntrusiveList.h>
#include <kiwano/math/Math.h>

namespace kiwano
{
class Actor;
class Animator;

KGE_DECLARE_SMART_PTR(Animation);
KGE_DECLARE_SMART_PTR(AnimationEventHandler);

/**
 * \~chinese
 * \defgroup Animation 动画
 */

/**
 * \addtogroup Animation
 * @{
 */

/// \~chinese
/// @brief 动画事件
enum class AnimationEvent
{
    Started,   ///< 动画开始
    LoopDone,  ///< 动画一次循环结束
    Done,      ///< 动画结束
};

/// \~chinese
/// @brief 动画事件处理器
class KGE_API AnimationEventHandler : public RefObject
{
public:
    /// \~chinese
    /// @brief 处理动画事件
    /// @param anim 动画对象
    /// @param target 执行动画的对象
    /// @param evt 动画事件
    virtual void Handle(Animation* anim, Actor* target, AnimationEvent evt) = 0;

    /// \~chinese
    /// @brief 创建动画事件处理器
    /// @param handler 处理动画事件回调函数
    static AnimationEventHandlerPtr Create(const Function<void(Animation*, Actor*, AnimationEvent)>& handler);

    /// \~chinese
    /// @brief 创建动画事件处理器
    /// @param evt 处理的动画事件
    /// @param handler 处理动画事件回调函数
    static AnimationEventHandlerPtr Create(AnimationEvent evt, const Function<void(Animation*, Actor*)>& handler);

    /// \~chinese
    /// @brief 创建Started动画事件处理器
    /// @param handler 处理动画事件回调函数
    static AnimationEventHandlerPtr HandleStarted(const Function<void(Animation*, Actor*)>& handler);

    /// \~chinese
    /// @brief 创建LoopDone动画事件处理器
    /// @param handler 处理动画事件回调函数
    static AnimationEventHandlerPtr HandleLoopDone(const Function<void(Animation*, Actor*)>& handler);

    /// \~chinese
    /// @brief 创建Done动画事件处理器
    /// @param handler 处理动画事件回调函数
    static AnimationEventHandlerPtr HandleDone(const Function<void(Animation*, Actor*)>& handler);
};

/// \~chinese
/// @brief 动画列表
typedef IntrusiveList<AnimationPtr> AnimationList;

/// \~chinese
/// @brief 动画
class KGE_API Animation
    : public ObjectBase
    , public Cloneable<Animation>
    , protected IntrusiveListValue<AnimationPtr>
{
    friend class Animator;
    friend class AnimationGroup;
    friend IntrusiveList<AnimationPtr>;

public:
    Animation();

    virtual ~Animation();

    /// \~chinese
    /// @brief 继续动画
    void Resume();

    /// \~chinese
    /// @brief 暂停动画
    void Pause();

    /// \~chinese
    /// @brief 停止动画
    void Stop();

    /// \~chinese
    /// @brief 设置动画延时
    void SetDelay(Duration delay);

    /// \~chinese
    /// @brief 设置循环次数
    /// @param loops 循环次数，-1 为永久循环
    void SetLoops(int loops);

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    void RemoveTargetWhenDone();

    /// \~chinese
    /// @brief 获取动画的倒转
    virtual Animation* Reverse() const = 0;

    /// \~chinese
    /// @brief 获取动画的运行状态
    bool IsRunning() const;

    /// \~chinese
    /// @brief 获取动画的循环次数
    int GetLoops() const;

    /// \~chinese
    /// @brief 获取动画的延时
    Duration GetDelay() const;

    /// \~chinese
    /// @brief 设置动画事件处理
    void SetHandler(AnimationEventHandlerPtr handler);

    /// \~chinese
    /// @brief 获取动画事件处理
    AnimationEventHandlerPtr GetHandler() const;

protected:
    /// \~chinese
    /// @brief 初始化动画
    virtual void Init(Actor* target);

    /// \~chinese
    /// @brief 更新动画
    virtual void Update(Actor* target, Duration dt);

    /// \~chinese
    /// @brief 更新一个时间步
    void UpdateStep(Actor* target, Duration dt);

    /// \~chinese
    /// @brief 完成动画
    void Complete(Actor* target);

    /// \~chinese
    /// @brief 重置动画
    void Reset();

    /// \~chinese
    /// @brief 动画状态
    enum class Status
    {
        NotStarted,  ///< 未开始
        Delayed,     ///< 等待延时
        Started,     ///< 已开始
        Done,        ///< 已结束
        Removeable   ///< 可移除
    };

    /// \~chinese
    /// @brief 获取动画状态
    Status GetStatus() const;

    /// \~chinese
    /// @brief 获取消逝时间
    Duration GetElapsed() const;

    /// \~chinese
    /// @brief 获取完成的循环次数
    int GetLoopsDone() const;

    /// \~chinese
    /// @brief 结束动画
    void Done();

    /// \~chinese
    /// @brief 是否已结束
    bool IsDone() const;

    /// \~chinese
    /// @brief 是否可移除
    bool IsRemoveable() const;

    /// \~chinese
    /// @brief 发出动画事件
    void EmitEvent(Actor* target, AnimationEvent evt);

    /// \~chinese
    /// @brief 执行克隆
    void DoClone(Animation* to) const;

private:
    Status   status_;
    bool     running_;
    bool     detach_target_;
    int      loops_;
    int      loops_done_;
    Duration delay_;
    Duration elapsed_;

    AnimationEventHandlerPtr handler_;
};

/** @} */

inline void Animation::Resume()
{
    running_ = true;
}

inline void Animation::Pause()
{
    running_ = false;
}

inline void Animation::Stop()
{
    Done();
}

inline void Animation::SetDelay(Duration delay)
{
    delay_ = delay;
}

inline void Animation::SetLoops(int loops)
{
    loops_ = loops;
}

inline void Animation::RemoveTargetWhenDone()
{
    detach_target_ = true;
}

inline void Animation::Done()
{
    status_ = Status::Done;
}

inline Animation::Status Animation::GetStatus() const
{
    return status_;
}

inline bool Animation::IsRunning() const
{
    return running_;
}

inline bool Animation::IsDone() const
{
    return status_ == Status::Done || status_ == Status::Removeable;
}

inline bool Animation::IsRemoveable() const
{
    return status_ == Status::Removeable;
}

inline void Animation::EmitEvent(Actor* target, AnimationEvent evt)
{
    if (handler_)
    {
        handler_->Handle(this, target, evt);
    }
}

inline int Animation::GetLoops() const
{
    return loops_;
}

inline Duration Animation::GetDelay() const
{
    return delay_;
}

inline void Animation::SetHandler(AnimationEventHandlerPtr handler)
{
    handler_ = handler;
}

inline AnimationEventHandlerPtr Animation::GetHandler() const
{
    return handler_;
}

inline Duration Animation::GetElapsed() const
{
    return elapsed_;
}

inline int Animation::GetLoopsDone() const
{
    return loops_done_;
}

}  // namespace kiwano
