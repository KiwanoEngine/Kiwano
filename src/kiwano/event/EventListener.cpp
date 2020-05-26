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
#include <kiwano/event/EventListener.h>

namespace kiwano
{

EventListenerPtr EventListener::Create(EventType type, const Callback& callback)
{
    EventListenerPtr ptr = memory::New<EventListener>();
    if (ptr)
    {
        ptr->SetEventType(type);
        ptr->SetCallback(callback);
    }
    return ptr;
}

EventListenerPtr EventListener::Create(const String& name, EventType type, const Callback& callback)
{
    EventListenerPtr ptr = memory::New<EventListener>();
    if (ptr)
    {
        ptr->SetName(name);
        ptr->SetEventType(type);
        ptr->SetCallback(callback);
    }
    return ptr;
}

EventListener::EventListener()
    : type_()
    , callback_()
    , running_(true)
    , removeable_(false)
    , swallow_(false)
{
}

EventListener::~EventListener() {}

}  // namespace kiwano
