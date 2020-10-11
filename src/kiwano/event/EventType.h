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
#include <typeindex>
#include <typeinfo>

namespace kiwano
{
/**
 * \addtogroup Events
 * @{
 */

/// \~chinese
/// @brief 事件类型
class EventType
{
public:
    EventType();

    EventType(const std::type_index& type);

    /// \~chinese
    /// @brief 是否是空类型
    bool IsNull() const;

    const std::type_index& GetType() const;

    bool operator==(const EventType& rhs) const;
    bool operator!=(const EventType& rhs) const;
    bool operator<(const EventType& rhs) const;
    bool operator<=(const EventType& rhs) const;
    bool operator>(const EventType& rhs) const;
    bool operator>=(const EventType& rhs) const;

private:
    std::type_index type_;
};

/** @} */

#define KGE_EVENT(EVENT_TYPE) ::kiwano::EventType(typeid(EVENT_TYPE))

inline EventType::EventType()
    : type_(typeid(void))
{
}

inline EventType::EventType(const std::type_index& type)
    : type_(type)
{
}

inline bool EventType::IsNull() const
{
    return type_ == typeid(void);
}

inline const std::type_index& EventType::GetType() const
{
    return type_;
}

inline bool EventType::operator==(const EventType& rhs) const
{
    return type_ == rhs.type_;
}

inline bool EventType::operator!=(const EventType& rhs) const
{
    return type_ != rhs.type_;
}

inline bool EventType::operator<(const EventType& rhs) const
{
    return type_ < rhs.type_;
}

inline bool EventType::operator<=(const EventType& rhs) const
{
    return type_ <= rhs.type_;
}

inline bool EventType::operator>(const EventType& rhs) const
{
    return type_ > rhs.type_;
}

inline bool EventType::operator>=(const EventType& rhs) const
{
    return type_ >= rhs.type_;
}

}  // namespace kiwano
