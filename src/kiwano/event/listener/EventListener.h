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
#include <kiwano/core/IntrusiveList.hpp>
#include <kiwano/core/BaseObject.h>
#include <kiwano/event/Events.h>

namespace kiwano
{
class EventDispatcher;

/**
 * \~chinese
 * \defgroup EventListener �¼�������
 */

/**
 * \addtogroup EventListener
 * @{
 */

/**
 * \~chinese
 * @brief �¼�������
 */
class KGE_API EventListener
    : public BaseObject
    , protected IntrusiveListValue<RefPtr<EventListener>>
{
    friend class EventDispatcher;
    friend IntrusiveList<RefPtr<EventListener>>;

public:
    /// \~chinese
    /// @brief �������ص�����
    using Callback = Function<void(Event*)>;

    EventListener();

    virtual ~EventListener();

    /// \~chinese
    /// @brief ����������
    /// @param callback �ص�����
    static RefPtr<EventListener> Create(const Callback& callback);

    /// \~chinese
    /// @brief ����������
    /// @param name ����������
    /// @param callback �ص�����
    static RefPtr<EventListener> Create(StringView name, const Callback& callback);

    /// \~chinese
    /// @brief ����������
    /// @param type �������¼�����
    /// @param callback �ص�����
    static RefPtr<EventListener> Create(EventType type, const Callback& callback);

    /// \~chinese
    /// @brief ����������
    /// @param name ����������
    /// @param type �������¼�����
    /// @param callback �ص�����
    static RefPtr<EventListener> Create(StringView name, EventType type, const Callback& callback);

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
    /// @brief ������Ϣ
    virtual void Handle(Event* evt) = 0;

private:
    bool running_;
    bool removeable_;
    bool swallow_;
};

/** @} */

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

}  // namespace kiwano
