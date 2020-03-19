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
#include <kiwano/core/IntrusiveList.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/WindowEvent.h>

namespace kiwano
{
class EventDispatcher;

KGE_DECLARE_SMART_PTR(EventListener);

/// \~chinese
/// @brief �������б�
typedef IntrusiveList<EventListenerPtr> ListenerList;

/**
 * \~chinese
 * @brief �¼�������
 */
class KGE_API EventListener
    : public ObjectBase
    , protected IntrusiveListValue<EventListenerPtr>
{
    friend class EventDispatcher;
    friend IntrusiveList<EventListenerPtr>;

public:
    /// \~chinese
    /// @brief �������ص�����
    using Callback = Function<void(Event*)>;

    /// \~chinese
    /// @brief ����������
    /// @param type �������¼�����
    /// @param callback �ص�����
    static EventListenerPtr Create(EventType type, const Callback& callback);

    /// \~chinese
    /// @brief ����������
    /// @param name ����������
    /// @param type �������¼�����
    /// @param callback �ص�����
    static EventListenerPtr Create(const String& name, EventType type, const Callback& callback);

    /// \~chinese
    /// @brief ����������
    /// @tparam _EventTy �¼�����
    /// @param callback �ص�����
    template <typename _EventTy, typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type>
    static inline EventListenerPtr Create(const Callback& callback)
    {
        return EventListener::Create(KGE_EVENT(_EventTy), callback);
    }

    /// \~chinese
    /// @brief ����������
    /// @tparam _EventTy �¼�����
    /// @param name ����������
    /// @param callback �ص�����
    template <typename _EventTy, typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type>
    static inline EventListenerPtr Create(const String& name, const Callback& callback)
    {
        return EventListener::Create(name, KGE_EVENT(_EventTy), callback);
    }

    EventListener();

    virtual ~EventListener();

    /// \~chinese
    /// @brief ����������
    void Start();

    /// \~chinese
    /// @brief ֹͣ������
    void Stop();

    /// \~chinese
    /// @brief �Ƴ�������
    void Remove();

    /// \~chinese
    /// @brief �Ƿ���������
    bool IsRunning() const;

    /// \~chinese
    /// @brief �Ƿ���Ƴ�
    bool IsRemoveable() const;

    /// \~chinese
    /// @brief �Ƿ�����Ϣ��û
    bool IsSwallowEnabled() const;

    /// \~chinese
    /// @brief ������Ϣ��û����
    /// @param enabled �Ƿ�����
    void SetSwallowEnabled(bool enabled);

    /// \~chinese
    /// @brief ��ȡ�ص�����
    const Callback& GetCallback() const;

    /// \~chinese
    /// @brief ���ûص�����
    void SetCallback(const Callback& cb);

    /// \~chinese
    /// @brief ��ȡ�������¼�����
    EventType GetEventType() const;

    /// \~chinese
    /// @brief ���ü������¼�����
    void SetEventType(const EventType& type);

    /// \~chinese
    /// @brief ���ü������¼�����
    /// @tparam _EventTy �¼�����
    template <typename _EventTy, typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type>
    inline void SetEventType()
    {
        SetEventType(KGE_EVENT(_EventTy));
    }

    /// \~chinese
    /// @brief ������Ϣ
    void Receive(Event* evt);

private:
    bool      running_;
    bool      removeable_;
    bool      swallow_;
    EventType type_;
    Callback  callback_;
};

inline void EventListener::Start()
{
    running_ = true;
}

inline void EventListener::Stop()
{
    running_ = false;
}

inline void EventListener::Remove()
{
    removeable_ = true;
}

inline bool EventListener::IsRunning() const
{
    return running_;
}

inline bool EventListener::IsRemoveable() const
{
    return removeable_;
}

inline bool EventListener::IsSwallowEnabled() const
{
    return swallow_;
}

inline void EventListener::SetSwallowEnabled(bool enabled)
{
    swallow_ = enabled;
}

inline const EventListener::Callback& EventListener::GetCallback() const
{
    return callback_;
}

inline void EventListener::SetCallback(const Callback& cb)
{
    callback_ = cb;
}

inline EventType EventListener::GetEventType() const
{
    return type_;
}

inline void EventListener::SetEventType(const EventType& type)
{
    type_ = type;
}

inline void EventListener::Receive(Event* evt)
{
    KGE_ASSERT(evt != nullptr);

    if (type_ == evt->GetType() && callback_)
    {
        callback_(evt);
    }
}
}  // namespace kiwano
