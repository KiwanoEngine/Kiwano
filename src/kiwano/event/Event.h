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
#include <kiwano/event/EventType.h>
#include <kiwano/math/Math.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Event);

/**
 * \~chinese
 * \defgroup Events 事件
 */

/**
 * \addtogroup Events
 * @{
 */

/// \~chinese
/// @brief 事件
class KGE_API Event : public RefCounter
{
public:
    /// \~chinese
    /// @brief 构造事件
    Event(const EventType& type);

    virtual ~Event();

    /// \~chinese
    /// @brief 获取类型事件
    const EventType& GetType() const;

    /// \~chinese
    /// @brief 判断事件类型
    /// @return 事件类型相同返回true，否则返回false
    template <typename _Ty>
    bool IsType() const;

    /// \~chinese
    /// @brief 安全转换为其他类型事件
    /// @throw std::bad_cast 类型无法转换时抛出
    template <typename _Ty>
    const _Ty* SafeCast() const;

    /// \~chinese
    /// @brief 安全转换为其他类型事件
    /// @throw std::bad_cast 类型无法转换时抛出
    template <typename _Ty>
    _Ty* SafeCast();

private:
    const EventType type_;
};

/// \~chinese
/// @brief 事件特性：判断是否是事件
template <typename _Ty>
struct IsBaseOfEvent : public std::bool_constant<std::is_base_of<Event, _Ty>::value || std::is_same<Event, _Ty>::value>
{
};

/// \~chinese
/// @brief 事件特性：判断事件类型是否相同
template <typename _Ty>
struct IsSameEventType
{
    inline bool operator()(const Event* evt) const
    {
        static_assert(kiwano::IsBaseOfEvent<_Ty>::value, "_Ty is not an event type.");
        return evt->GetType() == KGE_EVENT(_Ty);
    }
};

/** @} */

inline const EventType& Event::GetType() const
{
    return type_;
}

template <typename _Ty>
inline bool Event::IsType() const
{
    static_assert(kiwano::IsBaseOfEvent<_Ty>::value, "_Ty is not an event type.");
    return IsSameEventType<_Ty>()(this);
}

template <typename _Ty>
inline const _Ty* Event::SafeCast() const
{
    return const_cast<Event*>(this)->SafeCast<_Ty>();
}

template <typename _Ty>
inline _Ty* Event::SafeCast()
{
    if (!this->IsType<_Ty>())
        throw std::bad_cast();
    return dynamic_cast<_Ty*>(this);
}

}  // namespace kiwano
