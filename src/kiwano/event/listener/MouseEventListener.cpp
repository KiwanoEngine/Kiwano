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
#include <kiwano/event/listener/MouseEventListener.h>

namespace kiwano
{

void MouseEventListener::Handle(Event* evt)
{
    if (auto mouse_evt = evt->Cast<MouseMoveEvent>())
    {
        OnMouseMoving(mouse_evt->pos);
    }
    else if (auto mouse_evt = evt->Cast<MouseDownEvent>())
    {
        OnMouseDown(mouse_evt->pos, mouse_evt->button);
    }
    else if (auto mouse_evt = evt->Cast<MouseUpEvent>())
    {
        OnMouseUp(mouse_evt->pos, mouse_evt->button);
    }
    else if (auto mouse_evt = evt->Cast<MouseWheelEvent>())
    {
        OnMouseWheelScrolling(mouse_evt->pos, mouse_evt->wheel);
    }
}

MouseDragEventListener::MouseDragEventListener()
    : is_dragging_{ false, false, false }
    , drag_point_{}
{
}

void MouseDragEventListener::Handle(Event* evt)
{
    if (auto mouse_evt = evt->Cast<MouseDownEvent>())
    {
        const auto button = mouse_evt->button;
        if (OnDragStart(mouse_evt->pos, button))
        {
            SetDragging(button, true);
            SetDragPoint(button, mouse_evt->pos);
            SetDragPrevPoint(button, mouse_evt->pos);
        }
    }

    if (auto mouse_evt = evt->Cast<MouseMoveEvent>())
    {
        for (int i = 0; i < MouseButtonNum; i++)
        {
            const auto button = MouseButton(i);
            if (IsDragging(button))
            {
                auto new_pos = mouse_evt->pos;
                auto offset  = new_pos - GetDragPrevPoint(button);
                OnDragging(offset, button);
                SetDragPrevPoint(button, new_pos);
            }
        }
    }

    if (auto mouse_evt = evt->Cast<MouseUpEvent>())
    {
        const auto button = mouse_evt->button;
        if (IsDragging(button))
        {
            OnDragEnd(mouse_evt->pos, button);

            SetDragging(button, false);
            SetDragPoint(button, Point());
            SetDragPrevPoint(button, Point());
        }
    }
}

bool MouseDragEventListener::IsDragging(MouseButton button)
{
    int index = int(button);
    KGE_ASSERT(index >= 0 && index < MouseButtonNum);
    return is_dragging_[index];
}

void MouseDragEventListener::SetDragging(MouseButton button, bool dragging)
{
    int index = int(button);
    KGE_ASSERT(index >= 0 && index < MouseButtonNum);
    is_dragging_[index] = dragging;
}

Point MouseDragEventListener::GetDragPoint(MouseButton button)
{
    int index = int(button);
    KGE_ASSERT(index >= 0 && index < MouseButtonNum);
    return drag_point_[index];
}

void MouseDragEventListener::SetDragPoint(MouseButton button, Point pos)
{
    int index = int(button);
    KGE_ASSERT(index >= 0 && index < MouseButtonNum);
    drag_point_[index] = pos;
}

Point MouseDragEventListener::GetDragPrevPoint(MouseButton button)
{
    int index = int(button);
    KGE_ASSERT(index >= 0 && index < MouseButtonNum);
    return drag_prev_point_[index];
}

void MouseDragEventListener::SetDragPrevPoint(MouseButton button, Point pos)
{
    int index = int(button);
    KGE_ASSERT(index >= 0 && index < MouseButtonNum);
    drag_prev_point_[index] = pos;
}

}  // namespace kiwano
