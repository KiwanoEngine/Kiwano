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
 * \defgroup Animation ����
 */

/**
 * \addtogroup Animation
 * @{
 */

/// \~chinese
/// @brief �����¼�
enum class AnimationEvent
{
    Started,   ///< ������ʼ
    LoopDone,  ///< ����һ��ѭ������
    Done,      ///< ��������
};

/// \~chinese
/// @brief �����¼�������
class KGE_API AnimationEventHandler : public RefObject
{
public:
    /// \~chinese
    /// @brief �������¼�
    /// @param anim ��������
    /// @param target ִ�ж����Ķ���
    /// @param evt �����¼�
    virtual void Handle(Animation* anim, Actor* target, AnimationEvent evt) = 0;

    /// \~chinese
    /// @brief ���������¼�������
    /// @param handler �������¼��ص�����
    static AnimationEventHandlerPtr Create(const Function<void(Animation*, Actor*, AnimationEvent)>& handler);

    /// \~chinese
    /// @brief ���������¼�������
    /// @param evt ����Ķ����¼�
    /// @param handler �������¼��ص�����
    static AnimationEventHandlerPtr Create(AnimationEvent evt, const Function<void(Animation*, Actor*)>& handler);

    /// \~chinese
    /// @brief ����Started�����¼�������
    /// @param handler �������¼��ص�����
    static AnimationEventHandlerPtr HandleStarted(const Function<void(Animation*, Actor*)>& handler);

    /// \~chinese
    /// @brief ����LoopDone�����¼�������
    /// @param handler �������¼��ص�����
    static AnimationEventHandlerPtr HandleLoopDone(const Function<void(Animation*, Actor*)>& handler);

    /// \~chinese
    /// @brief ����Done�����¼�������
    /// @param handler �������¼��ص�����
    static AnimationEventHandlerPtr HandleDone(const Function<void(Animation*, Actor*)>& handler);
};

/// \~chinese
/// @brief �����б�
typedef IntrusiveList<AnimationPtr> AnimationList;

/// \~chinese
/// @brief ����
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
    /// @brief ��ȡ�����ĵ�ת
    virtual Animation* Reverse() const = 0;

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
    /// @brief ���ö����¼�����
    void SetHandler(AnimationEventHandlerPtr handler);

    /// \~chinese
    /// @brief ��ȡ�����¼�����
    AnimationEventHandlerPtr GetHandler() const;

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
    /// @brief ���������¼�
    void EmitEvent(Actor* target, AnimationEvent evt);

    /// \~chinese
    /// @brief ִ�п�¡
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
