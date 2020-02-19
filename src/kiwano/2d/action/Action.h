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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/core/Time.h>
#include <kiwano/core/IntrusiveList.h>
#include <kiwano/math/Math.h>

namespace kiwano
{
class Actor;
class ActionManager;

KGE_DECLARE_SMART_PTR(Action);

/// \~chinese
/// @brief 动画列表
typedef IntrusiveList<ActionPtr> ActionList;

/**
 * \~chinese
 * \defgroup Actions 动画
 */

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 动画
class KGE_API Action
    : public virtual ObjectBase
    , protected IntrusiveListValue<ActionPtr>
{
    friend class ActionManager;
    friend class ActionGroup;
    friend IntrusiveList<ActionPtr>;

public:
    /// \~chinese
    /// @brief 动画结束时的回调函数
    using DoneCallback = Function<void(Actor* /* target */)>;

    Action();

    virtual ~Action();

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
    void SetDoneCallback(const DoneCallback& cb);

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    void SetLoopDoneCallback(const DoneCallback& cb);

    /// \~chinese
    /// @brief 获取动画的拷贝
    virtual ActionPtr Clone() const = 0;

    /// \~chinese
    /// @brief 获取动画的倒转
    virtual ActionPtr Reverse() const = 0;

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
    DoneCallback GetDoneCallback() const;

    /// \~chinese
    /// @brief 获取动画循环结束时的回调函数
    DoneCallback GetLoopDoneCallback() const;

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
    /// @brief 重新开始动画
    void Restart(Actor* target);

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

protected:
    ActionPtr InnerClone(ActionPtr to) const;

private:
    Status       status_;
    bool         running_;
    bool         detach_target_;
    int          loops_;
    int          loops_done_;
    Duration     delay_;
    Duration     elapsed_;
    DoneCallback cb_done_;
    DoneCallback cb_loop_done_;
};

/** @} */

inline void Action::Resume()
{
    running_ = true;
}

inline void Action::Pause()
{
    running_ = false;
}

inline void Action::Stop()
{
    Done();
}

inline void Action::SetDelay(Duration delay)
{
    delay_ = delay;
}

inline void Action::SetLoops(int loops)
{
    loops_ = loops;
}

inline void Action::RemoveTargetWhenDone()
{
    detach_target_ = true;
}

inline void Action::SetDoneCallback(const DoneCallback& cb)
{
    cb_done_ = cb;
}

inline void Action::SetLoopDoneCallback(const DoneCallback& cb)
{
    cb_loop_done_ = cb;
}

inline void Action::Done()
{
    status_ = Status::Done;
}

inline Action::Status Action::GetStatus() const
{
    return status_;
}

inline bool Action::IsRunning() const
{
    return running_;
}

inline bool Action::IsDone() const
{
    return status_ == Status::Done || status_ == Status::Removeable;
}

inline bool Action::IsRemoveable() const
{
    return status_ == Status::Removeable;
}

inline int Action::GetLoops() const
{
    return loops_;
}

inline Duration Action::GetDelay() const
{
    return delay_;
}

inline Duration Action::GetElapsed() const
{
    return elapsed_;
}

inline int Action::GetLoopsDone() const
{
    return loops_done_;
}

inline Action::DoneCallback Action::GetDoneCallback() const
{
    return cb_done_;
}

inline Action::DoneCallback Action::GetLoopDoneCallback() const
{
    return cb_loop_done_;
}
}  // namespace kiwano
