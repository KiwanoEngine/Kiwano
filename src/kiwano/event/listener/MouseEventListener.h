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
#include <kiwano/event/MouseEvent.h>
#include <kiwano/event/listener/EventListener.h>

namespace kiwano
{

/**
 * \addtogroup EventListener
 * @{
 */

/**
 * \~chinese
 * @brief 鼠标事件监听器
 */
class KGE_API MouseEventListener
    : public EventListener
{
public:
    /// \~chinese
    /// @brief 鼠标移动时
    /// @param pos 鼠标位置
    virtual void OnMouseMoving(const Point& pos) {}

    /// \~chinese
    /// @brief 鼠标按下时
    /// @param pos 鼠标位置
    /// @param button 鼠标键值
    virtual void OnMouseDown(const Point& pos, MouseButton button) {}

    /// \~chinese
    /// @brief 鼠标抬起时
    /// @param pos 鼠标位置
    /// @param button 鼠标键值
    virtual void OnMouseUp(const Point& pos, MouseButton button) {}

    /// \~chinese
    /// @brief 鼠标滚轮滚动时
    /// @param pos 鼠标位置
    /// @param wheel 鼠标滚轮滚动值
    virtual void OnMouseWheelScrolling(const Point& pos, float wheel) {}

    /// \~chinese
    /// @brief 处理消息
    void Handle(Event* evt) override;
};

/**
 * \~chinese
 * @brief 鼠标拖动事件监听器
 */
class KGE_API MouseDragEventListener : public EventListener
{
public:
    MouseDragEventListener();

    /// \~chinese
    /// @brief 鼠标拖动开始
    /// @param pos 鼠标位置
    /// @param button 鼠标键值
    /// @return 是否响应这次拖动
    virtual bool OnDragStart(const Point& pos, MouseButton button)
    {
        return true;
    }

    /// \~chinese
    /// @brief 鼠标拖动时
    /// @param offset 鼠标位置偏移量
    /// @param button 鼠标键值
    virtual void OnDragging(const Point& offset, MouseButton button) {}

    /// \~chinese
    /// @brief 鼠标拖动结束
    /// @param pos 鼠标位置
    /// @param button 鼠标键值
    virtual void OnDragEnd(const Point& pos, MouseButton button) {}

    /// \~chinese
    /// @brief 获取鼠标按键的拖动状态
    bool IsDragging(MouseButton button);

    /// \~chinese
    /// @brief 获取鼠标按键的起始拖动位置
    Point GetDragPoint(MouseButton button);

    /// \~chinese
    /// @brief 处理消息
    void Handle(Event* evt) override;

private:
    void SetDragging(MouseButton button, bool dragging);

    void SetDragPoint(MouseButton button, Point pos);

    Point GetDragPrevPoint(MouseButton button);

    void SetDragPrevPoint(MouseButton button, Point pos);

private:
    enum : int
    {
        MouseButtonNum = 3
    };

    bool  is_dragging_[MouseButtonNum];
    Point drag_point_[MouseButtonNum];
    Point drag_prev_point_[MouseButtonNum];
};

/** @} */

}  // namespace kiwano
