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

#include <kiwano/component/MouseSensor.h>
#include <kiwano/2d/Actor.h>

namespace kiwano
{
MouseSensor::MouseSensor()
    : hover_(false)
    , pressed_(false)
{
    SetName("__KGE_MOUSE_SENSOR_COMPONENT__");
}

MouseSensor::~MouseSensor() {}

void MouseSensor::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);

    listener_ = EventListener::Create("__KGE_MOUSE_SENSOR_LISTENER__",
                                      std::bind(&MouseSensor::HandleEvent, this, std::placeholders::_1));
    actor->AddListener(listener_);
}

void MouseSensor::DestroyComponent()
{
    if (listener_)
        listener_->Remove();

    Component::DestroyComponent();
}

void MouseSensor::HandleEvent(Event* evt)
{
    Actor* target = GetBoundActor();
    if (evt->IsType<MouseMoveEvent>())
    {
        auto mouse_evt = dynamic_cast<MouseMoveEvent*>(evt);
        bool contains  = target->ContainsPoint(mouse_evt->pos);
        if (!hover_ && contains)
        {
            hover_ = true;

            RefPtr<MouseHoverEvent> hover = new MouseHoverEvent;
            hover->pos                    = mouse_evt->pos;
            GetBoundActor()->DispatchEvent(hover.Get());
        }
        else if (hover_ && !contains)
        {
            hover_   = false;
            pressed_ = false;

            RefPtr<MouseOutEvent> out = new MouseOutEvent;
            out->pos                  = mouse_evt->pos;
            GetBoundActor()->DispatchEvent(out.Get());
        }
    }

    if (evt->IsType<MouseDownEvent>() && hover_)
    {
        pressed_ = true;
    }

    if (evt->IsType<MouseUpEvent>() && pressed_)
    {
        pressed_ = false;

        auto mouse_up_evt = dynamic_cast<MouseUpEvent*>(evt);

        RefPtr<MouseClickEvent> click = new MouseClickEvent;
        click->pos                    = mouse_up_evt->pos;
        click->button                 = mouse_up_evt->button;
        GetBoundActor()->DispatchEvent(click.Get());
    }
}

}  // namespace kiwano
