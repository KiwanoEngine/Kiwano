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
/// @brief �����б�
typedef IntrusiveList<ActionPtr> ActionList;

/**
 * \~chinese
 * \defgroup Actions ����
 */

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief ����
class KGE_API Action
    : public ObjectBase
    , public Cloneable<Action>
    , protected IntrusiveListValue<ActionPtr>
{
    friend class ActionManager;
    friend class ActionGroup;
    friend IntrusiveList<ActionPtr>;

public:
    /// \~chinese
    /// @brief ��������ʱ�Ļص�����
    using DoneCallback = Function<void(Actor* /* target */)>;

    Action();

    virtual ~Action();

    /// \~chinese
    /// @brief ��������
    void Resume();

    /// \~chinese
    /// @brief ��ͣ����
    void Pause();

    /// \~chinese
    /// @brief ֹͣ����
    void Stop();

    /// \~chinese
    /// @brief ���ö�����ʱ
    void SetDelay(Duration delay);

    /// \~chinese
    /// @brief ����ѭ������
    /// @param loops ѭ��������-1 Ϊ����ѭ��
    void SetLoops(int loops);

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    void RemoveTargetWhenDone();

    /// \~chinese
    /// @brief ���ö�������ʱ�Ļص�����
    void SetDoneCallback(const DoneCallback& cb);

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    void SetLoopDoneCallback(const DoneCallback& cb);

    /// \~chinese
    /// @brief ��ȡ�����ĵ�ת
    virtual ActionPtr Reverse() const = 0;

    /// \~chinese
    /// @brief ��ȡ����������״̬
    bool IsRunning() const;

    /// \~chinese
    /// @brief ��ȡ������ѭ������
    int GetLoops() const;

    /// \~chinese
    /// @brief ��ȡ��������ʱ
    Duration GetDelay() const;

    /// \~chinese
    /// @brief ��ȡ��������ʱ�Ļص�����
    DoneCallback GetDoneCallback() const;

    /// \~chinese
    /// @brief ��ȡ����ѭ������ʱ�Ļص�����
    DoneCallback GetLoopDoneCallback() const;

protected:
    /// \~chinese
    /// @brief ��ʼ������
    virtual void Init(Actor* target);

    /// \~chinese
    /// @brief ���¶���
    virtual void Update(Actor* target, Duration dt);

    /// \~chinese
    /// @brief ����һ��ʱ�䲽
    void UpdateStep(Actor* target, Duration dt);

    /// \~chinese
    /// @brief ��ɶ���
    void Complete(Actor* target);

    /// \~chinese
    /// @brief ���¿�ʼ����
    void Restart(Actor* target);

    /// \~chinese
    /// @brief ����״̬
    enum class Status
    {
        NotStarted,  ///< δ��ʼ
        Delayed,     ///< �ȴ���ʱ
        Started,     ///< �ѿ�ʼ
        Done,        ///< �ѽ���
        Removeable   ///< ���Ƴ�
    };

    /// \~chinese
    /// @brief ��ȡ����״̬
    Status GetStatus() const;

    /// \~chinese
    /// @brief ��ȡ����ʱ��
    Duration GetElapsed() const;

    /// \~chinese
    /// @brief ��ȡ��ɵ�ѭ������
    int GetLoopsDone() const;

    /// \~chinese
    /// @brief ��������
    void Done();

    /// \~chinese
    /// @brief �Ƿ��ѽ���
    bool IsDone() const;

    /// \~chinese
    /// @brief �Ƿ���Ƴ�
    bool IsRemoveable() const;

protected:
    ActionPtr DoClone(ActionPtr to) const;

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
