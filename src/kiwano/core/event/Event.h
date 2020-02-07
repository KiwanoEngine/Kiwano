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
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/core/event/EventType.h>
#include <kiwano/math/Math.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Event);

/**
 * \~chinese
 * \defgroup Events �¼�
 */

/**
 * \addtogroup Events
 * @{
 */

/// \~chinese
/// @brief �¼�
class KGE_API Event : public RefCounter
{
public:
    /// \~chinese
    /// @brief �����¼�
    Event(EventType const& type);

    virtual ~Event();

    /// \~chinese
    /// @brief ��ȡ�����¼�
    const EventType& GetType() const;

    /// \~chinese
    /// @brief �ж��¼�����
    /// @return �Ƿ���ָ���¼�����
    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type>
    bool IsType() const;

    /// \~chinese
    /// @brief ��ȫת��Ϊ���������¼�
    /// @throw std::bad_cast �����޷�ת��ʱ�׳�
    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type>
    const _Ty* SafeCast() const;

    /// \~chinese
    /// @brief ��ȫת��Ϊ���������¼�
    /// @throw std::bad_cast �����޷�ת��ʱ�׳�
    template <typename _Ty, typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type>
    _Ty* SafeCast();

private:
    const EventType type_;
};

/// \~chinese
/// @brief �¼����ԣ��ж�ָ�������Ƿ����¼�
template <typename _Ty>
struct IsEvent : public std::bool_constant<std::is_base_of<Event, _Ty>::value || std::is_same<Event, _Ty>::value>
{
};

/// \~chinese
/// @brief �¼����ԣ��ж�һ���¼��ܷ�ȫת������һ�¼�����
template <typename _Ty, typename = typename std::enable_if<IsEvent<_Ty>::value, int>::type>
struct IsEventType
{
    inline bool operator()(const Event* evt) const
    {
        return evt->GetType() == KGE_EVENT(_Ty);
    }
};

/** @} */

inline const EventType& Event::GetType() const
{
    return type_;
}

template <typename _Ty, typename>
inline bool Event::IsType() const
{
    return kiwano::IsEventType<_Ty>()(this);
}

template <typename _Ty, typename>
inline const _Ty* Event::SafeCast() const
{
    return const_cast<Event*>(this)->SafeCast<_Ty>();
}

template <typename _Ty, typename>
inline _Ty* Event::SafeCast()
{
    if (!IsType<_Ty>())
        throw std::bad_cast();
    return dynamic_cast<_Ty*>(this);
}

}  // namespace kiwano
