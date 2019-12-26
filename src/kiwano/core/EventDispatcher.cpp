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

#include <kiwano/core/EventDispatcher.h>
#include <kiwano/core/Logger.h>

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

			if (listener->IsRunning())
			{
				listener->Receive(evt);
			}

			if (listener->IsRemoveable())
			{
				listeners_.remove(listener);
			}
		}
	}

	EventListener* EventDispatcher::AddListener(EventListenerPtr listener)
	{
		return AddListener(listener.get());
	}

	EventListener* EventDispatcher::AddListener(EventListener* listener)
	{
		KGE_ASSERT(listener && "AddListener failed, NULL pointer exception");

		if (listener)
		{
			listeners_.push_back(listener);
		}
		return listener;
	}

	EventListener* EventDispatcher::AddListener(String const& name, EventType type, EventListener::Callback callback)
	{
		EventListenerPtr listener = new EventListener(name, type, callback);
		return AddListener(listener);
	}

	EventListener* EventDispatcher::AddListener(EventType type, EventListener::Callback callback)
	{
		EventListenerPtr listener = new EventListener(type, callback);
		return AddListener(listener);
	}

	void EventDispatcher::StartListeners(String const & listener_name)
	{
		for (auto& listener : listeners_)
		{
			if (listener->IsName(listener_name))
			{
				listener->Start();
			}
		}
	}

	void EventDispatcher::StopListeners(String const & listener_name)
	{
		for (auto& listener : listeners_)
		{
			if (listener->IsName(listener_name))
			{
				listener->Stop();
			}
		}
	}

	void EventDispatcher::RemoveListeners(String const & listener_name)
	{
		for (auto& listener : listeners_)
		{
			if (listener->IsName(listener_name))
			{
				listener->Remove();
			}
		}
	}

	void EventDispatcher::StartListeners(const EventType& type)
	{
		for (auto& listener : listeners_)
		{
			if (listener->type_ == type)
			{
				listener->Start();
			}
		}
	}

	void EventDispatcher::StopListeners(const EventType& type)
	{
		for (auto& listener : listeners_)
		{
			if (listener->type_ == type)
			{
				listener->Stop();
			}
		}
	}

	void EventDispatcher::RemoveListeners(const EventType& type)
	{
		for (auto& listener : listeners_)
		{
			if (listener->type_ == type)
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

	const EventDispatcher::Listeners& EventDispatcher::GetAllListeners() const
	{
		return listeners_;
	}

}
