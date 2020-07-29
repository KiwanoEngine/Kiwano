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
 * \defgroup Actions ����
 */

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief �����б�
typedef IntrusiveList<ActionEntityPtr> ActionList;

/// \~chinese
/// @brief ��������ʱ�Ļص�����
typedef Function<void(Actor* /* target */)> ActionDoneCallback;

/// \~chinese
/// @brief ����ʵ��
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
    void SetDoneCallback(const ActionDoneCallback& cb);

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    void SetLoopDoneCallback(const ActionDoneCallback& cb);

    /// \~chinese
    /// @brief ��ȡ�����ĵ�ת
    virtual ActionEntity* Reverse() const = 0;

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
    ActionDoneCallback GetDoneCallback() const;

    /// \~chinese
    /// @brief ��ȡ����ѭ������ʱ�Ļص�����
    ActionDoneCallback GetLoopDoneCallback() const;

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
    /// @brief ���ö���
    void Reset();

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

    /// \~chinese
    /// @brief ִ�п�¡
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
/// @brief ����
class KGE_API Action
{
public:
    Action() = default;

    inline Action(ActionEntityPtr ptr)
        : ptr(ptr)
    {
    }

    /// \~chinese
    /// @brief ����ѭ������
    inline Action& Loops(int loops)
    {
        if (ptr)
            ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö����ӳ�
    inline Action& Delay(Duration delay)
    {
        if (ptr)
            ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö��������ص�����
    inline Action& DoneCallback(const ActionDoneCallback& cb)
    {
        if (ptr)
            ptr->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    inline Action& LoopDoneCallback(const ActionDoneCallback& cb)
    {
        if (ptr)
            ptr->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    inline Action& RemoveTargetWhenDone()
    {
        if (ptr)
            ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief ��������
    inline Action& Name(const String& name)
    {
        if (ptr)
            ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief ��¡����
    inline Action Clone() const
    {
        if (ptr)
            return Action(ptr->Clone());
        return Action();
    }

    /// \~chinese
    /// @brief ��ȡ���򶯻�
    inline Action Reverse() const
    {
        if (ptr)
            return Action(ptr->Reverse());
        return Action();
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline ActionEntity* Get() const
    {
        return const_cast<ActionEntity*>(ptr.Get());
    }

    /// \~chinese
    /// @brief ���ö���ʵ��
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
