// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "EventDispatcher.h"
#include "logs.h"

namespace easy2d
{
	void EventDispatcher::DispatchEvent(Event* e)
	{
		if (listeners_.IsEmpty())
			return;

		EventListenerPtr next;
		for (auto listener = listeners_.First(); listener; listener = next)
		{
			next = listener->NextItem();

			if (listener->type_ == e->type)
			{
				listener->callback_(e);
			}
		}
	}

	void EventDispatcher::AddListener(EventListenerPtr const & listener)
	{
		E2D_ASSERT(listener && "AddListener failed, NULL pointer exception");

		if (listener)
		{
			listeners_.PushBack(listener);
		}
	}

	void EventDispatcher::AddListener(EventType type, EventCallback callback, String const& name)
	{
		EventListenerPtr listener = new EventListener(type, callback, name);
		if (listener)
		{
			listeners_.PushBack(listener);
		}
	}

	void EventDispatcher::StartListeners(String const & listener_name)
	{
		for (auto listener = listeners_.First(); listener; listener = listener->NextItem())
		{
			if (listener->GetName() == listener_name)
			{
				listener->Start();
			}
		}
	}

	void EventDispatcher::StopListeners(String const & listener_name)
	{
		for (auto listener = listeners_.First(); listener; listener = listener->NextItem())
		{
			if (listener->GetName() == listener_name)
			{
				listener->Stop();
			}
		}
	}

	void EventDispatcher::RemoveListeners(String const & listener_name)
	{
		EventListenerPtr next;
		for (auto listener = listeners_.First(); listener; listener = next)
		{
			next = listener->NextItem();

			if (listener->GetName() == listener_name)
			{
				listeners_.Remove(listener);
			}
		}
	}

	void EventDispatcher::StartListeners(EventType type)
	{
		for (auto listener = listeners_.First(); listener; listener = listener->NextItem())
		{
			if (listener->type_ == type)
			{
				listener->Start();
			}
		}
	}

	void EventDispatcher::StopListeners(EventType type)
	{
		for (auto listener = listeners_.First(); listener; listener = listener->NextItem())
		{
			if (listener->type_ == type)
			{
				listener->Stop();
			}
		}
	}

	void EventDispatcher::RemoveListeners(EventType type)
	{
		EventListenerPtr next;
		for (auto listener = listeners_.First(); listener; listener = next)
		{
			next = listener->NextItem();

			if (listener->type_ == type)
			{
				listeners_.Remove(listener);
			}
		}
	}

}
