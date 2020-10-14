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
#include <kiwano/platform/Keys.h>
#include <kiwano/event/Event.h>
#include <kiwano/math/Math.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(MouseEvent);
KGE_DECLARE_SMART_PTR(MouseMoveEvent);
KGE_DECLARE_SMART_PTR(MouseDownEvent);
KGE_DECLARE_SMART_PTR(MouseUpEvent);
KGE_DECLARE_SMART_PTR(MouseClickEvent);
KGE_DECLARE_SMART_PTR(MouseHoverEvent);
KGE_DECLARE_SMART_PTR(MouseOutEvent);
KGE_DECLARE_SMART_PTR(MouseWheelEvent);

/**
 * \addtogroup Event
 * @{
 */

/// \~chinese
/// @brief ����¼�
class KGE_API MouseEvent : public Event
{
public:
    Point pos;  ///< ���λ��

    MouseEvent(const EventType& type);
};

/// \~chinese
/// @brief ����ƶ��¼�
class KGE_API MouseMoveEvent : public MouseEvent
{
public:
    MouseMoveEvent();
};

/// \~chinese
/// @brief ��갴�������¼�
class KGE_API MouseDownEvent : public MouseEvent
{
public:
    MouseButton button;  ///< ����ֵ

    MouseDownEvent();
};

/// \~chinese
/// @brief ��갴��̧���¼�
class KGE_API MouseUpEvent : public MouseEvent
{
public:
    MouseButton button;  ///< ����ֵ

    MouseUpEvent();
};

/// \~chinese
/// @brief ������¼�
class KGE_API MouseClickEvent : public MouseEvent
{
public:
    MouseButton button;  ///< ����ֵ

    MouseClickEvent();
};

/// \~chinese
/// @brief ��������¼�
class KGE_API MouseHoverEvent : public MouseEvent
{
public:
    MouseHoverEvent();
};

/// \~chinese
/// @brief ����Ƴ��¼�
class KGE_API MouseOutEvent : public MouseEvent
{
public:
    MouseOutEvent();
};

/// \~chinese
/// @brief �������¼�
class KGE_API MouseWheelEvent : public MouseEvent
{
public:
    float wheel;  ///< ����ֵ

    MouseWheelEvent();
};

template <>
struct IsSameEventType<MouseEvent>
{
    inline bool operator()(const Event* evt) const
    {
        return evt->GetType() == KGE_EVENT(MouseMoveEvent) || evt->GetType() == KGE_EVENT(MouseDownEvent)
               || evt->GetType() == KGE_EVENT(MouseUpEvent) || evt->GetType() == KGE_EVENT(MouseClickEvent)
               || evt->GetType() == KGE_EVENT(MouseHoverEvent) || evt->GetType() == KGE_EVENT(MouseOutEvent)
               || evt->GetType() == KGE_EVENT(MouseWheelEvent);
    }
};

/** @} */

}  // namespace kiwano
