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

#include <kiwano/event/EventDispatcher.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{
bool EventDispatcher::DispatchEvent(Event* evt)
{
    if (listeners_.IsEmpty())
        return true;

    RefPtr<EventListener> next;
    for (auto listener = listeners_.GetFirst(); listener; listener = next)
    {
        next = listener->GetNext();

        if (listener->IsRunning())
            listener->Handle(evt);

        if (listener->IsRemoveable())
            listeners_.Remove(listener);

        if (listener->IsSwallowEnabled())
            return false;
    }
    return true;
}

EventListener* EventDispatcher::AddListener(RefPtr<EventListener> listener)
{
    KGE_ASSERT(listener && "AddListener failed, NULL pointer exception");

    if (listener)
    {
        listeners_.PushBack(listener);
    }
    return listener.Get();
}

EventListener* EventDispatcher::AddListener(StringView name, EventType type, EventListener::Callback callback)
{
    auto lis = EventListener::Create(name, type, callback);
    return AddListener(lis);
}

EventListener* EventDispatcher::AddListener(EventType type, EventListener::Callback callback)
{
    auto lis = EventListener::Create(type, callback);
    return AddListener(lis);
}

void EventDispatcher::StartListeners(StringView name)
{
    for (auto& listener : listeners_)
    {
        if (listener->IsName(name))
        {
            listener->Start();
        }
    }
}

void EventDispatcher::StopListeners(StringView name)
{
    for (auto& listener : listeners_)
    {
        if (listener->IsName(name))
        {
            listener->Stop();
        }
    }
}

void EventDispatcher::RemoveListeners(StringView name)
{
    for (auto& listener : listeners_)
    {
        if (listener->IsName(name))
        {
            listener->Remove();
        }
    }
}

void EventDispatcher::StartAllListeners()
{
    for (auto& listener : listeners_)
    {
        listener->Start();
    }
}

void EventDispatcher::StopAllListeners()
{
    for (auto& listener : listeners_)
    {
        listener->Stop();
    }
}

void EventDispatcher::RemoveAllListeners()
{
    for (auto& listener : listeners_)
    {
        listener->Remove();
    }
}

const ListenerList& EventDispatcher::GetAllListeners() const
{
    return listeners_;
}

}  // namespace kiwano
