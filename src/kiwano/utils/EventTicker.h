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
#include <kiwano/event/EventDispatcher.h>
#include <kiwano/event/Event.h>

namespace kiwano
{
class EventTicker;

/**
 * \addtogroup Event
 * @{
 */

/// \~chinese
/// @brief ��ʱʱ��
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
/// @brief �¼���ʱ��
class KGE_API EventTicker
    : public Ticker
    , public EventDispatcher
{
public:
    EventTicker();

    /// \~chinese
    /// @brief �����¼���ʱ��
    /// @param interval ��ʱ���
    /// @param tick_count ��ʱ�������� -1 Ϊ���ã�
    EventTicker(Duration interval, int tick_count = -1);

    using Ticker::Tick;

    bool Tick(Duration dt) override;
};

}  // namespace kiwano
