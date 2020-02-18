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

ButtonPtr Button::Create(ActorPtr actor, Callback const& click)
{
    return Button::Create(actor, click, nullptr, nullptr, nullptr);
}

ButtonPtr Button::Create(ActorPtr actor, Callback const& click, Callback const& pressed, Callback const& mouse_over,
                         Callback const& mouse_out)
{
    ButtonPtr ptr = new (std::nothrow) Button;
    if (ptr)
    {
        ptr->BindActor(actor);
        ptr->SetClickCallback(click);
        ptr->SetPressedCallback(pressed);
        ptr->SetMouseOverCallback(mouse_over);
        ptr->SetMouseOutCallback(mouse_out);
    }
    return ptr;
}

Button::Button()
    : enabled_(true)
    , status_(Status::Normal)
{
}

Button::~Button()
{
    Unbind();
}

bool Button::IsEnable() const
{
    return enabled_;
}

void Button::SetEnabled(bool enabled)
{
    if (enabled_ != enabled)
    {
        enabled_ = enabled;
    }
}

void Button::SetClickCallback(const Callback& func)
{
    click_callback_ = func;
}

void Button::SetPressedCallback(const Callback& func)
{
    pressed_callback_ = func;
}

void Button::SetMouseOverCallback(const Callback& func)
{
    mouse_over_callback_ = func;
}

void Button::SetMouseOutCallback(const Callback& func)
{
    mouse_out_callback_ = func;
}

Button::Status Button::GetStatus() const
{
    return status_;
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
                mouse_out_callback_(this, actor_);
        }
        else if (status == Status::Hover)
        {
            Application::GetInstance().GetMainWindow()->SetCursor(CursorType::Hand);

            if (old_status != Status::Pressed)
            {
                if (mouse_over_callback_)
                    mouse_over_callback_(this, actor_);
            }
        }
        else if (status == Status::Pressed)
        {
            if (pressed_callback_)
                pressed_callback_(this, actor_);
        }

        status_ = status;
    }
}

void Button::HandleEvent(Event* evt)
{
    if (!enabled_)
        return;

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
            click_callback_(this, actor_);
    }
}

void Button::BindActor(Actor* actor)
{
    Component::BindActor(actor);
    if (actor)
    {
        actor->SetResponsible(true);
    }
}

}  // namespace kiwano
