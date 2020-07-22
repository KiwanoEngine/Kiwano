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
class ActionScheduler;

KGE_DECLARE_SMART_PTR(ActionEntity);

/**
 * \~chinese
 * \defgroup Actions 动画
 */

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 动画列表
typedef IntrusiveList<ActionEntityPtr> ActionList;

/// \~chinese
/// @brief 动画结束时的回调函数
typedef Function<void(Actor* /* target */)> ActionDoneCallback;

/// \~chinese
/// @brief 动画实体
class KGE_API ActionEntity
    : public ObjectBase
    , public Cloneable<ActionEntity>
    , protected IntrusiveListValue<ActionEntityPtr>
{
    friend class ActionScheduler;
    friend class ActionGroupEntity;
    friend IntrusiveList<ActionEntityPtr>;

public:
    ActionEntity();

    virtual ~ActionEntity();

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
    /// @brief 设置动画结束时的回调函数
    void SetDoneCallback(const ActionDoneCallback& cb);

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    void SetLoopDoneCallback(const ActionDoneCallback& cb);

    /// \~chinese
    /// @brief 获取动画的倒转
    virtual ActionEntity* Reverse() const = 0;

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
    /// @brief 获取动画结束时的回调函数
    ActionDoneCallback GetDoneCallback() const;

    /// \~chinese
    /// @brief 获取动画循环结束时的回调函数
    ActionDoneCallback GetLoopDoneCallback() const;

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
    /// @brief 执行克隆
    void DoClone(ActionEntity* to) const;

private:
    Status             status_;
    bool               running_;
    bool               detach_target_;
    int                loops_;
    int                loops_done_;
    Duration           delay_;
    Duration           elapsed_;
    ActionDoneCallback cb_done_;
    ActionDoneCallback cb_loop_done_;
};

/// \~chinese
/// @brief 动画
class KGE_API Action
{
public:
    Action() = default;

    inline Action(ActionEntityPtr ptr)
        : ptr(ptr)
    {
    }

    /// \~chinese
    /// @brief 设置循环次数
    inline Action& Loops(int loops)
    {
        if (ptr)
            ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画延迟
    inline Action& Delay(Duration delay)
    {
        if (ptr)
            ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画结束回调函数
    inline Action& DoneCallback(const ActionDoneCallback& cb)
    {
        if (ptr)
            ptr->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    inline Action& LoopDoneCallback(const ActionDoneCallback& cb)
    {
        if (ptr)
            ptr->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    inline Action& RemoveTargetWhenDone()
    {
        if (ptr)
            ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief 设置名称
    inline Action& Name(const String& name)
    {
        if (ptr)
            ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief 克隆动画
    inline Action Clone() const
    {
        if (ptr)
            return Action(ptr->Clone());
        return Action();
    }

    /// \~chinese
    /// @brief 获取反向动画
    inline Action Reverse() const
    {
        if (ptr)
            return Action(ptr->Reverse());
        return Action();
    }

    /// \~chinese
    /// @brief 获取指针
    inline ActionEntity* Get() const
    {
        return const_cast<ActionEntity*>(ptr.Get());
    }

    /// \~chinese
    /// @brief 设置动画实体
    inline void SetEntity(ActionEntityPtr ptr)
    {
        this->ptr = ptr;
    }

    inline ActionEntity* operator->() const
    {
        return Get();
    }

    inline operator ActionEntity*() const
    {
        return Get();
    }

    inline operator ActionEntityPtr() const
    {
        return ptr;
    }

    inline operator bool() const
    {
        return ptr != nullptr;
    }

protected:
    ActionEntityPtr ptr;
};

/** @} */

inline void ActionEntity::Resume()
{
    running_ = true;
}

inline void ActionEntity::Pause()
{
    running_ = false;
}

inline void ActionEntity::Stop()
{
    Done();
}

inline void ActionEntity::SetDelay(Duration delay)
{
    delay_ = delay;
}

inline void ActionEntity::SetLoops(int loops)
{
    loops_ = loops;
}

inline void ActionEntity::RemoveTargetWhenDone()
{
    detach_target_ = true;
}

inline void ActionEntity::SetDoneCallback(const ActionDoneCallback& cb)
{
    cb_done_ = cb;
}

inline void ActionEntity::SetLoopDoneCallback(const ActionDoneCallback& cb)
{
    cb_loop_done_ = cb;
}

inline void ActionEntity::Done()
{
    status_ = Status::Done;
}

inline ActionEntity::Status ActionEntity::GetStatus() const
{
    return status_;
}

inline bool ActionEntity::IsRunning() const
{
    return running_;
}

inline bool ActionEntity::IsDone() const
{
    return status_ == Status::Done || status_ == Status::Removeable;
}

inline bool ActionEntity::IsRemoveable() const
{
    return status_ == Status::Removeable;
}

inline int ActionEntity::GetLoops() const
{
    return loops_;
}

inline Duration ActionEntity::GetDelay() const
{
    return delay_;
}

inline Duration ActionEntity::GetElapsed() const
{
    return elapsed_;
}

inline int ActionEntity::GetLoopsDone() const
{
    return loops_done_;
}

inline ActionDoneCallback ActionEntity::GetDoneCallback() const
{
    return cb_done_;
}

inline ActionDoneCallback ActionEntity::GetLoopDoneCallback() const
{
    return cb_loop_done_;
}
}  // namespace kiwano
