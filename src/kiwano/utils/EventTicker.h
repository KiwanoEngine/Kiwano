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
#include <kiwano/utils/Ticker.h>
#include <kiwano/core/EventDispatcher.h>
#include <kiwano/core/event/Event.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(TickEvent);
KGE_DECLARE_SMART_PTR(EventTicker);

/**
 * \addtogroup Events
 * @{
 */

/// \~chinese
/// @brief 报时时间
class KGE_API TickEvent : public Event
{
public:
    TickEvent();

    EventTicker* ticker_;
    Duration     delta_time_;
};

/**
 * @}
 */


/// \~chinese
/// @brief 事件报时器
class KGE_API EventTicker
    : public Ticker
    , public EventDispatcher
{
public:
    /// \~chinese
    /// @brief 创建事件报时器
    /// @param interval 报时间隔
    /// @param times 报时次数（设 -1 为永久）
    static EventTickerPtr Create(Duration interval, int times = -1);

    bool Tick(Duration dt) override;
};

}  // namespace kiwano
