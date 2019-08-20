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

#include "EventDispatcher.h"
#include "../base/Logger.h"

namespace kiwano
{
	void EventDispatcher::Dispatch(Event& evt)
	{
		if (listeners_.empty())
			return;

		EventListenerPtr next;
		for (auto listener = listeners_.first_item(); listener; listener = next)
		{
			next = listener->next_item();

			if (listener->type_ == evt.type)
			{
				listener->callback_(evt);
			}
		}
	}

	EventListener* EventDispatcher::AddListener(EventListenerPtr listener)
	{
		KGE_ASSERT(listener && "AddListener failed, NULL pointer exception");

		if (listener)
		{
			listeners_.push_back(listener);
		}
		return listener.get();
	}

	EventListener* EventDispatcher::AddListener(UInt32 type, EventCallback callback, String const& name)
	{
		EventListenerPtr listener = new EventListener(type, callback, name);
		return AddListener(listener);
	}

	void EventDispatcher::StartListeners(String const & listener_name)
	{
		for (auto listener = listeners_.first_item(); listener; listener = listener->next_item())
		{
			if (listener->IsName(listener_name))
			{
				listener->Start();
			}
		}
	}

	void EventDispatcher::StopListeners(String const & listener_name)
	{
		for (auto listener = listeners_.first_item(); listener; listener = listener->next_item())
		{
			if (listener->IsName(listener_name))
			{
				listener->Stop();
			}
		}
	}

	void EventDispatcher::RemoveListeners(String const & listener_name)
	{
		EventListenerPtr next;
		for (auto listener = listeners_.first_item(); listener; listener = next)
		{
			next = listener->next_item();

			if (listener->IsName(listener_name))
			{
				listeners_.remove(listener);
			}
		}
	}

	void EventDispatcher::StartListeners(UInt32 type)
	{
		for (auto listener = listeners_.first_item(); listener; listener = listener->next_item())
		{
			if (listener->type_ == type)
			{
				listener->Start();
			}
		}
	}

	void EventDispatcher::StopListeners(UInt32 type)
	{
		for (auto listener = listeners_.first_item(); listener; listener = listener->next_item())
		{
			if (listener->type_ == type)
			{
				listener->Stop();
			}
		}
	}

	void EventDispatcher::RemoveListeners(UInt32 type)
	{
		EventListenerPtr next;
		for (auto listener = listeners_.first_item(); listener; listener = next)
		{
			next = listener->next_item();

			if (listener->type_ == type)
			{
				listeners_.remove(listener);
			}
		}
	}

}
