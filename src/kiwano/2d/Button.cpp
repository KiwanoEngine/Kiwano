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

#include <kiwano/2d/Button.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/platform/Application.h>

namespace kiwano
{

ButtonPtr Button::Create(const Callback& click)
{
    return Button::Create(click, nullptr, nullptr, nullptr);
}

ButtonPtr Button::Create(const Callback& click, const Callback& pressed, const Callback& mouse_over,
                         const Callback& mouse_out)
{
    ButtonPtr ptr = memory::New<Button>();
    if (ptr)
    {
        ptr->SetClickCallback(click);
        ptr->SetPressedCallback(pressed);
        ptr->SetMouseOverCallback(mouse_over);
        ptr->SetMouseOutCallback(mouse_out);
    }
    return ptr;
}

Button::Button()
    : status_(Status::Normal)
{
}

Button::~Button()
{
}

void Button::SetStatus(Status status)
{
    if (status_ != status)
    {
        Status old_status = status_;

        if (status == Status::Normal)
        {
            Application::GetInstance().GetMainWindow()->SetCursor(CursorType::Arrow);

            if (mouse_out_callback_)
                mouse_out_callback_(this, GetBoundActor());
        }
        else if (status == Status::Hover)
        {
            Application::GetInstance().GetMainWindow()->SetCursor(CursorType::Hand);

            if (old_status != Status::Pressed)
            {
                if (mouse_over_callback_)
                    mouse_over_callback_(this, GetBoundActor());
            }
        }
        else if (status == Status::Pressed)
        {
            if (pressed_callback_)
                pressed_callback_(this, GetBoundActor());
        }

        status_ = status;
    }
}

void Button::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);
    if (actor)
    {
        actor->SetResponsible(true);
    }
}

void Button::DestroyComponent()
{
    Component::DestroyComponent();
}

void Button::HandleEvent(Event* evt)
{
    if (evt->IsType<MouseHoverEvent>())
    {
        SetStatus(Status::Hover);
    }
    else if (evt->IsType<MouseOutEvent>())
    {
        SetStatus(Status::Normal);
    }
    else if (evt->IsType<MouseDownEvent>() && status_ == Status::Hover)
    {
        SetStatus(Status::Pressed);
    }
    else if (evt->IsType<MouseUpEvent>() && status_ == Status::Pressed)
    {
        SetStatus(Status::Hover);
    }
    else if (evt->IsType<MouseClickEvent>())
    {
        if (click_callback_)
            click_callback_(this, GetBoundActor());
    }
}

}  // namespace kiwano
