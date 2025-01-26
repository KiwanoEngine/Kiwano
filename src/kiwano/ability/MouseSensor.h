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
#include <kiwano/ability/Ability.h>
#include <kiwano/event/MouseEvent.h>
#include <kiwano/event/listener/EventListener.h>

namespace kiwano
{

/**
 * \addtogroup Event
 * @{
 */

/// \~chinese
/// @brief 鼠标点击事件
class KGE_API MouseClickEvent : public MouseEvent
{
public:
    MouseButton button;  ///< 鼠标键值

    MouseClickEvent();
};

/// \~chinese
/// @brief 鼠标移入事件
class KGE_API MouseHoverEvent : public MouseEvent
{
public:
    MouseHoverEvent();
};

/// \~chinese
/// @brief 鼠标移出事件
class KGE_API MouseOutEvent : public MouseEvent
{
public:
    MouseOutEvent();
};

/** @} */

/**
 * \addtogroup Ability
 * @{
 */

/**
 * \~chinese
 * @brief 鼠标传感器
 * @details 该能力使角色会收到鼠标的 Hover | Out | Click 消息
 */
class KGE_API MouseSensor : public Ability
{
public:
    MouseSensor();

    /// \~chinese
    /// @brief 鼠标是否正悬于角色上
    bool IsHovering() const;

    /// \~chinese
    /// @brief 鼠标是否正按于角色上
    bool IsPressing() const;

protected:
    void OnAttached(Actor* actor) override;

    void OnDetached() override;

    virtual void HandleEvent(Event* evt);

private:
    bool hover_;
    bool pressed_;

    RefPtr<EventListener> listener_;
};

/** @} */

inline bool MouseSensor::IsHovering() const
{
    return hover_;
}

inline bool MouseSensor::IsPressing() const
{
    return pressed_;
}

}  // namespace kiwano
