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
#include <kiwano/event/listener/EventListener.h>

namespace kiwano
{

/// \~chinese
/// @brief �������б�
typedef IntrusiveList<RefPtr<EventListener>> ListenerList;

/**
 * \~chinese
 * @brief �¼��ַ���
 */
class KGE_API EventDispatcher
{
public:
    /// \~chinese
    /// @brief ��Ӽ�����
    EventListener* AddListener(RefPtr<EventListener> listener);

    /// \~chinese
    /// @brief ��Ӽ�����
    /// @param type �������¼�����
    /// @param callback �ص�����
    EventListener* AddListener(EventType type, EventListener::Callback callback);

    /// \~chinese
    /// @brief ��Ӽ�����
    /// @param name ����������
    /// @param type �������¼�����
    /// @param callback �ص�����
    EventListener* AddListener(StringView name, EventType type, EventListener::Callback callback);

    /// \~chinese
    /// @brief ��Ӽ�����
    /// @tparam _EventTy �¼�����
    /// @param callback �ص�����
    template <typename _EventTy>
    EventListener* AddListener(EventListener::Callback callback)
    {
        static_assert(std::is_base_of<Event, _EventTy>::value, "_EventTy is not an event type.");
        return AddListener(KGE_EVENT(_EventTy), callback);
    }

    /// \~chinese
    /// @brief ��Ӽ�����
    /// @tparam _EventTy �¼�����
    /// @param name ����������
    /// @param callback �ص�����
    template <typename _EventTy>
    EventListener* AddListener(StringView name, EventListener::Callback callback)
    {
        static_assert(std::is_base_of<Event, _EventTy>::value, "_EventTy is not an event type.");
        return AddListener(name, KGE_EVENT(_EventTy), callback);
    }

    /// \~chinese
    /// @brief ����������
    /// @param name ����������
    void StartListeners(StringView name);

    /// \~chinese
    /// @brief ֹͣ������
    /// @param name ����������
    void StopListeners(StringView name);

    /// \~chinese
    /// @brief �Ƴ�������
    /// @param name ����������
    void RemoveListeners(StringView name);

    /// \~chinese
    /// @brief �������м�����
    void StartAllListeners();

    /// \~chinese
    /// @brief ֹͣ���м�����
    void StopAllListeners();

    /// \~chinese
    /// @brief �Ƴ����м�����
    void RemoveAllListeners();

    /// \~chinese
    /// @brief ��ȡ���м�����
    const ListenerList& GetAllListeners() const;

    /// \~chinese
    /// @brief �ַ��¼�
    /// @param evt �¼�
    /// @return �Ƿ�����ַ����¼�
    bool DispatchEvent(Event* evt);

private:
    ListenerList listeners_;
};
}  // namespace kiwano
